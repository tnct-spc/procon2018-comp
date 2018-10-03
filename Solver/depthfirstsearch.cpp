#include "depthfirstsearch.h"

DepthFirstSearch::DepthFirstSearch(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side)
{
    if(dock_show){
        dock = std::make_shared<ProgresDock>();
        dock->show();
    }
}

const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> DepthFirstSearch::agentAct(int now_turn){

    if(do_output)
        std::cout << "turn : " << now_turn << std::endl;

    maxval = std::min(maxval, final_turn - now_turn);
    predict_per.resize(4, std::vector<std::vector<std::vector<double>>>(maxval, std::vector<std::vector<double>>(field.getSize().first, std::vector<double>(field.getSize().second, 0.0))));

    std::shared_ptr<SearchNode> node_1, node_2;
    std::list<std::pair<int,int>> moves_1, moves_2;
    std::vector<std::vector<std::vector<double>>> states_1, states_2;
    std::vector<std::vector<std::vector<int>>> agent_states_1, agent_states_2;

    std::vector<std::vector<double>> after_values(field.getSize().first, std::vector<double>(field.getSize().second, 1));
    std::vector<std::vector<int>> states(field.getSize().first, std::vector<int>(field.getSize().second, 1));
    for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
        for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y){
            int pos_state = field.getState(pos_x, pos_y).first;
            if(pos_state){
                states.at(pos_x).at(pos_y) = 2 * (pos_state != side + 1);
                after_values.at(pos_x).at(pos_y) = 2 * (pos_state != side + 1);
            }
        }

    for(int cou = 0; cou < loop_count; ++cou){
        std::vector<std::thread> threads;
        for(int index = 0; index < 4; ++index)
            threads.push_back(std::thread([&](int arg){updatePredictData(arg & 2, arg & 1);}, index));

        for(int index = 0; index < 4; ++index)
            threads.at(index).join();
    }

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

    for(int depth = 0; depth < maxval - 1; ++depth)
        for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
            for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y){
                states_1.at(depth + 1).at(pos_x).at(pos_y) += states_1.at(depth).at(pos_x).at(pos_y);
                states_2.at(depth + 1).at(pos_x).at(pos_y) += states_2.at(depth).at(pos_x).at(pos_y);
            }

    int depth_index = std::max(maxval - 1, 0);

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

    if(dock_show)
        dock->addMinumuVisu(field.getSize(), std::vector<std::list<std::pair<int,int>>>({moves_1, moves_2}), colors);

    if(vis_show){
        minimum = std::make_shared<MinimumVisualizer>(field.getSize());

        std::vector<std::list<std::pair<int,int>>> use_vec = std::vector<std::list<std::pair<int,int>>>({moves_1, moves_2});
        minimum->setRoute(use_vec);

        for(int index = 0; index < 3; ++index)
            minimum->setValues(colors.at(index), index);

        minimum->show();
    }

    if(node_1->size == 1 || node_2->size == 1){
        procon::CsvIo::exportField(field, "./error_case.csv");
        std::cerr << "error_case\n";
        return std::make_pair(std::make_tuple(0, 0, 0), std::make_tuple(0, 0, 0));
    }

    int move_1 = node_1->getMaxAdvMove().second;
    int move_2 = node_2->getMaxAdvMove().second;



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


   //std::cout<<agent_pos.at(0).first + SearchNode::dx.at(move_1)<<" "<<agent_pos.at(0).second + SearchNode::dy.at(move_1)<<" "<<agent_pos.at(1).first + SearchNode::dx.at(move_2)<<" "<<agent_pos.at(1).second + SearchNode::dy.at(move_2)<<std::endl;
   // std::cout<<" "<<std::endl;

    return std::make_pair(std::make_tuple((field.getState(agent_pos.at(0).first + SearchNode::dx.at(move_1), agent_pos.at(0).second + SearchNode::dy.at(move_1)).first != (side ? 1 : 2) ? 1 : 2), SearchNode::dx.at(move_1), SearchNode::dy.at(move_1)),
                          std::make_tuple((field.getState(agent_pos.at(1).first + SearchNode::dx.at(move_2), agent_pos.at(1).second + SearchNode::dy.at(move_2)).first != (side ? 1 : 2) ? 1 : 2), SearchNode::dx.at(move_2), SearchNode::dy.at(move_2)));
}

