#include "montecarlotreesearch.h"

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> MonteCarloTreeSearch::agentAct(int side){

    std::vector<std::pair<int,int>> agents { manager->getField().getAgent(side, 0) , manager->getField().getAgent(side, 1) };

    SearchNode root_node(nullptr, side, 100000);//try_countには閾値より大きな値を渡す

    std::vector<std::vector<int>> field_data = manager->getField().getField();
    std::vector<std::vector<std::pair<int,int>>> agent_data = manager->getField().getAgents();


    GameSimulator sim(manager->getField());

    for(int count = 0; count < trial; ++count){

        root_node.all_pat_try_count = count;

        sim.setFieldData(field_data, agent_data);
        root_node.trySimulate(&sim);


    }

}
