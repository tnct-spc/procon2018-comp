#include "montecarlotreesearch.h"

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> MonteCarloTreeSearch::agentAct(int side){

    std::vector<std::pair<int,int>> agents { manager->getField().getAgent(side, 0) , manager->getField().getAgent(side, 1) };

    SearchNode root_node(this, nullptr, agents, manager->getField().getSize().first, manager->getField().getSize().second);

}
