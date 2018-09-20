#include "depthfirstsearch.h"

DepthFirstSearch::DepthFirstSearch(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side)
{}

const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> DepthFirstSearch::agentAct(int now_turn){

    minimum.clear();

    std::shared_ptr<SearchNode> node_1 = depthSearch(0, std::min(final_turn - now_turn, maxval));
    std::shared_ptr<SearchNode> node_2 = depthSearch(1, std::min(final_turn - now_turn, maxval));

    /*
    dock = std::make_shared<MinimumVisualizerDock>(2);
    for(int index = 0; index < 2; ++index)
        dock->addVisualizer(minimum.at(index));
    dock->show();
    */

    int move_1 = node_1->getMaxAdvMove().second;
    int move_2 = node_2->getMaxAdvMove().second;

    std::vector<std::pair<int,int>> agent_pos(2);
    for(int index = 0; index < 2; ++index)
        agent_pos.at(index) = field.getAgent(side, index);

    return std::make_pair(std::make_tuple((field.getState(agent_pos.at(0).first + SearchNode::dx.at(move_1), agent_pos.at(0).second + SearchNode::dy.at(move_1)).first != (side ? 1 : 2) ? 1 : 2), SearchNode::dx.at(move_1), SearchNode::dy.at(move_1)),
                          std::make_tuple((field.getState(agent_pos.at(1).first + SearchNode::dx.at(move_2), agent_pos.at(1).second + SearchNode::dy.at(move_2)).first != (side ? 1 : 2) ? 1 : 2), SearchNode::dx.at(move_2), SearchNode::dy.at(move_2)));
}

std::shared_ptr<DepthFirstSearch::SearchNode> DepthFirstSearch::depthSearch(int agent, int turn_max){
    int size_x, size_y;
    std::tie(size_x, size_y) = field.getSize();

    std::vector<std::vector<int>> state(size_x, std::vector<int>(size_y, 1));
    const std::vector<std::vector<int>>& field_values = field.getValue();

    for(int pos_x = 0; pos_x < size_x; ++pos_x)
        for(int pos_y = 0; pos_y < size_y; ++pos_y){
            int pos_state = field.getState(pos_x, pos_y).first;
            if(pos_state)
                state.at(pos_x).at(pos_y) = 2 * (pos_state != side + 1);
        }

    std::shared_ptr<SearchNode> node = std::make_shared<SearchNode>(0, 0, turn_max, field.getAgent(side, agent), side, field_values, state);

    std::list<std::pair<int,int>> moves;
    std::pair<int,int> now_pos = field.getAgent(side, agent);
    moves.emplace_back(now_pos);
    std::shared_ptr<SearchNode> now_node = node;
    bool is_move = true;
    std::pair<int,int> value(0, 0);

    while(1){
        value = now_node->getMaxAdvMove();
        if(value.first == -1000000007)
            break;

        if(is_move){
            now_pos.first += SearchNode::dx.at(value.second);
            now_pos.second += SearchNode::dy.at(value.second);
        }
        moves.emplace_back(now_pos);
        is_move  = now_node->childs[value.second].second;
        now_node = now_node->childs[value.second].first;
    }

    std::vector<std::vector<int>> values(field.getSize().first, std::vector<int>(field.getSize().second, 0));

    node->dfsAdd(field.getAgent(side, agent), values);

    int par_size = node->size;
    for(int x_pos = 0; x_pos < field.getSize().first; ++x_pos)
        for(int y_pos = 0; y_pos < field.getSize().second; ++y_pos)
            par_size = std::max(par_size, values.at(x_pos).at(y_pos));

    std::for_each(values.begin(), values.end(), [par_size](std::vector<int>& v){std::for_each(v.begin(), v.end(), [par_size](int& k){k = k * 255 / par_size;});});

    minimum.emplace_back(std::make_shared<MinimumVisualizer>(field.getSize()));

    minimum.back()->setRoute(moves);
    minimum.back()->setValues(values);
    minimum.back()->show();

    return node;
}

DepthFirstSearch::SearchNode::SearchNode(int adv, int depth, int remain, std::pair<int,int> pos, int side, const std::vector<std::vector<int>>& value, std::vector<std::vector<int>>& state) :
    adv(adv),
    depth(depth)
{
    size = 1;

    // 末尾ノード
    if(!remain)
        return ;

    // {point,is_move}
    auto state_data = [side, pos, value, state](int move_index){
        int x_pos = pos.first + dx.at(move_index);
        int y_pos = pos.second + dy.at(move_index);

        if(x_pos < 0 || y_pos < 0 || x_pos >= state.size() || y_pos >= state.at(0).size() || !state.at(x_pos).at(y_pos))
            return std::make_pair(-1000000007, false);
        return std::make_pair(value.at(x_pos).at(y_pos), state.at(x_pos).at(y_pos) != 2);
    };

    // {{point, ismove}, dir}
    std::vector<std::pair<std::pair<int, bool>, int>> moves;
    for(int index = 0; index < 8; ++index)
        moves.emplace_back(std::make_pair(state_data(index), index));

    std::sort(moves.begin(), moves.end());

    int bound_val = std::max(moves.at(8 - movecount).first.first, 0);

    moves.erase(moves.begin(), std::lower_bound(moves.begin(), moves.end(), std::make_pair(std::make_pair(bound_val, false), 0)));
    if(moves.empty())
        return ;

    size = 0;

    for(auto move : moves){
        std::pair<int,int> new_pos = std::make_pair(pos.first + dx.at(move.second), pos.second + dy.at(move.second));
        --state.at(new_pos.first).at(new_pos.second);

        childs[move.second] = std::make_pair(std::make_shared<SearchNode>(move.first.first, depth + 1, remain - 1, (move.first.second ? new_pos : pos), side, value, state), move.first.second);

        size += childs[move.second].first->size;

        ++state.at(new_pos.first).at(new_pos.second);
    }
}

void DepthFirstSearch::SearchNode::dfsAdd(std::pair<int,int> pos, std::vector<std::vector<int>>& vec){

    vec.at(pos.first).at(pos.second) += size;

    for(auto ch : childs){
        std::pair<int,int> new_pos(pos);
        if(ch.second.second){
            new_pos.first += dx.at(ch.first);
            new_pos.second += dy.at(ch.first);
        }
        ch.second.first->dfsAdd(new_pos, vec);
    }
}

int DepthFirstSearch::SearchNode::getAdvSum(){
    if(advsum != -1000000007)
        return advsum;

    int point = 0;
    for(auto ch : childs)
        point = std::max(point, ch.second.first->getAdvSum());

    return advsum = point + adv;
}

std::pair<int, int> DepthFirstSearch::SearchNode::getMaxAdvMove(){
    std::pair<int,int> maxmove(-1000000007, -1);
    for(auto ch : childs)
        if(maxmove.first < ch.second.first->getAdvSum())
            maxmove = std::make_pair(ch.second.first->getAdvSum(), ch.first);

    return maxmove;
}

const std::vector<int> DepthFirstSearch::SearchNode::dx({1, 1, 0, -1, -1, -1, 0, 1});
const std::vector<int> DepthFirstSearch::SearchNode::dy({0, -1, -1, -1, 0, 1, 1, 1});
