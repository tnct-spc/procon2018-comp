#include "useabstractdata.h"

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> UseAbstractData::agentAct(int now_turn){

    return std::make_pair(std::make_tuple(0,0,0), std::make_tuple(0,0,0));
}

std::vector<std::vector<double>> UseAbstractData::getAbstractBasedAgent(bool side, bool agent){

    std::vector<int> agent_pos = {field.getAgent(side, agent).first, field.getAgent(side, agent).second};
    std::vector<int> search_pos(2, 0);

    auto calc_distance = [&](std::vector<int> pos){
        return 1.0 * std::max(std::abs(agent_pos.at(0) - pos.at(0)), std::abs(agent_pos.at(1) - pos.at(1)))
            +  0.5 * std::min(std::abs(agent_pos.at(0) - pos.at(0)), std::abs(agent_pos.at(1) - pos.at(1)));
    };

    for(;search_pos.at(0) < field.getSize().first; ++search_pos.at(0))
        for(;search_pos.at(1) < field.getSize().second; ++search_pos.at(1)){


        }

}
