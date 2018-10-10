#include "depthfirstsearch.h"

DepthFirstSearch::DepthFirstSearch(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side)
{
    if(dock_show)
        dock = std::make_shared<ProgresDock>();
}

const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> DepthFirstSearch::agentAct(int now_turn){

    if(do_output)
        std::cout << "turn : " << now_turn << std::endl;

    if(now_turn + maxval >= field.getFinalTurn()){
        LastRegion algo(field, final_turn, side);
        return algo.agentAct(now_turn);
    }

    maxval = std::min(maxval, final_turn - now_turn);
    predict_per.resize(4, std::vector<std::vector<std::vector<double>>>(maxval, std::vector<std::vector<double>>(field.getSize().first, std::vector<double>(field.getSize().second, 0.0))));

    std::shared_ptr<SearchNode> node_1, node_2;
    std::list<std::pair<int,int>> moves_1, moves_2;
    std::vector<std::vector<std::vector<double>>> states_1, states_2;
    std::vector<std::vector<std::vector<int>>> agent_states_1, agent_states_2;

    std::vector<std::vector<double>> after_values(field.getSize().first, std::vector<double>(field.getSize().second, 1));
	// 元々の盤面状況
    std::vector<std::vector<int>> states(field.getSize().first, std::vector<int>(field.getSize().second, 1));

    for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
        for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y){
            int pos_state = field.getState(pos_x, pos_y).first;
            if(pos_state){
                states.at(pos_x).at(pos_y) = 2 * (pos_state != side + 1);
                after_values.at(pos_x).at(pos_y) = 2 * (pos_state != side + 1);
            }
        }

	// 勝率予測用をマルチスレッドで動かす
    for(int cou = 0; cou < loop_count; ++cou){
        std::vector<std::thread> threads;
        for(int index = 0; index < 4; ++index)
            threads.push_back(std::thread([&](int arg){updatePredictData(arg & 2, arg & 1, dock_show && (side != ((arg & 2) >> 1)) && (cou == loop_count - 1));}, index));

        for(int index = 0; index < 4; ++index)
            threads.at(index).join();
    }

	// 予測をもとにした各深さごとのペナルティ(高いほど味方が踏みやすい)(累積しない)
    std::vector<std::vector<std::vector<double>>> pred(maxval, std::vector<std::vector<double>>(field.getSize().first, std::vector<double>(field.getSize().second, 0.0)));
    for(int index = 0; index < 4; ++index)
        if(index != side * 2)
            for(int depth = 0; depth < maxval; ++depth)
                for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
                    for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y)
                        pred.at(depth).at(pos_x).at(pos_y) += (index / 2 == side ? ally_weight : -1) * predict_per.at(index).at(depth).at(pos_x).at(pos_y);

    std::tie(node_1, moves_1, states_1) = calcMove(side, 0, states, pred);

    pred.resize(maxval, std::vector<std::vector<double>>(field.getSize().first, std::vector<double>(field.getSize().second, 0.0)));

    for(int index = 0; index < 4; ++index)
        if(index != side * 2 + 1)
            for(int depth = 0; depth < maxval; ++depth)
                for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
                    for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y)
                        pred.at(depth).at(pos_x).at(pos_y) += (index / 2 == side ? ally_weight : -1) * predict_per.at(index).at(depth).at(pos_x).at(pos_y);


    std::tie(node_2, moves_2, states_2) = calcMove(side, 1, states, pred);

	// kターン後にどのマスが踏まれるかの予測値の累積和を取る
    for(int depth = 0; depth < maxval - 1; ++depth)
        for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
            for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y){
                states_1.at(depth + 1).at(pos_x).at(pos_y) += states_1.at(depth).at(pos_x).at(pos_y);
                states_2.at(depth + 1).at(pos_x).at(pos_y) += states_2.at(depth).at(pos_x).at(pos_y);
            }

	// 探索する末尾の深さ
    int depth_index = std::max(maxval - 1, 0);

	// 現在の盤面から予測した変動量(の累積和)だけafter_valuesを変動させる
    for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
        for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y){

            after_values.at(pos_x).at(pos_y) -= states_1.at(depth_index).at(pos_x).at(pos_y);
            after_values.at(pos_x).at(pos_y) -= states_2.at(depth_index).at(pos_x).at(pos_y);

            after_values.at(pos_x).at(pos_y) = std::max(0.0, after_values.at(pos_x).at(pos_y));
        }

    if(do_output){
        std::cout << "node_1 size : " << node_1->size << " , " << node_1->real_size << std::endl;
        std::cout << "node_2 size : " << node_2->size << " , " << node_2->real_size << std::endl;
    }

	// after_valuesの値によって色付け
    std::vector<std::vector<std::vector<int>>> colors(3, std::vector<std::vector<int>>(field.getSize().first, std::vector<int>(field.getSize().second, 255)));
    for(int x_pos = 0; x_pos < field.getSize().first; ++x_pos)
        for(int y_pos = 0; y_pos < field.getSize().second; ++y_pos){
            if(after_values.at(x_pos).at(y_pos) > 1){
                colors.at(0).at(x_pos).at(y_pos) -= 255 * (after_values.at(x_pos).at(y_pos) - 1);
                colors.at(1).at(x_pos).at(y_pos) -= 255 * (after_values.at(x_pos).at(y_pos) - 1);

            }else if(after_values.at(x_pos).at(y_pos) < 1){
                colors.at(1).at(x_pos).at(y_pos) -= 255 * (1 - after_values.at(x_pos).at(y_pos));
                colors.at(2).at(x_pos).at(y_pos) -= 255 * (1 - after_values.at(x_pos).at(y_pos));
            }

            for(int index = 0; index < 3; ++index)
                colors.at(index).at(x_pos).at(y_pos) = std::max(0, colors.at(index).at(x_pos).at(y_pos));
        }


    std::vector<std::vector<std::vector<int>>> colors_1(3, std::vector<std::vector<int>>(field.getSize().first, std::vector<int>(field.getSize().second, 255)));
    std::vector<std::vector<std::vector<int>>> colors_2(3, std::vector<std::vector<int>>(field.getSize().first, std::vector<int>(field.getSize().second, 255)));

	// 各エージェントの出した予測値(の累積和)を元に色つけ
    for(int x_pos = 0; x_pos < field.getSize().first; ++x_pos)
        for(int y_pos = 0; y_pos < field.getSize().second; ++y_pos){

            colors_1.at(1).at(x_pos).at(y_pos) -= 255 * states_1.at(maxval - 1).at(x_pos).at(y_pos);
            colors_1.at(1).at(x_pos).at(y_pos) = std::max(0, colors_1.at(1).at(x_pos).at(y_pos));
            colors_2.at(1).at(x_pos).at(y_pos) -= 255 * states_2.at(maxval - 1).at(x_pos).at(y_pos);
            colors_2.at(1).at(x_pos).at(y_pos) = std::max(0, colors_2.at(1).at(x_pos).at(y_pos));

            colors_1.at(2 - 2 * side).at(x_pos).at(y_pos) -= 255 * states_1.at(maxval - 1).at(x_pos).at(y_pos);
            colors_1.at(2 - 2 * side).at(x_pos).at(y_pos) = std::max(0, colors_1.at(2 - 2 * side).at(x_pos).at(y_pos));
            colors_2.at(2 - 2 * side).at(x_pos).at(y_pos) -= 255 * states_2.at(maxval - 1).at(x_pos).at(y_pos);
            colors_2.at(2 - 2 * side).at(x_pos).at(y_pos) = std::max(0, colors_2.at(2 - 2 * side).at(x_pos).at(y_pos));
        }

    if(dock_show){
        std::pair<int,int> siz = field.getSize();

		// dock_stackに追加 valueには踏まれる可能性の(累積和の)値を格納
        addVisualizerToDock(siz, std::vector<std::list<std::pair<int,int>>>({moves_2}), colors_2, states_1.at(maxval - 1));
        addVisualizerToDock(siz, std::vector<std::list<std::pair<int,int>>>({moves_1}), colors_1, states_2.at(maxval - 1));

        std::pair<int,int> pos;
        std::vector<std::list<std::pair<int,int>>> route;
        std::vector<std::vector<std::vector<int>>> value_int;
        std::vector<std::vector<double>> value_double;

        while(!dock_stack.empty()){

			// dockに投入
            pos = std::get<0>(dock_stack.top());
            route = std::get<1>(dock_stack.top());
            value_int = std::get<2>(dock_stack.top());
            value_double = std::get<3>(dock_stack.top());
            std::tie(pos, route, value_int, value_double) = dock_stack.top();

            dock->addMinumuVisu(pos, route, value_int);
            std::vector<std::vector<double>> values = std::move(value_double);
            if(!values.empty())
                dock->setValuesToBack(values);
            dock_stack.pop();
        }
    }

    if(vis_show){
        minimum = std::make_shared<MinimumVisualizer>(field.getSize());

        /*
        std::vector<std::list<std::pair<int,int>>> use_vec = std::vector<std::list<std::pair<int,int>>>({moves_1, moves_2});
        minimum->setRoute(use_vec);
        */

		// 色指定してビジュアライザに投入
        for(int index = 0; index < 3; ++index)
            minimum->setValues(colors.at(index), index);

        minimum->show();
    }

    if(node_1->size == 1 || node_2->size == 1){
        procon::CsvIo::exportField(field, "./error_case.csv");
        std::cerr << "error_case\n";
        return std::make_pair(std::make_tuple(0, 0, 0), std::make_tuple(0, 0, 0));
    }

	// (コンフリクトを考慮していない)最大利得の手
    int move_1 = node_1->getMaxAdvMove().second;
    int move_2 = node_2->getMaxAdvMove().second;



	// コンフリクト考慮をした上での手を取得
    std::pair<std::pair<int,int>,int> ins = getMaxAdvMove(node_1, node_2);

    move_1 = ins.first.first;
    move_2 = ins.first.second;


    node_1.reset();
    node_2.reset();


    if(move_1 < 0 || move_2 < 0){
        procon::CsvIo::exportField(field, "./error_case.csv");
        std::cerr << "error_case\n";
        return std::make_pair(std::make_tuple(0, 0, 0), std::make_tuple(0, 0, 0));
    }

    std::vector<std::pair<int,int>> agent_pos(2);
    for(int index = 0; index < 2; ++index)
        agent_pos.at(index) = field.getAgent(side, index);


    return std::make_pair(std::make_tuple((field.getState(agent_pos.at(0).first + SearchNode::dx.at(move_1), agent_pos.at(0).second + SearchNode::dy.at(move_1)).first != (side ? 1 : 2) ? 1 : 2), SearchNode::dx.at(move_1), SearchNode::dy.at(move_1)),
                          std::make_tuple((field.getState(agent_pos.at(1).first + SearchNode::dx.at(move_2), agent_pos.at(1).second + SearchNode::dy.at(move_2)).first != (side ? 1 : 2) ? 1 : 2), SearchNode::dx.at(move_2), SearchNode::dy.at(move_2)));
}

