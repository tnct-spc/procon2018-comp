#include "beamsearch.h"

using namespace std;
int beamsearch::Evaluation_Field(procon::Field field){
    /*
    std::pair<int,int> size = field.getSize();
    int Eva_manhattan = 0;
    std::vector<std::vector<int>> value = field.getValue();
    for(int a = 0;a < size.first;a++){
        for(int b =0;b < size.second;b++){
            if((std::min(std::abs(agent1.first-a),std::abs(agent1.second-b))+std::max(std::abs(agent1.first-a),std::abs(agent1.second-b))-std::min(std::abs(agent1.first-a),std::abs(agent1.second-b)))!=0){
            Eva_manhattan += (value.at(a).at(b)+16)/(std::min(std::abs(agent1.first-a),std::abs(agent1.second-b))+std::max(std::abs(agent1.first-a),std::abs(agent1.second-b))-std::min(std::abs(agent1.first-a),std::abs(agent1.second-b)));
            }
            if((std::min(std::abs(agent2.first-a),std::abs(agent2.second-b))+std::max(std::abs(agent2.first-a),std::abs(agent2.second-b))-std::min(std::abs(agent2.first-a),std::abs(agent2.second-b)))){
            Eva_manhattan += (value.at(a).at(b)+16)/(std::min(std::abs(agent2.first-a),std::abs(agent2.second-b))+std::max(std::abs(agent2.first-a),std::abs(agent2.second-b))-std::min(std::abs(agent2.first-a),std::abs(agent2.second-b)));
            }
        }
    }
    */
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<> rand1000(0, 999);
    //cout<<Eva_manhattan<<endl;
    return rand1000(mt);
}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> beamsearch::agentAct(int side){


    const procon::Field& field = manager->getField();

    auto sortEva = [&](std::pair<int,std::tuple<procon::Field,std::tuple<int,int,int>,std::tuple<int,int,int>>> const& a,std::pair<int,std::tuple<procon::Field,std::tuple<int,int,int>,std::tuple<int,int,int>>> const& b)->bool
    {
      return a.first < b.first;
    };
  //  cout<<"unya"<<endl;
    std::vector<std::vector<std::pair<int,int>>> Agents = field.getAgents();

        agent1=Agents.at(side).at(0);
        agent2=Agents.at(side).at(1);

    cout<<"("<<agent1.first<<","<<agent1.second<<")"<<"("<<agent2.first<<","<<agent2.second<<")"<<endl;

    vector<vector<int>> vec;

    vector<pair<int,int>> age1,age2;

    age1.push_back(make_pair(0,1));
    age1.push_back(make_pair(0,-1));
    age1.push_back(make_pair(1,0));
    age1.push_back(make_pair(1,1));
    age1.push_back(make_pair(1,-1));
    age1.push_back(make_pair(-1,1));
    age1.push_back(make_pair(-1,0));
    age1.push_back(make_pair(-1,-1));

    age2 = age1;


    return std::make_pair(std::get<1>(beam.front().second),std::get<2>(beam.front().second));
}
procon::Field beamsearch::agentmove(procon::Field field, std::vector<std::vector<std::pair<int,std::pair<int,int>>>> pos)
{

    std::map<std::pair<int,int>,std::vector<std::pair<int,int>>> dest_map;
    std::map<std::pair<int,int>,std::vector<std::pair<int,int>>> tile_map;

    int type, pos_x, pos_y;

    for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
        for(int agent_num = 0; agent_num < 2; ++agent_num){

            std::tie(type, pos_x, pos_y) = std::make_tuple(pos.at(turn_flag).at(agent_num).first,pos.at(turn_flag).at(agent_num).second.first,pos.at(turn_flag).at(agent_num).second.second);
            std::pair<int,int> pos = std::make_pair(pos_x, pos_y);

            if(type == 1){
                dest_map[ pos ].push_back( std::make_pair(turn_flag, agent_num) );
            }else
            if(type == 2){
                tile_map[ pos ].push_back( std::make_pair(turn_flag, agent_num) );
                dest_map[ field.getAgent(turn_flag, agent_num)].push_back(std::make_pair(turn_flag, agent_num));
            }
        }

    for(auto elements : dest_map){

        if(elements.second.size() > 1)
            continue;

        if(field.getState(elements.first.first, elements.first.second).first == (elements.second.at(0).first == 0 ? 2 : 1))
            continue;

        if(field.getAgent(elements.second.at(0).first,(elements.second.at(0).second ? 0 : 1 )) == elements.first)
            continue;

        field.setAgent(elements.second.at(0).first, elements.second.at(0).second, elements.first.first, elements.first.second);
        field.setState(elements.first.first, elements.first.second, elements.second.at(0).first + 1);
    }

    for(auto elements : tile_map){
        bool state_flag = true;
        if(elements.second.size() > 1)
            continue;


        for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
            for(int agent_num = 0; agent_num < 2; ++agent_num)
                if(field.getAgent(turn_flag, agent_num) == elements.first){
                    state_flag = false;
                    break;
                }
    }
        return field;
}


