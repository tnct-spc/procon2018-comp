#include "depthfirstsearch.h"

DepthFirstSearch::DepthFirstSearch(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side)
{
    /*
    mt = std::mt19937(rnd());
    random = std::uniform_real_distribution<> (0.0, 1.0);
    */
    if(dock_show){
        dock = std::make_shared<MinimumVisualizerDock>(6);
        dock->show();
    }
}

const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> DepthFirstSearch::agentAct(int now_turn){

    std::cout << "turn : " << now_turn << std::endl;

    maxval = std::min(maxval, final_turn - now_turn);
    predict_per.resize(4, std::vector<std::vector<std::vector<double>>>(maxval, std::vector<std::vector<double>>(field.getSize().first, std::vector<double>(field.getSize().second, 0.0))));

    std::shared_ptr<SearchNode> node_1, node_2;
    std::list<std::pair<int,int>> moves_1, moves_2;
    std::vector<std::vector<std::vector<double>>> states_1, states_2;
    std::vector<std::vector<std::vector<int>>> agent_states_1, agent_states_2;
    std::vector<int> sizes_1, sizes_2;

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

    bool flag = true;
    while(flag){
        flag = false;
        flag |= updatePredictData(side, 0);
        flag |= updatePredictData(side, 1);
        flag |= updatePredictData(side ^ 1, 0);
        flag |= updatePredictData(side ^ 1, 1);
    }

    std::vector<std::vector<std::vector<double>>> pred(maxval, std::vector<std::vector<double>>(field.getSize().first, std::vector<double>(field.getSize().second, 0.0)));
    for(int index = 0; index < 4; ++index)
        if(index != side * 2)
            for(int depth = 0; depth < maxval; ++depth)
                for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
                    for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y)
                        pred.at(depth).at(pos_x).at(pos_y) += (index / 2 == side ? 1 : -1) * predict_per.at(index).at(depth).at(pos_x).at(pos_y);

    std::tie(node_1, moves_1, states_1, sizes_1, agent_states_1) = depthSearch(0, std::min(final_turn - now_turn, maxval), states, pred);

    pred.resize(maxval, std::vector<std::vector<double>>(field.getSize().first, std::vector<double>(field.getSize().second, 0.0)));

    for(int index = 0; index < 4; ++index)
        if(index != side * 2 + 1)
            for(int depth = 0; depth < maxval; ++depth)
                for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
                    for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y)
                        pred.at(depth).at(pos_x).at(pos_y) += (index / 2 == side ? 1 : -1) * predict_per.at(index).at(depth).at(pos_x).at(pos_y);


    std::tie(node_2, moves_2, states_2, sizes_2, agent_states_2) = depthSearch(1, std::min(final_turn - now_turn, maxval), states, pred);

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

    std::cout << "node_1 size : " << node_1->size << " , " << node_1->real_size << std::endl;
    std::cout << "node_2 size : " << node_2->size << " , " << node_2->real_size << std::endl;

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
        dock->addVisualizer(field.getSize(), std::vector<std::list<std::pair<int,int>>>({moves_1, moves_2}), colors);

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

/*
bool DepthFirstSearch::randPer(double bound){
    return bound <= random(mt);
}
*/