void DepthFirstSearch::setParams(std::vector<std::pair<QString, double>> params)
{
    // 渡されたvectorから該当のパラメータを検索
    // param : 検索するパラメータ名
    auto search = [&](std::string param_name) {
        // 渡されたvectorのすべての要素を検索する
        int count = params.size();

        for (int i = 0; i < count; i++) {

            // vector内のQStringにパラメータ名が含まれていないか確認
            if (params.at(i).first.toStdString().find(param_name) != std::string::npos) {
                // 該当するものがあったら数値を返す
                return params.at(i).second;
            }
        }
        // 見つからなかったらdoubleの最大値を返す
        return std::numeric_limits<double>::max();
    };

    bool dockshow_before = dock_show;

    // 自チームのパラメータを設定
    dock_show = (bool)search("dock_show");
    vis_show = (bool)search("vis_show");
    maxval = (int)search("maxval");
    loop_count = (int)search("loop_count");
    use_beamsearch = (bool)search("use_beamsearch");
    beam_width = (int)search("beam_width");
    ally_weight = search("ally_weight");
    movecount = (int)search("movecount");
    predict_weight = search("predict_weight");
    ratio = search("ratio");

    if(!dockshow_before && dock_show){
        dock = std::make_shared<ProgresDock>();
        dock->show();
    }
}