std::shared_ptr<DepthFirstSearch::SearchNode> DepthFirstSearch::createNodeWithDepthSearch(bool inp_side, bool agent, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict){
    const std::vector<std::vector<int>>& field_values = field.getValue();

    std::pair<int,int> agent_pos = field.getAgent(inp_side, agent);
    std::bitset<296> bs((agent_pos.first << 4) + agent_pos.second);

    std::map<std::bitset<296>, std::shared_ptr<SearchNode>, BitSetSorter> node_map;
    return std::make_shared<SearchNode>(0, 0, maxval, agent_pos, inp_side, field_values, state, node_map, bs, predict);

}

std::shared_ptr<DepthFirstSearch::SearchNode> DepthFirstSearch::createNodeWithBeamSearch(bool inp_side, bool agent, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict){

    std::vector<std::shared_ptr<Treap>> treap_vec;
    for(int count = 0; count < 2; ++count)
        treap_vec.emplace_back(std::make_shared<Treap>());

    std::shared_ptr<SearchNode> parent = std::make_shared<SearchNode>(0.0, 0);
    treap_vec.at(0)->insert(std::make_pair(0.0, parent));

    const std::vector<std::vector<int>>& value = field.getValue();

    std::pair<int,int> old_pos = field.getAgent(inp_side, agent);
    for(int dep = 0; dep < maxval + 1; ++dep){

        std::shared_ptr<Treap> before = treap_vec.at(dep % 2);
        std::shared_ptr<Treap> after = treap_vec.at((dep + 1) % 2);

        std::map<std::bitset<296>, std::shared_ptr<SearchNode>, BitSetSorter> node_map;
        while(before->size()){
            std::pair<double, std::shared_ptr<SearchNode>> element = before->get(0);
            double now_adv = element.first;
            std::shared_ptr<SearchNode> node = element.second;
            before->erase(0);

            if(dep == maxval){
                node->is_back = true;
                continue;
            }

            SearchNode* now_node = node.get();
            std::pair<int,int> pos(old_pos);

            std::vector<std::pair<int,int>> rev_move;
            while(now_node->parent.first){
                rev_move.emplace_back(now_node->parent.second, now_node->parent.first->childs[now_node->parent.second].second);
                now_node = now_node->parent.first;
            }

            std::bitset<296> bs;

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

                if(is_move){
                    pos.first += SearchNode::dx.at(move_index);
                    pos.second += SearchNode::dy.at(move_index);
                }
            }

            bs &= ~std::bitset<296>(255);
            bs |= ((pos.first << 4) | pos.second);

            for(int index = 0; index < 8; ++index){
                int x_pos = pos.first + SearchNode::dx.at(index);
                int y_pos = pos.second + SearchNode::dy.at(index);
                if(!(x_pos < 0 || y_pos < 0 || x_pos >= static_cast<int>(state.size()) || y_pos >= static_cast<int>(state.at(0).size()))){

                    bool is_replace = state.at(x_pos).at(y_pos);

                    double point = (is_replace ? value.at(x_pos).at(y_pos) * (1.0 - SearchNode::predict_weight * (dep ? predict.at(dep - 1).at(x_pos).at(y_pos) : 0)) : 0);

                    bool is_move = (state.at(x_pos).at(y_pos) != 2);

                    int bit_index = 8 + ((pos.first + SearchNode::dx.at(index)) * 12 + (pos.second + SearchNode::dy.at(index))) * 2;
                    int bit_count = ((bs >> bit_index) & std::bitset<296>((1LL << 32) - 1)).to_ulong() & 3;

                    if(is_replace){
                        bs &= ~(std::bitset<296>(3) << bit_index);
                        bs |= std::bitset<296>(bit_count + 1) << bit_index;
                    }

                    if(node_map.count(bs)){
                        ++node_map[bs]->size;
                    }else{
                        node->childs[index] = std::make_pair(std::make_shared<SearchNode>(point, dep + 1), static_cast<int>(is_move) | (static_cast<int>(is_replace) << 1));
                        node->childs[index].first->parent = std::make_pair(node.get(), index);
                        node_map[bs] = node->childs[index].first;
                        after->insert(std::make_pair(now_adv + point, node->childs[index].first));
                    }

                    if(is_replace){
                        bs &= ~(std::bitset<296>(3) << bit_index);
                        bs |= std::bitset<296>(bit_count) << bit_index;
                    }
                }
            }

            pos = old_pos;
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

            if(after->size() > beam_width)
                after->erase_back(beam_width);
        }
    }
    treap_vec.clear();

    return parent;
}

