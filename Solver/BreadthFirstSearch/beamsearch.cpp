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
            Eva_manhattan += (value.at(a).at(b)+16)/(std::min(std::abs(agent1.first-a),std::abs(agent1.second-b))+std::max(std::abs(agent1.first-a),std::abs(agent1.second-b))-std::min(std::abs(agent1.first-a),std::abs(agent1.second-b)));
            Eva_manhattan += (value.at(a).at(b)+16)/(abs(agent2.first-a)+abs(agent2.second-b));
        }
    }
    return Eva_manhattan;
}
std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> beamsearch::agentAct(int side){
    const procon::Field& field = manager->getField();
    std::vector<std::tuple<int,procon::Field,std::tuple<int,int,int>,std::tuple<int,int,int>>> beam;
    std::vector<std::pair<int,int>> age1,age2;
    age1.push_back(std::make_pair(1,0));
    age1.push_back(std::make_pair(1,1));
    age1.push_back(std::make_pair(1,-1));
    age1.push_back(std::make_pair(0,0));
    age1.push_back(std::make_pair(0,1));
    age1.push_back(std::make_pair(0,-1));
    age1.push_back(std::make_pair(-1,0));
    age1.push_back(std::make_pair(-1,1));
    age1.push_back(std::make_pair(-1,-1));
    age2 = age1;
    std::pair<int,int> field_size = field.getSize();
    for(int turn = 0;turn < beam_turn;turn++){
        if(turn==0){
            for(int a = 0;a < 9;a++){

                if(age1.at(a).first==-1&&agent1.first==0)continue;
                if(age1.at(a).first==1&&agent1.first==field_size.first-1)continue;
                if(age1.at(a).second==-1&&agent1.second==0)continue;
                if(age1.at(a).second==1&&agent1.second==field_size.second-1)continue;

                for(int b = 0;b < 9;b++){
                    if(age2.at(a).first==-1&&agent2.first==0)continue;
                    if(age2.at(a).first==1&&agent2.first==field_size.first-1)continue;
                    if(age2.at(a).second==-1&&agent2.second==0)continue;
                    if(age2.at(a).second==1&&agent2.second==field_size.second-1)continue;

                }

            }
        }else{

        }
    }
    const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> ans;
    return ans;
}