// Parametersが入力された時に反映
void DepthFirstSearch::setParams(DepthFirstSearch::Parameters& params){
    assert(params.ally_weight >= -5 && params.ally_weight <= 15);
    assert(params.predict_weight >= -5 && params.predict_weight <= 5);
    assert(params.ratio > 0 && params.ratio <= 1);
    assert(params.movecount > 0);
    assert(params.beam_width > 0);
    assert(params.maxval > 0);
    assert(params.deverse_per >= 0);
    assert(params.conflict_atk_per >= 0);
    assert(params.conflict_def_per >= 0);

    ally_weight      = params.ally_weight;

    beam_width       = params.beam_width;
    ally_weight      = params.ally_weight;
    conflict_atk_per = params.conflict_atk_per;
    conflict_def_per = params.conflict_def_per;
    deverse_per      = params.deverse_per;
    loop_count       = params.loop_count;
    predict_weight   = params.predict_weight;
    ratio            = params.ratio;
    use_beamsearch   = params.use_beamsearch;
    maxval           = params.maxval;

    if(params.pena_ratio_val != -1){
        double value = 1;
        for(auto& val : penaRatio){
            val = value;
            value *= params.pena_ratio_val;
        }
    }
    else
        penaRatio        = params.penaRatio;

    if(params.depth_weight_val != -1){
        double value = 1;
        for(auto it = std::prev(depth_weight.end()); it >= depth_weight.begin(); --it){
            auto& val = (*it);
            val = value;
            value *= params.depth_weight_val;
        }
    }
    else
        depth_weight        = params.depth_weight;

    if(params.conflict_weight_val != -1){
        double value = 1;
        for(auto it = std::prev(conflict_weight.end()); it >= conflict_weight.begin(); --it){
            auto& val = (*it);
            val = value;
            value *= params.conflict_weight_val;
        }
    }
    else
        conflict_weight        = params.conflict_weight;
}

// Parametersを取得
DepthFirstSearch::Parameters DepthFirstSearch::getParams(){
    Parameters params;
    params.ally_weight      = ally_weight;
    params.beam_width       = beam_width;
    params.ally_weight      = ally_weight;
    params.conflict_atk_per = conflict_atk_per;
    params.conflict_def_per = conflict_def_per;
    params.depth_weight     = depth_weight;
    params.conflict_weight  = conflict_weight;
    params.deverse_per      = deverse_per;
    params.loop_count       = loop_count;
    params.predict_weight   = predict_weight;
    params.ratio            = ratio;
    params.use_beamsearch   = use_beamsearch;
    params.maxval = maxval;

    params.penaRatio = penaRatio;

    return params;
}