std::tuple<std::shared_ptr<DepthFirstSearch::SearchNode>, std::list<std::pair<int,int>>, std::vector<std::vector<std::vector<double>>>> DepthFirstSearch::calcMove(bool inp_side, int agent, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict){

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

        is_move  = now_node->childs[value.second].second & 2;
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

            if(depth)
                depth_size.at(depth - 1) += now_ptr->size + 1;
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

    for(int dep = 0; dep < maxval; ++dep){
        std::vector<std::vector<double>>& vec = values.at(dep);
        int siz = depth_size.at(dep);
        std::for_each(vec.begin(), vec.end(), [siz](std::vector<double>& v){std::for_each(v.begin(), v.end(), [siz](double& val){if(val && siz)val /= siz;});});
    }

    return std::make_tuple(node, moves, values);
}

void DepthFirstSearch::updatePredictData(bool inp_side, bool agent){

    getMovePer(inp_side, agent);
    if(do_output){

        std::vector<std::vector<std::vector<double>>> ret_val = getMovePer(inp_side, agent);

        std::vector<std::vector<std::vector<double>>> before_vec = predict_per.at(inp_side * 2 + agent);
        double diff = 0.0;
        for(int dep = 0; dep < maxval; ++dep)
        for(int x_index = 0; x_index < field.getSize().first; ++x_index)
            for(int y_index = 0; y_index < field.getSize().second; ++y_index)
            diff += std::abs(before_vec.at(dep).at(x_index).at(y_index) - ret_val.at(dep).at(x_index).at(y_index));

        predict_per.at(inp_side * 2 + agent) = std::move(ret_val);

        if(do_output)
            std::cout << "diff : " << diff << std::endl;
    }
}

std::vector<std::vector<std::vector<double>>> DepthFirstSearch::getMovePer(bool inp_side, bool agent){

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

    std::tie(std::ignore, std::ignore, ret_states) = calcMove(inp_side, agent, states, pred);

    for(int depth = 0; depth < maxval - 1; ++depth)
        for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
            for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y)
                ret_states.at(depth + 1).at(pos_x).at(pos_y) += ret_states.at(depth).at(pos_x).at(pos_y);

    return ret_states;
}

DepthFirstSearch::SearchNode::SearchNode(double adv, int depth, int remain, std::pair<int,int> pos, int side, const std::vector<std::vector<int>>& value, std::vector<std::vector<int>>& state, std::map<std::bitset<296>, std::shared_ptr<SearchNode>, BitSetSorter>& node_map, std::bitset<296>& bs, const std::vector<std::vector<std::vector<double>>>& predict) :

    depth(depth),
    size(0),
    real_size(1),
    adv(adv)
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
            childs[move.second] = std::make_pair(std::make_shared<SearchNode>(move.first.first, depth + 1, remain - 1, (is_move ? new_pos : pos), side, value, state, node_map, bs, predict), move.first.second);
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

//    std::cout<<"YES"<<std::endl;

     rearch *= ratio;
    //std::cout<<rearch<<std::endl;
    //rearch = 3;
     if(rearch == 0)rearch = 3;

    std::vector<RoutesAndNode> routes1,routes2;


    for(int index = 0;index < rearch;index++){
        RoutesAndNode ins;
        if(!age1->flag)continue;
        ins.CollectIndex(age1);
        ins.CollectPos(side, 0, field);
        routes1.push_back(ins);
    }

   // std::cout<<routes1.size()<<" "<<routes2.size()<<std::endl;

    for(int index = 0;index < rearch;index++){
        RoutesAndNode ins;
        if(!age2->flag)continue;
        ins.CollectIndex(age2);
        ins.CollectPos(side, 1, field);
        routes2.push_back(ins);
    }

    for(auto ch : age1->childs){
        RoutesAndNode ins;
        if(!ch.second.first->flag)continue;
        ins.indexs.push_back(ch.first);
        ins.CollectIndex(ch.second.first);
        ins.CollectPos(side, 0, field);
        routes1.push_back(ins);
    }
    // std::cout<<routes1.size()<<std::endl;

    for(auto ch :  age2->childs){  
        RoutesAndNode ins;
        if(!ch.second.first->flag)continue;
        ins.indexs.push_back(ch.first);
        ins.CollectIndex(ch.second.first);
        ins.CollectPos(side, 1, field);
        routes2.push_back(ins);
    }


    //std::cout<<routes1.size()<<" "<<routes2.size()<<std::endl;

 //   std::cout<<"HOGHOGE"<<std::endl;
    std::vector<long long> penaRatio = {1,2,4,8,16,32,64,128,256,512,1024,2048,4096};
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

