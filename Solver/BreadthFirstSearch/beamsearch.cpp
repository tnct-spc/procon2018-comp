#include "beamsearch.h"

int beamsearch::Evaluation_Field(procon::Field field,int turn){
    std::vector<std::vector<std::pair<int,int>>> Agents = field.getAgents();
    if(turn==0){
        agent1=Agents.at(0).at(0);
        agent2=Agents.at(0).at(1);
    }else{
        agent1=Agents.at(1).at(0);
        agent2=Agents.at(1).at(1);
    }
    std::pair<int,int> size = field.getSize();
    int Eva_manhattan = 0;
    std::vector<std::vector<int>> value = field.getValue();
    for(int a = 0;a < size.first;a++){
        for(int b =0;b < size.second;b++){
            Eva_manhattan += (value.at(a).at(b)+16)/(abs(agent1.first-a)+abs(agent1.second-b));
            Eva_manhattan += (value.at(a).at(b)+16)/(abs(agent2.first-a)+abs(agent2.second-b));
        }
    }
    return Eva_manhattan;
}
const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> beamsearch::agentAct(int side){
    const procon::Field& field = manager->getField();
    std::queue<std::tuple<int,procon::Field,std::tuple<int,int,int>,std::tuple<int,int,int>>> beam;
    for(int a = 0;a < beam_range;a++){

    }
}
