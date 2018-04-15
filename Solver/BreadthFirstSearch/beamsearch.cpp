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
    int Eva = 0;
    for(int a = 0;a < size.first;a++){
        for(int b =0;b < size.second;b++){

        }
    }
}