// beamsearch 本質
std::shared_ptr<DepthFirstSearch::SearchNode> DepthFirstSearch::createNodeWithBeamSearch(bool inp_side, bool agent, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict){

	// pri_que代わりのtreap
    std::vector<std::shared_ptr<Treap>> treap_vec;
    for(int count = 0; count < 2; ++count)
        treap_vec.emplace_back(std::make_shared<Treap>());

    std::shared_ptr<SearchNode> parent = std::make_shared<SearchNode>(0.0, 0, predict_weight, movecount);
    treap_vec.at(0)->insert(std::make_pair(0.0, std::make_pair(0.0, parent)));

	// 盤面の値をdoubleに置き換えて保管、最大値で割って[1,-1]の範囲で平均化する
    const std::vector<std::vector<int>>& value = field.getValue();
    std::vector<std::vector<double>> double_value(field.getSize().first, std::vector<double>(field.getSize().second));
    int max_value = 0;
    for(int x_index = 0; x_index < field.getSize().first; ++x_index)
        for(int y_index = 0; y_index < field.getSize().second; ++y_index)
            max_value = std::max(max_value, std::abs(value.at(x_index).at(y_index)));

    for(int x_index = 0; x_index < field.getSize().first; ++x_index)
        for(int y_index = 0; y_index < field.getSize().second; ++y_index)
            double_value.at(x_index).at(y_index) = 1.0 * value.at(x_index).at(y_index) / max_value;


    std::pair<int,int> old_pos = field.getAgent(inp_side, agent);
    for(int dep = 0; dep < maxval + 1; ++dep){

		// beforeを吐き出しながらafterに詰め込む
        std::shared_ptr<Treap> before = treap_vec.at(dep % 2);
        std::shared_ptr<Treap> after = treap_vec.at((dep + 1) % 2);

		// 重複判定に使うやつ
        std::vector<std::vector<int>> put_tile_count(field.getSize().first, std::vector<int>(field.getSize().second, 0));
        int put_tile_sum = 0;

        std::map<std::bitset<296>, std::shared_ptr<SearchNode>, BitSetSorter> node_map;
		// 全ての手に対して
        while(before->size()){

            std::pair<double, std::shared_ptr<SearchNode>> element = before->get(0).second;
            double now_adv = element.first;
            std::shared_ptr<SearchNode> node = element.second;
            before->erase(0);

            if(dep == maxval){
                node->is_back = true;
                continue;
            }

            SearchNode* now_node = node.get();

			// 遡りながら移動先を保存(方向,is_move)
			// rev_moveはbackが根側になる
            std::vector<std::pair<int,int>> rev_move;
            while(now_node->parent.first){
                rev_move.emplace_back(now_node->parent.second, now_node->parent.first->childs[now_node->parent.second].second);
                now_node = now_node->parent.first;
            }

            std::bitset<296> bs;

            double conflict_value = 0.0;

            std::pair<int,int> pos(old_pos);

			// 根から子の方向へ進んでbitsetに回数を格納、put_tile_countに加算する
            for(int index = rev_move.size() - 1; index >= 0; --index){
                int move_index = rev_move.at(index).first;
                bool is_move = rev_move.at(index).second & 1;
                bool is_replace = rev_move.at(index).second & 2;

                if(is_replace){
                    --state.at(pos.first + SearchNode::dx.at(move_index)).at(pos.second + SearchNode::dy.at(move_index));

                    int bit_index = 8 + ((pos.first + SearchNode::dx.at(move_index)) * 12 + (pos.second + SearchNode::dy.at(move_index))) * 2;
                    int bit_count = ((bs >> bit_index) & std::bitset<296>((1LL << 32) - 1)).to_ulong() & 3;
                    bs &= ~(std::bitset<296>(3) << bit_index);
                    bs |= std::bitset<296>(bit_count + 1) << bit_index;
                }


                if(put_tile_sum)
                    conflict_value += conflict_weight.at(rev_move.size() - 1 - index) * put_tile_count.at(pos.first + SearchNode::dx.at(move_index)).at(pos.second + SearchNode::dy.at(move_index)) / put_tile_sum;

                ++put_tile_count.at(pos.first + SearchNode::dx.at(move_index)).at(pos.second + SearchNode::dy.at(move_index));
                ++put_tile_sum;

                if(is_move){
                    pos.first += SearchNode::dx.at(move_index);
                    pos.second += SearchNode::dy.at(move_index);
                }
            }

            bs &= ~std::bitset<296>(255);
            bs |= ((pos.first << 4) | pos.second);

			// 全方向に対して
            for(int index = 0; index < 8; ++index){
                int x_pos = pos.first + SearchNode::dx.at(index);
                int y_pos = pos.second + SearchNode::dy.at(index);
				// 行動が可能なら
                if(!(x_pos < 0 || y_pos < 0 || x_pos >= static_cast<int>(state.size()) || y_pos >= static_cast<int>(state.at(0).size()))){

                    std::pair<int,int> newpos_pair(x_pos, y_pos);
					// 敵エージェントの位置
                    std::vector<std::pair<int,int>> enemy_agents = {field.getAgent(inp_side ^ 1, 0), field.getAgent(inp_side ^ 1, 1)};

                    bool is_move = (state.at(x_pos).at(y_pos) != 2);
                    bool is_replace = state.at(x_pos).at(y_pos);

                    bool is_conflict = (newpos_pair == enemy_agents.at(0)) || (newpos_pair == enemy_agents.at(1));
					// 敵のエージェントが8近傍に存在するなら
                    bool is_defence = ((std::abs(enemy_agents.at(0).first - x_pos) <= 1) && (std::abs(enemy_agents.at(0).second - y_pos) <= 1)) ||
                                      ((std::abs(enemy_agents.at(1).first - x_pos) <= 1) && (std::abs(enemy_agents.at(1).second - y_pos) <= 1));

					// 踏むマスの得点 * 深さごとの重み * (累積しない)そのマスの予測変化率
                    double point = depth_weight.at(dep) * double_value.at(x_pos).at(y_pos) * (1.0 - std::min(1.0, predict_weight * (dep ? predict.at(dep - 1).at(x_pos).at(y_pos) : 0)));

					// 諸々の処理(主に敵エージェントとのコンフリクト対応とか)
                    if(!is_replace)
                        point = 0;
                    if(is_conflict)
                        point *= conflict_atk_per;
                    if(is_defence && !is_move)
                        point *= conflict_def_per;

					// 次のTreapに採用する優先度は多様性ペナルティを考慮する
					// 今までの多様性ペナ + この行動での多様性ペナ
                    double conf_pri =  conflict_value + conflict_weight.at(rev_move.size()) * put_tile_count.at(x_pos).at(y_pos) / put_tile_sum;

                    ++put_tile_count.at(x_pos).at(y_pos);
                    ++put_tile_sum;

					// 多様性ペナを倍率としてかけてしまう
                    double priority = (now_adv + point) * (1 - std::min(1.0, conf_pri * deverse_per));

                    int bit_index = 8 + (x_pos * 12 + y_pos) * 2;
                    int bit_count = ((bs >> bit_index) & std::bitset<296>((1LL << 32) - 1)).to_ulong() & 3;

                    if(is_replace){
                        bs &= ~(std::bitset<296>(3) << bit_index);
                        bs |= std::bitset<296>(bit_count + 1) << bit_index;
                    }

					// 枝が重複しているならsize増やして終了(枝切り)
                    if(node_map.count(bs)){
                        ++node_map[bs]->size;
                    }else{
						// なんかいい感じに子を作ってる
                        node->childs[index] = std::make_pair(std::make_shared<SearchNode>(point, dep + 1, predict_weight, movecount), static_cast<int>(is_move) | (static_cast<int>(is_replace) << 1));
                        node->childs[index].first->parent = std::make_pair(node.get(), index);
                        node_map[bs] = node->childs[index].first;
                        after->insert(std::make_pair(priority, std::make_pair(now_adv + point, node->childs[index].first)));
                    }

                    if(is_replace){
                        bs &= ~(std::bitset<296>(3) << bit_index);
                        bs |= std::bitset<296>(bit_count) << bit_index;
                    }
                }
            }

            pos = old_pos;
			// 前と同様にstateを戻していく
            for(int index = rev_move.size() - 1; index >= 0; --index){
                int move_index = rev_move.at(index).first;
                bool is_move = rev_move.at(index).second & 1;
                bool is_replace = rev_move.at(index).second & 2;
                if(is_replace)
                    ++state.at(pos.first + SearchNode::dx.at(move_index)).at(pos.second + SearchNode::dy.at(move_index));
                if(is_move){
                    pos.first += SearchNode::dx.at(move_index);
                    pos.second += SearchNode::dy.at(move_index);
                }
            }

            if(node->childs.empty())
                node->is_back = true;

			// pri_queの最大サイズ制限
            if(after->size() > beam_width)
                after->erase_back(beam_width);
        }
    }
    treap_vec.clear();

    return parent;
}

