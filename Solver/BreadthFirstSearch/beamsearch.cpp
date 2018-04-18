#include "beamsearch.h"

int beamsearch::Evaluation_Field(procon::Field field){
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

    std::vector<std::vector<std::pair<int,int>>> Agents = field.getAgents();

    if(side==0){
        agent1=Agents.at(0).at(0);
        agent2=Agents.at(0).at(1);
    }else{
        agent1=Agents.at(1).at(0);
        agent2=Agents.at(1).at(1);
    }
    std::vector<std::pair<int,std::tuple<procon::Field,std::tuple<int,int,int>,std::tuple<int,int,int>>>> beam;
    std::vector<std::pair<int,int>> age1,age2;
    age1.push_back(std::make_pair(1,0));
    age1.push_back(std::make_pair(1,1));
    age1.push_back(std::make_pair(1,-1));
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
                    if(age2.at(b).first==-1&&agent2.first==0)continue;
                    if(age2.at(b).first==1&&agent2.first==field_size.first-1)continue;
                    if(age2.at(b).second==-1&&agent2.second==0)continue;
                    if(age2.at(b).second==1&&agent2.second==field_size.second-1)continue;
                    procon::Field ins_field = field;
                    ins_field.setAgent(side,0,agent1.first+age1.at(a).first,agent1.second+age1.at(a).second);
                    ins_field.setState(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second,side+1);
                    ins_field.setAgent(side,1,agent2.first+age2.at(b).first,agent2.second+age2.at(b).second);
                    ins_field.setState(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second,side+1);
                    int Value = Evaluation_Field(ins_field);
                    int state1 = field.getState(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second).first;
                    int state2 = field.getState(agent2.first+age2.at(a).first,agent2.second+age2.at(a).second).first;
                    beam.push_back(std::make_pair(Value,std::make_tuple(ins_field,std::make_tuple((state1!=side+1&&state1!=0?2:1),age1.at(a).first,age1.at(a).second),std::make_tuple((state2!=side+1&&state2!=0?2:1),age2.at(b).first,age2.at(b).second))));
                }

            }
        }else{

        }
    }
    const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> ans;
    return ans;
}