std::tuple<std::shared_ptr<DepthFirstSearch::SearchNode>, std::list<std::pair<int,int>>, std::vector<std::vector<std::vector<double>>>, std::vector<int>, std::vector<std::vector<std::vector<int>>>> DepthFirstSearch::depthSearch(int agent, int turn_max, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict){
    int size_x, size_y;
    std::tie(size_x, size_y) = field.getSize();

    const std::vector<std::vector<int>>& field_values = field.getValue();

    std::pair<int,int> agent_pos = field.getAgent(side, agent);
    std::bitset<296> bs((agent_pos.first << 4) + agent_pos.second);

    std::map<std::bitset<296>, std::shared_ptr<SearchNode>, BitSetSorter> node_map;
    std::shared_ptr<SearchNode> node = std::make_shared<SearchNode>(0, 0, turn_max, agent_pos, side, field_values, state, node_map, bs, predict);

    std::list<std::pair<int,int>> moves;
    std::pair<int,int> now_pos = field.getAgent(side, agent);
    moves.emplace_back(now_pos);
    std::shared_ptr<SearchNode> now_node = node;
    bool is_move = true;
    std::pair<int,int> value(0, 0);

    while(1){
        value = now_node->getMaxAdvMove();
        if(value.first == -10007)
            break;

        is_move  = now_node->childs[value.second].second;
        now_node = now_node->childs[value.second].first;

        if(is_move){
            now_pos.first += SearchNode::dx.at(value.second);
            now_pos.second += SearchNode::dy.at(value.second);
        }
        moves.emplace_back(now_pos);
    }

    std::vector<std::vector<std::vector<double>>> values(maxval, std::vector<std::vector<double>>(field.getSize().first, std::vector<double>(field.getSize().second, 0.0)));
    std::vector<std::vector<std::vector<int>>> agent_values(maxval, std::vector<std::vector<int>>(field.getSize().first, std::vector<int>(field.getSize().second, 0)));

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

    node->dfsAdd(field.getAgent(side, agent), values, agent_values);

    for(int dep = 0; dep < maxval; ++dep){
        std::vector<std::vector<double>>& vec = values.at(dep);
        int siz = depth_size.at(dep);
        std::for_each(vec.begin(), vec.end(), [siz](std::vector<double>& v){std::for_each(v.begin(), v.end(), [siz](double& val){if(val && siz)val /= siz;});});
    }

    return std::make_tuple(node, moves, values, depth_size, agent_values);
}

bool DepthFirstSearch::updatePredictData(bool inp_side, bool agent){

    getMovePer(inp_side, agent);
    std::vector<std::vector<std::vector<double>>> ret_val = getMovePer(inp_side, agent);

    /*
    auto rev = [](std::vector<std::vector<std::vector<double>>>& v){
        std::for_each(v.begin(), v.end(), [](std::vector<std::vector<double>>& v2){
            std::for_each(v2.begin(), v2.end(), [](std::vector<double>& v3){
                std::for_each(v3.begin(), v3.end(), [](double& v4){
                    v4 *= -1;
                });
            });
        });
    };
    if(side ^ inp_side)
        rev(ret_val);
    */
    std::vector<std::vector<std::vector<double>>> before_vec = predict_per.at(inp_side * 2 + agent);
    double diff = 0.0;
    for(int dep = 0; dep < maxval; ++dep)
        for(int x_index = 0; x_index < field.getSize().first; ++x_index)
            for(int y_index = 0; y_index < field.getSize().second; ++y_index)
                diff += std::abs(before_vec.at(dep).at(x_index).at(y_index) - ret_val.at(dep).at(x_index).at(y_index));

    predict_per.at(inp_side * 2 + agent) = std::move(ret_val);
    return diff;
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
                        pred.at(depth).at(pos_x).at(pos_y) += (index / 2 == inp_side ? 1 : -1) * predict_per.at(index).at(depth).at(pos_x).at(pos_y);

    std::tie(std::ignore, std::ignore, ret_states, std::ignore, std::ignore) = depthSearch(agent, maxval, states, pred);

    for(int depth = 0; depth < maxval - 1; ++depth)
        for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
            for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y)
                ret_states.at(depth + 1).at(pos_x).at(pos_y) += ret_states.at(depth).at(pos_x).at(pos_y);

    return ret_states;
}

