#include "depthfirstsearch.h"

const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> DepthFirstSearch::agentAct(int now_turn){

    std::shared_ptr<SearchNode> node_1 = depthSearch(0, std::min(final_turn - now_turn, maxval));
    std::shared_ptr<SearchNode> node_2 = depthSearch(1, std::min(final_turn - now_turn, maxval));
    return std::make_pair(std::make_tuple(0,0,0), std::make_tuple(0,0,0));
}

std::shared_ptr<DepthFirstSearch::SearchNode> DepthFirstSearch::depthSearch(int agent, int turn_max){
    std::unordered_map<std::pair<int,int>, int, pairHash, pairEqual> used;
    return std::make_shared<SearchNode>(0, 0, turn_max, field.getAgent(side, agent), side, field, used);
}

DepthFirstSearch::SearchNode::SearchNode(int adv, int depth, int remain, std::pair<int,int> pos, int side, const procon::Field& field, std::unordered_map<std::pair<int,int>, int, pairHash, pairEqual>& used) :
    adv(adv),
    depth(depth)
{
    // 末尾ノード
    if(!remain)
        return ;

    // 移動先の得点を返す(枯れている場合は適当に小さい値を返す)
    auto state_data = [side, pos, field, &used](int move_index){
        int x_pos = pos.first + dx.at(move_index);
        int y_pos = pos.second + dy.at(move_index);

        if(x_pos < 0 || y_pos < 0 || x_pos >= field.getSize().first || y_pos >= field.getSize().second)
            return -1000000007;
        std::pair<int,int> state = field.getState(x_pos, y_pos);
        int count = (state.first == side + 1 ? 0 : (2 - !(state.first))) - used[std::make_pair(x_pos, y_pos)];
        return (count <= 0 ? 0 : state.second);
    };

    std::vector<std::pair<int,int>> moves;
    for(int index = 0; index < 8; ++index)
        moves.emplace_back(state_data(index), index);

    std::sort(moves.begin(), moves.end());

    int bound_val = std::max(moves.at(8 - movecount).first, 0);

    moves.erase(moves.begin(), std::lower_bound(moves.begin(), moves.end(), std::make_pair(bound_val, 0)));
    if(moves.empty())
        return ;

    for(auto move : moves){
        std::pair<int,int> new_pos = std::make_pair(pos.first + dx.at(move.second), pos.second + dy.at(move.second));
        ++used[new_pos];
        childs[move.second] = std::make_shared<SearchNode>(move.first, depth + 1, remain - 1, new_pos, side, field, used);
        --used[new_pos];
    }
}

int DepthFirstSearch::SearchNode::calcAdv(){
    int point = 0;
    for(auto ch : childs)
        point = std::max(point, ch.second->calcAdv());
    return point + adv;
}


size_t DepthFirstSearch::pairHash::operator()(const std::pair<int,int>& key) const{
    return (key.first << 5) + key.second;
}


const std::vector<int> DepthFirstSearch::SearchNode::dx({1, 1, 0, -1, -1, -1, 0, 1});
const std::vector<int> DepthFirstSearch::SearchNode::dy({0, -1, -1, -1, 0, 1, 1, 1});