std::tuple<std::shared_ptr<DepthFirstSearch::SearchNode>, std::list<std::pair<int,int>>, std::vector<std::vector<std::vector<double>>>> DepthFirstSearch::calcMove(bool inp_side, int agent, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict){

	// nodeを根にした木を生成する
    std::shared_ptr<SearchNode> node;
    if(use_beamsearch)
        node = createNodeWithBeamSearch(inp_side, agent, state, predict);
    else
        node = createNodeWithDepthSearch(inp_side, agent, state, predict);

    std::list<std::pair<int,int>> moves;
    std::pair<int,int> now_pos = field.getAgent(inp_side, agent);
    moves.emplace_back(now_pos);
    std::shared_ptr<SearchNode> now_node = node;
    bool is_move;
    std::pair<int,int> value(0, 0);

    while(1){
        value = now_node->getMaxAdvMove();
        if(value.first == -10007)
            break;

        is_move  = now_node->childs[value.second].second & 1;
        now_node = now_node->childs[value.second].first;

        if(is_move){
            now_pos.first += SearchNode::dx.at(value.second);
            now_pos.second += SearchNode::dy.at(value.second);
        }
        moves.emplace_back(now_pos);
    }

    std::vector<std::vector<std::vector<double>>> values(maxval, std::vector<std::vector<double>>(field.getSize().first, std::vector<double>(field.getSize().second, 0.0)));

    std::vector<std::vector<double>> after_values(field.getSize().first, std::vector<double>(field.getSize().second));

    std::vector<int> depth_size(maxval, 0);
    auto size_bfs = [&]{
        std::queue<std::shared_ptr<SearchNode>> que;

        que.push(node);
        while(!que.empty()){
            std::shared_ptr<SearchNode> now_ptr = que.front();
            int depth = now_ptr->depth;

            now_ptr->leaf_size = now_ptr->size + 1;

            for(auto ch : now_ptr->childs){
                ch.second.first->size += now_ptr->size;
                que.push(ch.second.first);
            }
            if(depth == maxval)
                ++now_ptr->size;
            else
                now_ptr->size = 0;
            que.pop();
        }
    };
    size_bfs();

    node->dfsAdd(field.getAgent(inp_side, agent), values);

    std::queue<std::shared_ptr<SearchNode>> que;

    que.push(node);
    while(!que.empty()){
        std::shared_ptr<SearchNode> now_ptr = que.front();

        for(auto ch : now_ptr->childs)
            que.push(ch.second.first);

        if(now_ptr->depth)
            depth_size.at(now_ptr->depth - 1) += now_ptr->size;
        que.pop();
    }

    for(int dep = 0; dep < maxval; ++dep){
        std::vector<std::vector<double>>& vec = values.at(dep);
        int siz = depth_size.at(dep);
        std::for_each(vec.begin(), vec.end(), [siz](std::vector<double>& v){std::for_each(v.begin(), v.end(), [siz](double& val){if(val && siz)val /= siz;});});
    }

	// valuesは累積されていないものなはず
    return std::make_tuple(node, moves, values);
}

void DepthFirstSearch::updatePredictData(bool inp_side, bool agent, bool is_adddock){

    std::vector<std::vector<std::vector<double>>> ret_val = getMovePer(inp_side, agent, is_adddock);

    double diff = 0.0;
    for(int dep = 0; dep < maxval; ++dep)
        for(int x_index = 0; x_index < field.getSize().first; ++x_index)
            for(int y_index = 0; y_index < field.getSize().second; ++y_index){
                diff += std::abs(predict_per.at(inp_side * 2 + agent).at(dep).at(x_index).at(y_index) - ret_val.at(dep).at(x_index).at(y_index));
                predict_per.at(inp_side * 2 + agent).at(dep).at(x_index).at(y_index) = ret_val.at(dep).at(x_index).at(y_index);
            }
    if(do_output)
        std::cout << "diff : " << diff << std::endl;

}

std::vector<std::vector<std::vector<double>>> DepthFirstSearch::getMovePer(bool inp_side, bool agent, bool is_adddock){

    int now_turn = field.getTurnCount();
    maxval = std::min(maxval, final_turn - now_turn);

    std::vector<std::vector<std::vector<double>>> ret_states;

    std::vector<std::vector<int>> states(field.getSize().first, std::vector<int>(field.getSize().second, 1));
    for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
        for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y){
            int pos_state = field.getState(pos_x, pos_y).first;
            if(pos_state)
                states.at(pos_x).at(pos_y) = 2 * (pos_state != inp_side + 1);
        }

    std::vector<std::vector<std::vector<double>>> pred(maxval, std::vector<std::vector<double>>(field.getSize().first, std::vector<double>(field.getSize().second, 0.0)));
    for(int index = 0; index < 4; ++index)
        if(index != inp_side * 2 + agent)
            for(int depth = 0; depth < maxval; ++depth)
                for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
                    for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y)
                        pred.at(depth).at(pos_x).at(pos_y) += (index / 2 == inp_side ? ally_weight : -1) * predict_per.at(index).at(depth).at(pos_x).at(pos_y);

    if(is_adddock){
        std::list<std::pair<int,int>> moves;
        std::tie(std::ignore, moves, ret_states) = calcMove(inp_side, agent, states, pred);
        std::vector<std::vector<std::vector<int>>> colors(3, std::vector<std::vector<int>>(field.getSize().first, std::vector<int>(field.getSize().second, 255)));

        double hog = 0.0;

        for(int dep = 0; dep < maxval; ++dep)
            for(int x_pos = 0; x_pos < field.getSize().first; ++x_pos)
                for(int y_pos = 0; y_pos < field.getSize().second; ++y_pos){

                    if(inp_side)
                        colors.at(0).at(x_pos).at(y_pos) -= 255 * ret_states.at(dep).at(x_pos).at(y_pos);
                    else
                        colors.at(2).at(x_pos).at(y_pos) -= 255 * ret_states.at(dep).at(x_pos).at(y_pos);

                    colors.at(1).at(x_pos).at(y_pos) -= 255 * ret_states.at(dep).at(x_pos).at(y_pos);

                    for(int index = 0; index < 3; ++index)
                        colors.at(index).at(x_pos).at(y_pos) = std::max(0, colors.at(index).at(x_pos).at(y_pos));
                }

        std::pair<int,int> siz = field.getSize();
        addVisualizerToDock(siz, std::vector<std::list<std::pair<int,int>>>({moves}), colors);

    }else
        std::tie(std::ignore, std::ignore, ret_states) = calcMove(inp_side, agent, states, pred);

    for(int depth = 0; depth < maxval - 1; ++depth)
        for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
            for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y)
                ret_states.at(depth + 1).at(pos_x).at(pos_y) += ret_states.at(depth).at(pos_x).at(pos_y);

    return ret_states;
}