DepthFirstSearch::SearchNode::SearchNode(double adv, int depth, int remain, std::pair<int,int> pos, int side, const std::vector<std::vector<int>>& value, std::vector<std::vector<int>>& state, std::map<std::bitset<296>, std::shared_ptr<SearchNode>, BitSetSorter>& node_map, std::bitset<296>& bs, const std::vector<std::vector<std::vector<double>>>& predict) :
    adv(adv),
    depth(depth),
    size(0),
    real_size(1)
{
    // 末尾ノード
    if(!remain){
        is_back = true;
        return ;
    }

    // {{point, ismove}, dir}
    std::vector<std::pair<std::pair<double, bool>, int>> moves;
    for(int index = 0; index < 8; ++index){

        std::pair<double, bool>ret_pair(-10007.0, false);
        int x_pos = pos.first + dx.at(index);
        int y_pos = pos.second + dy.at(index);

        if(!(x_pos < 0 || y_pos < 0 || x_pos >= static_cast<int>(state.size()) || y_pos >= static_cast<int>(state.at(0).size()) || !state.at(x_pos).at(y_pos))){

            double point = value.at(x_pos).at(y_pos) * (1.0 - predict_weight * predict.at(depth).at(x_pos).at(y_pos));

            ret_pair = std::make_pair(point, state.at(x_pos).at(y_pos) != 2);
        }

        moves.emplace_back(std::make_pair(std::move(ret_pair), index));
    }

    std::sort(moves.begin(), moves.end());

    double bound_val = std::max(moves.at(8 - movecount).first.first, 0.0);

    moves.erase(moves.begin(), std::lower_bound(moves.begin(), moves.end(), std::make_pair(std::make_pair(bound_val, false), 0)));
    if(moves.empty()){
        is_back = true;
        return ;
    }

    real_size = 0;

    for(auto move : moves){
        std::pair<int,int> new_pos = std::make_pair(pos.first + dx.at(move.second), pos.second + dy.at(move.second));

        --state.at(new_pos.first).at(new_pos.second);

        // ここでbitsetを変更していい感じにする
        bs &= ~std::bitset<296>(255);
        bs |= (( (move.first.second ? new_pos.first : pos.first) << 4) | (move.first.second ? new_pos.second : pos.second));

        int bit_index = 8 + (new_pos.first * 12 + new_pos.second) * 2;
        int bit_count = ((bs >> bit_index) & std::bitset<296>((1LL << 32) - 1)).to_ulong() & 3;
        bs &= ~(std::bitset<296>(3) << bit_index);
        bs |= std::bitset<296>(bit_count + 1) << bit_index;

        if(node_map.count(bs)){
            ++node_map[bs]->size;
        }
        else{
            childs[move.second] = std::make_pair(std::make_shared<SearchNode>(move.first.first, depth + 1, remain - 1, (move.first.second ? new_pos : pos), side, value, state, node_map, bs, predict), move.first.second);

            real_size += childs[move.second].first->real_size;

            node_map[bs] = childs[move.second].first;

        }

        // ここでbsを戻す
        bs &= ~std::bitset<296>(255);
        bs |= (( pos.first << 4) | pos.second);
        bs &= ~(std::bitset<296>(3) << bit_index);
        bs |= std::bitset<296>(bit_count) << bit_index;

        ++state.at(new_pos.first).at(new_pos.second);
    }
}

void DepthFirstSearch::SearchNode::dfsAdd(std::pair<int,int> pos, std::vector<std::vector<std::vector<double>>>& vec, std::vector<std::vector<std::vector<int>>>& agent_vec){

    for(auto ch : childs){
        std::pair<int,int> new_pos(pos);
        new_pos.first += dx.at(ch.first);
        new_pos.second += dy.at(ch.first);

        std::pair<int,int> agent_pos(ch.second.second ? new_pos : pos);

        ch.second.first->dfsAdd(agent_pos, vec, agent_vec);

        size += ch.second.first->size;

        agent_vec.at(depth).at(agent_pos.first).at(agent_pos.second) += ch.second.first->size;

        vec.at(depth).at(new_pos.first).at(new_pos.second) += ch.second.first->leaf_size;
    }

}

double DepthFirstSearch::SearchNode::getAdvSum(){
    if(advsum != -10007.0)
        return advsum;

    double point = 0.0;
    for(auto ch : childs)
        point = std::max(point, ch.second.first->getAdvSum());

    return advsum = point + adv;
}

std::pair<int, int> DepthFirstSearch::SearchNode::getMaxAdvMove(){
    std::pair<int,int> maxmove(-10007.0, -1);
    for(auto ch : childs)
        if(maxmove.first < ch.second.first->getAdvSum())
            maxmove = std::make_pair(ch.second.first->getAdvSum(), ch.first);

    return maxmove;
}

const std::vector<int> DepthFirstSearch::SearchNode::dx({1, 1, 0, -1, -1, -1, 0, 1});
const std::vector<int> DepthFirstSearch::SearchNode::dy({0, -1, -1, -1, 0, 1, 1, 1});