//    std::cout<<"HOGE"<<std::endl;

    //std::cout<<age1->childs.size()<<" "<<age2->childs.size()<<std::endl;



    //std::cout<<routes2.front().indexs.size()<<std::endl;
    for(int a = 0;a < routes1.size();a++){
        for(int b = 0;b < routes2.size();b++){
            double pena = check(a,b);
            if(ans.second < routes1.at(a).adv + routes2.at(b).adv - pena && routes1.at(a).next_pos != routes2.at(b).next_pos){
                ans.second = routes1.at(a).adv + routes2.at(b).adv - pena;
                ans.first = std::make_pair(routes1.at(a).indexs.front(), routes2.at(b).indexs.front());
            }
        }
    }
    // std::cout<<"NNN"<<std::endl;
    //  std::cout<<field.getTurnCount()<<"ターン目: "<<ans.first.first<<" "<<ans.first.second<<std::endl;

    return ans;
}

void DepthFirstSearch::RoutesAndNode::CollectIndex(std::shared_ptr<SearchNode> now){

    std::shared_ptr<SearchNode> ins;
    int way = 8;
    long long mi = -1e9;
    for(auto ch : now->childs){
        if(mi <= ch.second.first->getAdvSum() && ch.second.first->flag){
            ins = ch.second.first;
            mi =  ch.second.first->getAdvSum();
            way = ch.first;
        }
    }
    if(mi != -1e9){
      //  std::cout<<way<<std::endl;
       indexs.push_back(way);
       adv = mi;
       CollectIndex(ins);
       ins->advsum = SearchNode::advinit;
    }else{
        if(indexs.empty())indexs.push_back(8);
        now->flag = false;
    }
}


DepthFirstSearch::SearchNode::SearchNode(double adv, int depth) :
    depth(depth),
    size(0),
    real_size(1),
    adv(adv)
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

        vec.at(depth).at(new_pos.first).at(new_pos.second) += ch.second.first->leaf_size;
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



const std::vector<int> DepthFirstSearch::SearchNode::dx({1, 1, 0, -1, -1, -1, 0, 1, 0});
const std::vector<int> DepthFirstSearch::SearchNode::dy({0, -1, -1, -1, 0, 1, 1, 1, 0});

void DepthFirstSearch::RoutesAndNode::CollectPos(int side, int agent, procon::Field field){
  //  for(int a = 0;a < indexs.size();a++)std::cout<<" "<<indexs.at(a);
  //  std::cout<<std::endl;
    std::pair<int,int> pos = field.getAgent(side, agent);
    route_pos.push_back(pos);
    std::vector<std::vector<int>> flag(12, std::vector<int>(12, 0));
    for(int x = 0;x < field.getSize().first; x++){
        for(int y = 0;y < field.getSize().second; y++){
            flag.at(x).at(y) = field.getState(x, y).first;
        }
    }

    auto moveAfter = [&](int move){
       // std::cout<<SearchNode::dx.at(move) + pos.first<<" "<<SearchNode::dy.at(move) + pos.second<<std::endl;
        if(flag.at(SearchNode::dx.at(move) + pos.first).at(SearchNode::dy.at(move) + pos.second) != side + 1 && flag.at(SearchNode::dx.at(move) + pos.first).at(SearchNode::dy.at(move) + pos.second) != 0){
            flag.at(SearchNode::dx.at(move) + pos.first).at(SearchNode::dy.at(move) + pos.second) = 0;
            return pos;
        }else{
            return std::make_pair(SearchNode::dx.at(move) + pos.first, SearchNode::dy.at(move) + pos.second);
        }
    };

    if(!indexs.empty())next_pos = std::make_pair(pos.first + SearchNode::dx.at(indexs.front()), pos.second + SearchNode::dy.at(indexs.front()));
    else next_pos = pos;

    for(int a = 0;a < indexs.size();a++){
        pos = moveAfter(indexs.at(a));
        route_pos.push_back(pos);
    }
}
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
