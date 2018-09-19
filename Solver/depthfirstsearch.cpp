#include "depthfirstsearch.h"

const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> DepthFirstSearch::agentAct(int now_turn){

    std::shared_ptr<SearchNode> node_1 = depthSearch(0, std::min(final_turn - now_turn, maxval));
    std::shared_ptr<SearchNode> node_2 = depthSearch(1, std::min(final_turn - now_turn, maxval));
    return std::make_pair(std::make_tuple(0,0,0), std::make_tuple(0,0,0));
}

std::shared_ptr<DepthFirstSearch::SearchNode> DepthFirstSearch::depthSearch(int agent, int turn_max){
    std::unordered_map<std::pair<int,int>, int, pairHash, pairEqual> used;
    return std::make_shared<SearchNode>(0, turn_max, field.getAgent(side, agent), field, used);
}

DepthFirstSearch::SearchNode::SearchNode(int depth, int remain, std::pair<int,int> pos, const procon::Field& field, std::unordered_map<std::pair<int,int>, int, pairHash, pairEqual>& used) :
    depth(depth)
{
    // 末尾ノード
    if(!remain)
        return ;

}


size_t DepthFirstSearch::pairHash::operator()(const std::pair<int,int>& key) const{
    return (key.first << 5) + key.second;
}

size_t DepthFirstSearch::moveHash::operator()(const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>& key) const{
    return (std::get<0>(key.first)  + 1)      + (std::get<1>(key.first)  + 1) * 3  + (std::get<2>(key.first)  + 1) * 9
         + (std::get<0>(key.second) + 1) * 27 + (std::get<1>(key.second) + 1) * 81 + (std::get<2>(key.second) + 1) * 243;
}