void DepthFirstSearch::addVisualizerToDock(const std::pair<int,int>& size, const std::vector<std::list<std::pair<int,int>>>& route, const std::vector<std::vector<std::vector<int>>>& color, const std::vector<std::vector<double>>& values){
    std::lock_guard<std::mutex> lock(mtx);
    dock_stack.push(std::make_tuple(size, route, color, values));
}


std::shared_ptr<DepthFirstSearch::SearchNode> DepthFirstSearch::createNodeWithDepthSearch(bool inp_side, bool agent, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict){
    const std::vector<std::vector<int>>& field_values = field.getValue();

    std::pair<int,int> agent_pos = field.getAgent(inp_side, agent);
    std::bitset<296> bs((agent_pos.first << 4) + agent_pos.second);

    std::map<std::bitset<296>, std::shared_ptr<SearchNode>, BitSetSorter> node_map;
    return std::make_shared<SearchNode>(0, 0, maxval, agent_pos, inp_side, field_values, state, node_map, bs, predict, predict_weight, movecount);

}


DepthFirstSearch::SearchNode::SearchNode(double adv, int depth, int remain, std::pair<int,int> pos, int side, const std::vector<std::vector<int>>& value, std::vector<std::vector<int>>& state, std::map<std::bitset<296>, std::shared_ptr<SearchNode>, BitSetSorter>& node_map, std::bitset<296>& bs, const std::vector<std::vector<std::vector<double>>>& predict, double predict_weight, int movecount) :

    depth(depth),
    size(0),
    real_size(1),
    adv(adv),
    predict_weight(predict_weight),
    movecount(movecount)
{
    // 末尾ノード
    if(!remain){
        is_back = true;
        return ;
    }

    // {{point, ismove}, dir}
    std::vector<std::pair<std::pair<double, int>, int>> moves;
    for(int index = 0; index < 8; ++index){

        std::pair<double, int> ret_pair(-10007.0, false);
        int x_pos = pos.first + dx.at(index);
        int y_pos = pos.second + dy.at(index);

        if(!(x_pos < 0 || y_pos < 0 || x_pos >= static_cast<int>(state.size()) || y_pos >= static_cast<int>(state.at(0).size()))){

            double point = (state.at(x_pos).at(y_pos) ? value.at(x_pos).at(y_pos) * (1.0 - predict_weight * predict.at(depth).at(x_pos).at(y_pos)) : 0.0);

            ret_pair = std::make_pair(point, static_cast<int>(state.at(x_pos).at(y_pos) != 2) | (static_cast<int>(state.at(x_pos).at(y_pos) != 0) << 1));
        }

        moves.emplace_back(std::make_pair(std::move(ret_pair), index));
    }

    std::sort(moves.begin(), moves.end());

    double bound_val = std::max(moves.at(8 - movecount).first.first, 0.0);

    moves.erase(moves.begin(), std::lower_bound(moves.begin(), moves.end(), std::make_pair(std::make_pair(bound_val, 0), 0)));
    if(moves.empty()){
        is_back = true;
        return ;
    }

    real_size = 0;

    for(auto move : moves){
        std::pair<int,int> new_pos = std::make_pair(pos.first + dx.at(move.second), pos.second + dy.at(move.second));

        bool is_move = move.first.second & 1;
        bool is_replace = move.first.second & 2;

        int bit_index = 8 + (new_pos.first * 12 + new_pos.second) * 2;
        int bit_count = ((bs >> bit_index) & std::bitset<296>((1LL << 32) - 1)).to_ulong() & 3;

        // ここでbitsetを変更していい感じにする
        bs &= ~std::bitset<296>(255);
        bs |= (( (is_move ? new_pos.first : pos.first) << 4) | (is_move ? new_pos.second : pos.second));

        if(is_replace){
            --state.at(new_pos.first).at(new_pos.second);
            bs &= ~(std::bitset<296>(3) << bit_index);
            bs |= std::bitset<296>(bit_count + 1) << bit_index;
        }

        if(node_map.count(bs)){
            ++node_map[bs]->size;
        }
        else{
            childs[move.second] = std::make_pair(std::make_shared<SearchNode>(move.first.first, depth + 1, remain - 1, (is_move ? new_pos : pos), side, value, state, node_map, bs, predict, predict_weight, movecount), move.first.second);
            // childs[move.second].first->parent = std::make_pair(shared_from_this(), move.second);

            real_size += childs[move.second].first->real_size;

            node_map[bs] = childs[move.second].first;

        }

        // ここでbsを戻す
        bs &= ~std::bitset<296>(255);
        bs |= (( pos.first << 4) | pos.second);

        if(is_replace){
            bs &= ~(std::bitset<296>(3) << bit_index);
            bs |= std::bitset<296>(bit_count) << bit_index;

            ++state.at(new_pos.first).at(new_pos.second);
        }
    }
}


std::pair<std::pair<int,int>, int> DepthFirstSearch::getMaxAdvMove(std::shared_ptr<SearchNode> age1, std::shared_ptr<SearchNode> age2){

    long long rearch = std::min(age1->size, age2->size);


     rearch *= ratio;
    //rearch = 3;
     if(rearch == 0)rearch = 3;

    std::vector<RoutesAndNode> routes1,routes2;


    for(int index = 0;index < rearch;index++){
        RoutesAndNode ins;
        if(!age1->flag)continue;
        ins.CollectIndex(age1, true);
        ins.CollectPos(side, 0, field);
        routes1.push_back(ins);
    }


    for(int index = 0;index < rearch;index++){
        RoutesAndNode ins;
        if(!age2->flag)continue;
        ins.CollectIndex(age2, true);
        ins.CollectPos(side, 1, field);
        routes2.push_back(ins);
    }

    for(auto ch : age1->childs){
        RoutesAndNode ins;
        if(!ch.second.first->flag)continue;
        ins.indexs.push_back(ch.first);
        ins.CollectIndex(ch.second.first, true);
        ins.CollectPos(side, 0, field);
        routes1.push_back(ins);
    }

    for(auto ch :  age2->childs){
        RoutesAndNode ins;
        if(!ch.second.first->flag)continue;
        ins.indexs.push_back(ch.first);
        ins.CollectIndex(ch.second.first, true);
        ins.CollectPos(side, 1, field);
        routes2.push_back(ins);
    }

    auto check = [&](int index1, int index2){
        RoutesAndNode ro1 = routes1.at(index1);
        RoutesAndNode ro2 = routes2.at(index2);

        int count = 0;
        for(int index = 0;index < std::min(ro1.route_pos.size(),ro2.route_pos.size());index++){
            if(ro1.route_pos.at(index) == ro2.route_pos.at(index)){
                return 1.0*(ro1.adv + ro2.adv) / penaRatio.at(index);
            }
        }
        return 0.0;
    };
    std::pair<std::pair<int,int>,long long> ans = std::make_pair(std::make_pair(8,8),-1e18);


    std::list<std::pair<int,int>> moves_1, moves_2;

    for(int a = 0;a < routes1.size();a++){
        for(int b = 0;b < routes2.size();b++){
            double pena = check(a,b);

            if(ans.second < routes1.at(a).adv + routes2.at(b).adv - pena && routes1.at(a).next_pos != routes2.at(b).next_pos){
                moves_1.clear();
                moves_2.clear();
                ans.second = routes1.at(a).adv + routes2.at(b).adv - pena;
                ans.first = std::make_pair(routes1.at(a).indexs.front(), routes2.at(b).indexs.front());

                for(auto pos : routes1.at(a).route_pos){
                    moves_1.push_back(pos);
                }
                for(auto pos : routes2.at(b).route_pos){
                    moves_2.push_back(pos);
                }
            }
        }
    }
    if(vis_show){

        std::vector<std::list<std::pair<int,int>>> use_vec = std::vector<std::list<std::pair<int,int>>>({moves_1, moves_2});

        minimum->setRoute(use_vec);
    }

    return ans;
}

void DepthFirstSearch::RoutesAndNode::CollectIndex(std::shared_ptr<SearchNode> now, bool flag){

    std::shared_ptr<SearchNode> ins;
    int way = 8;
    long long mi = -1e9;
    for(auto ch : now->childs){
        ch.second.first->advsum = SearchNode::advinit;
        int ins_adv = ch.second.first->getAdvSum();
        if(mi <= ins_adv && ch.second.first->flag){
            ins = ch.second.first;
            mi =  ins_adv;
            way = ch.first;
        }
    }
    if(mi != -1e9){
       indexs.push_back(way);
       CollectIndex(ins, false);
       if(flag){
           adv = now->getAdvSum();
       }
    }else{
        if(indexs.empty())indexs.push_back(8);
        now->flag = false;
    }
}


DepthFirstSearch::SearchNode::SearchNode(double adv, int depth, double predict_weight, int movecount) :
    depth(depth),
    size(0),
    real_size(1),
    adv(adv),
    predict_weight(predict_weight),
    movecount(movecount)
{
}

void DepthFirstSearch::SearchNode::dfsAdd(std::pair<int,int> pos, std::vector<std::vector<std::vector<double>>>& vec){


    for(auto ch : childs){
        std::pair<int,int> new_pos(pos);
        new_pos.first += dx.at(ch.first);
        new_pos.second += dy.at(ch.first);

        std::pair<int,int> agent_pos(ch.second.second & 1 ? new_pos : pos);

        ch.second.first->dfsAdd(agent_pos, vec);

        size += ch.second.first->size;

        vec.at(depth).at(new_pos.first).at(new_pos.second) += ch.second.first->size;
    }

}

double DepthFirstSearch::SearchNode::getAdvSum(){
    if(advsum != advinit)
        return advsum;

    double point = 0.0;
    for(auto ch : childs)
        if(ch.second.first->flag) point = std::max(point, ch.second.first->getAdvSum());

    return advsum = point + adv;
}

std::pair<int, int> DepthFirstSearch::SearchNode::getMaxAdvMove(){
    std::pair<int,int> maxmove(advinit, -1);
    for(auto ch : childs)
        if(maxmove.first < ch.second.first->getAdvSum())
            maxmove = std::make_pair(ch.second.first->getAdvSum(), ch.first);

    return maxmove;
}

void DepthFirstSearch::RoutesAndNode::CollectPos(int side, int agent, procon::Field field){
    route_pos.clear();
    std::pair<int,int> pos = field.getAgent(side, agent);
    route_pos.push_back(pos);
    std::vector<std::vector<int>> flag(12, std::vector<int>(12, 0));
    for(int x = 0;x < field.getSize().first; x++){
        for(int y = 0;y < field.getSize().second; y++){
            flag.at(x).at(y) = field.getState(x, y).first;
        }
    }

    auto moveAfter = [&](int move){
        if(flag.at(SearchNode::dx.at(move) + pos.first).at(SearchNode::dy.at(move) + pos.second) != side + 1 && flag.at(SearchNode::dx.at(move) + pos.first).at(SearchNode::dy.at(move) + pos.second) != 0){
            flag.at(SearchNode::dx.at(move) + pos.first).at(SearchNode::dy.at(move) + pos.second) = 0;
            return pos;
        }else{
            return std::make_pair(SearchNode::dx.at(move) + pos.first, SearchNode::dy.at(move) + pos.second);
        }
    };

    if(!indexs.empty()){
        next_pos = std::make_pair(pos.first + SearchNode::dx.at(indexs.front()), pos.second + SearchNode::dy.at(indexs.front()));
    }else{
        next_pos = pos;
    }

    for(int a = 0;a < indexs.size();a++){
        pos = moveAfter(indexs.at(a));
        route_pos.push_back(pos);
    }
}

const std::vector<int> DepthFirstSearch::SearchNode::dx({1, 1, 0, -1, -1, -1, 0, 1, 0});
const std::vector<int> DepthFirstSearch::SearchNode::dy({0, -1, -1, -1, 0, 1, 1, 1, 0});

DepthFirstSearch::Treap::Treap() : root(TreapNode::nil){}

DepthFirstSearch::Treap::Treap(value_type val) : root(std::make_shared<TreapNode>(val)){}

// イテレータが指す[st,en)の範囲で初期化する
DepthFirstSearch::Treap::Treap(std::vector<value_type>::iterator st, std::vector<value_type>::iterator en) : root(TreapNode::nil){
    while(st != en){
        root = _merge(root, std::make_shared<TreapNode>(*st));
        ++st;
    }
}

// 配列で初期化する
DepthFirstSearch::Treap::Treap(std::vector<value_type> v) : root(TreapNode::nil){
    for(auto& xx : v)
        root = _merge(root, std::make_shared<TreapNode>(xx));
}

int DepthFirstSearch::Treap::_size(np x){
    return x == TreapNode::nil ? 0 : x->size;
}

np DepthFirstSearch::Treap::_update(np x){
    x->size = _size(x->l) + _size(x->r) + 1;
    return x;
}

np DepthFirstSearch::Treap::_merge(np l, np r){
    if(l == TreapNode::nil || r == TreapNode::nil)
        return l == TreapNode::nil ? r : l;

    if(l->pri > r->pri){
        l->r = _merge(l->r, r);
        return _update(l);
    }else{
        r->l = _merge(l, r->l);
        return _update(r);
    }
}

std::pair<np,np> DepthFirstSearch::Treap::_split(np x, int k){
    if(x == TreapNode::nil)
        return std::make_pair(TreapNode::nil, TreapNode::nil);

    if(k <= _size(x->l)){
        std::pair<np,np> s = _split(x->l, k);
        x->l = s.second;
        return std::make_pair(s.first, _update(x));

    }else{
        std::pair<np,np> s = _split(x->r, k - _size(x->l) - 1);
        x->r = s.first;
        return std::make_pair(_update(x), s.second);
    }
}

np DepthFirstSearch::Treap::_insert(np x, int k, value_type val){
    np l,r;
    std::tie(l, r) = _split(x, k);
    return _merge(_merge(l, std::make_shared<TreapNode>(val)), r);
}

np DepthFirstSearch::Treap::_erase(np x, int k){
    np l,r,m;
    std::tie(l, r) = _split(x, k);
    std::tie(m, r) = _split(r, 1);
    return _merge(l, r);
}

np DepthFirstSearch::Treap::_erase_back(np x, int k){
    return _split(x, k).first;
}

void DepthFirstSearch::Treap::_set(np x, int k, value_type val){
    if(k < _size(x->l))
        _set(x->l, k, val);
    else if(_size(x->l) == k)
        x->val = val;
    else
        _set(x->r, k - _size(x->l) - 1, val);

    _update(x);
}

value_type DepthFirstSearch::Treap::_get(np x, int k){
    if(k < _size(x->l))
        return _get(x->l, k);
    else if(_size(x->l) == k)
        return x->val;
    else
        return _get(x->r, k - _size(x->l) - 1);
}

int DepthFirstSearch::Treap::_lowerbound(np x, value_type val){
    if(x == TreapNode::nil)
        return 0;
    if(val >= x->val)
        return _lowerbound(x->l, val);
    else
        return _lowerbound(x->r,val) + _size(x->l) + 1;
}

np DepthFirstSearch::Treap::_insert(np x, value_type val){
    return _insert(x, _lowerbound(x, val), val);
}

void DepthFirstSearch::Treap::push_front(value_type val){
    root = _merge(std::make_shared<TreapNode>(val), root);
}

void DepthFirstSearch::Treap::push_back(value_type val){
    root = _merge(root, std::make_shared<TreapNode>(val));
}

void DepthFirstSearch::Treap::pop_front(){
    root = _split(root, 1).second;
}

void DepthFirstSearch::Treap::pop_back(){
    root = _split(root, _size(root) - 1).first;
}

int DepthFirstSearch::Treap::size(){
    return root == TreapNode::nil ? 0 : root->size;
}

void DepthFirstSearch::Treap::set(int k, value_type val){
    return _set(root, k, val);
}

value_type DepthFirstSearch::Treap::get(int k){
    return _get(root, k);
}

void DepthFirstSearch::Treap::insert(int k, value_type val){
    root = _insert(root, k, val);
}

void DepthFirstSearch::Treap::insert(value_type val){
    root = _insert(root, val);
}

int DepthFirstSearch::Treap::lowerbound(value_type val){
    return _lowerbound(root, val);
}

void DepthFirstSearch::Treap::erase(int k){
    root = _erase(root, k);
}

void DepthFirstSearch::Treap::erase_back(int k){
    root = _erase_back(root, k);
}

np DepthFirstSearch::TreapNode::nil = std::make_shared<DepthFirstSearch::TreapNode>(value_type(), 0);
