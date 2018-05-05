#include "beamsearch.h"
typedef std::pair<double,std::tuple<procon::Field,int,int>> origin;

beamsearch::beamsearch(std::shared_ptr<GameManager> manager_ptr, const GeneticAgent& agent) :
    AlgorithmWrapper(manager_ptr),
    agent_data(agent)
{

}

double beamsearch::Evaluation_Field(procon::Field field,int side){

    std::vector<double> agent_value = agent_data.getData();

    agent_value.at(0) = var1;
    agent_value.at(1) = var2;
    agent_value.at(2) = var3;
    agent_value.at(3) = var4;
    agent_value.at(4) = var5;

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
        }s
    }
    */
    int red = 0,blue = 0;
        for(int a = 0;a < field.getSize().first;a++){
            for(int b = 0;b < field.getSize().second;b++){
                if(field.getState(a,b).first==1){
                    red+=field.getState(a,b).second;
                }
                if(field.getState(a,b).first==2){
                    blue+=field.getState(a,b).second;
                }
            }
       }
    double Eva1,Eva2,Eva3,Eva4,Eva5;
    Eva1 = (!side?red-blue:blue-red);
 // most_Eva_value = std::max(Eva1,most_Eva_value);
    Eva1 = (1.0000*Eva1+100)/192;     //マスのポイント
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<> rand1000(1, 1000);
    Eva2 = 1.0000 * rand1000(mt)/1000; //乱択要素

    std::pair<int,int> agent_1=field.getAgent(side,0),agent_2=field.getAgent(side,1);

    Eva3 = 1.0000 * abs(agent_1.first-agent_2.first)+abs(agent_1.second-agent_2.second) / (field.getSize().first+field.getSize().second);//味方のagentとのマンハッタン距離

    int minus_point = 0;

    for(int a = 0;a < field.getSize().first;a++){
        for(int b = 0;b < field.getSize().second;b++){
            if(field.getState(a,b).first==side+1&&field.getState(a,b).second < 0)minus_point+=field.getState(a,b).second;
        }
    }

    Eva4 = 1.0000 * (100 - minus_point)/100;  //マイナスのポイントをいかに踏んでいないか

    std::pair<int,int> enemy_1=field.getAgent((side?0:1),0),enemy_2 = field.getAgent((side?0:1),1);

    Eva5 = 1.0000 *(abs(agent_1.first-enemy_1.first)+abs(agent_1.second-enemy_1.second)+abs(agent_1.first-enemy_2.first)+abs(agent_1.second-enemy_2.second)+abs(agent_2.first-enemy_1.first)+abs(agent_2.second-enemy_1.second)+abs(agent_2.first-enemy_2.first)+abs(agent_2.second-enemy_2.second))/60;//敵のagentとのきょり
    return var1*Eva1+var2*Eva2+var3*Eva3+var4*Eva4+var5*Eva5;
}



const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> beamsearch::agentAct(int side){

    int count = 0;

    const procon::Field& field = manager->getField();

    std::vector<std::vector<std::pair<int,int>>> Agents = field.getAgents();

        agent1=Agents.at(side).at(0);
        agent2=Agents.at(side).at(1);

    std::cout<<"("<<agent1.first<<","<<agent1.second<<")"<<"("<<agent2.first<<","<<agent2.second<<")"<<std::endl;

    std::pair<int,int> grid_size = field.getSize();

    std::vector<std::pair<int,int>> age1,age2;

    age1.push_back(std::make_pair(0,1));
    age1.push_back(std::make_pair(0,-1));
    age1.push_back(std::make_pair(1,0));
    age1.push_back(std::make_pair(1,1));
    age1.push_back(std::make_pair(1,-1));
    age1.push_back(std::make_pair(-1,1));
    age1.push_back(std::make_pair(-1,0));
    age1.push_back(std::make_pair(-1,-1));

    age2 = age1;
    std::vector<std::vector<std::pair<double,int>>> Eva_stack(8 ,std::vector<std::pair<double,int>>(8 , std::make_pair(0,0))); //age1,age2の組み合わせの得点/回数
    std::vector<std::vector<std::pair<int,int>>> ways(8,std::vector<std::pair<int,int>>(8 , std::make_pair(0,0)));
    std::priority_queue<origin,std::vector<origin>,
            std::function<bool(origin,origin)>>
            beam([](origin a,origin b) -> bool {if(a.first<b.first){return true;}else{return false;}});
    int RemainingTurn = manager->getFinalTurn()-manager->getTurnCount();
    beam_turn = std::min(beam_turn,RemainingTurn);
    for(int turn = 0;turn < beam_turn;turn++){
        if(turn == 0){

            for(int a = 0;a < 8;a++){

                if(age1.at(a).first+agent1.first < 0)continue;
                if(age1.at(a).first+agent1.first >= grid_size.first)continue;
                if(age1.at(a).second+agent1.second < 0)continue;
                if(age1.at(a).second+agent1.second >= grid_size.second)continue;

                for(int b = 0;b < 8;b++){
                    if(age2.at(b).first+agent2.first < 0)continue;
                    if(age2.at(b).first+agent2.first >= grid_size.first)continue;
                    if(age2.at(b).second+agent2.second < 0)continue;
                    if(age2.at(b).second+agent2.second >= grid_size.second)continue;

                    std::vector<std::vector<std::pair<int,std::pair<int,int>>>> pos;

                    std::vector<std::pair<int,std::pair<int,int>>> ins_pos;

                    int way1=1,way2=1;

                   if(side==0){
                    way1 = (field.getState(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second).first == ( side ? 1 : 2 ) ? 2 : 1 );

                    way2 = (field.getState(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second).first == ( side ? 1 : 2 ) ? 2 : 1 );

                    ins_pos.push_back(std::make_pair(way1,std::make_pair(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second)));
                    ins_pos.push_back(std::make_pair(way2,std::make_pair(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second)));

                    pos.push_back(ins_pos);

                    ins_pos.clear();
                    ins_pos.push_back(std::make_pair(0,std::make_pair(agent1.first,agent1.second)));
                    ins_pos.push_back(std::make_pair(0,std::make_pair(agent2.first,agent2.second)));
                    pos.push_back(ins_pos);

                   }else{

                       ins_pos.push_back(std::make_pair(0,std::make_pair(agent1.first,agent1.second)));
                       ins_pos.push_back(std::make_pair(0,std::make_pair(agent2.first,agent2.second)));
                       pos.push_back(ins_pos);
                       ins_pos.clear();

                       way1 = (field.getState(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second).first == ( side ? 1 : 2 ) ? 2 : 1 );

                       way2 = (field.getState(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second).first == ( side ? 1 : 2 ) ? 2 : 1 );

                       ins_pos.push_back(std::make_pair(way1,std::make_pair(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second)));
                       ins_pos.push_back(std::make_pair(way2,std::make_pair(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second)));

                       pos.push_back(ins_pos);
                   }

                    procon::Field ins_field = agentmove(field,pos);

                    double Eva = Evaluation_Field(ins_field,side);

                    ways.at(a).at(b).first=way1;
                    ways.at(a).at(b).second=way2;

                    beam.push(std::make_pair(Eva,std::make_tuple(ins_field,a,b)));
                    Eva_stack.at(a).at(b).first+=Eva;
                    Eva_stack.at(a).at(b).second++;
                    count++;
                }
            }
        }else{
            int beam_size = beam.size();
            std::priority_queue<origin,std::vector<origin>,
                   std::function<bool(origin,origin)>>
                    beam_ins([](origin a,origin b) -> bool{return a.first < b.first;});
         //   cout<<beam_size<<endl;
            for(int range = 0;range < std::min(beam_range,beam_size);range++){
            procon::Field Neo_ins_field = std::get<0>(beam.top().second);
            agent1 = Neo_ins_field.getAgent(side,0);
            agent2 = Neo_ins_field.getAgent(side,1);
         //   std::cout<<beam.top().first<<std::endl;
            for(int a = 0;a < 8;a++){

                if(age1.at(a).first+agent1.first < 0)continue;
                if(age1.at(a).first+agent1.first >= grid_size.first)continue;
                if(age1.at(a).second+agent1.second < 0)continue;
                if(age1.at(a).second+agent1.second >= grid_size.second)continue;

                for(int b = 0;b < 8;b++){
                    if(age2.at(b).first+agent2.first < 0)continue;
                    if(age2.at(b).first+agent2.first >= grid_size.first)continue;
                    if(age2.at(b).second+agent2.second < 0)continue;
                    if(age2.at(b).second+agent2.second >= grid_size.second)continue;

                    std::vector<std::vector<std::pair<int,std::pair<int,int>>>> pos;

                    std::vector<std::pair<int,std::pair<int,int>>> ins_pos;

                    int way1=1,way2=1;

                   if(side==0){
                    way1 = (Neo_ins_field.getState(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second).first == ( side ? 1 : 2 ) ? 2 : 1 );

                    way2 = (Neo_ins_field.getState(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second).first == ( side ? 1 : 2 ) ? 2 : 1 );

                    ins_pos.push_back(std::make_pair(way1,std::make_pair(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second)));
                    ins_pos.push_back(std::make_pair(way2,std::make_pair(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second)));

                    pos.push_back(ins_pos);

                    ins_pos.clear();
                    ins_pos.push_back(std::make_pair(0,std::make_pair(agent1.first,agent1.second)));
                    ins_pos.push_back(std::make_pair(0,std::make_pair(agent2.first,agent2.second)));
                    pos.push_back(ins_pos);

                   }else{

                       ins_pos.push_back(std::make_pair(0,std::make_pair(agent1.first,agent1.second)));
                       ins_pos.push_back(std::make_pair(0,std::make_pair(agent2.first,agent2.second)));
                       pos.push_back(ins_pos);
                       ins_pos.clear();

                       way1 = (field.getState(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second).first == ( side ? 1 : 2 ) ? 2 : 1 );

                       way2 = (field.getState(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second).first == ( side ? 1 : 2 ) ? 2 : 1 );

                       ins_pos.push_back(std::make_pair(way1,std::make_pair(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second)));
                       ins_pos.push_back(std::make_pair(way2,std::make_pair(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second)));

                       pos.push_back(ins_pos);
                   }
                    procon::Field ins_field = agentmove(Neo_ins_field,pos);
                    double Eva = Evaluation_Field(ins_field,side);
                    std::tuple<procon::Field,int,int> ins_value = beam.top().second;

                    beam_ins.push(std::make_pair(Eva,std::make_tuple(ins_field,std::get<1>(ins_value),std::get<2>(ins_value))));
                    Eva_stack.at(std::get<1>(ins_value)).at(std::get<2>(ins_value)).first+=Eva;
                    Eva_stack.at(std::get<1>(ins_value)).at(std::get<2>(ins_value)).second++;
                    count++;
                    }
                }
            beam.pop();
            }
            beam = beam_ins;
        }
    }


    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> ans;
    double most_Eva = -1000000;
    for(int a = 0;a < 8;a++){
        for(int b = 0;b < 8;b++){
            if(Eva_stack.at(a).at(b).second==0)continue;
            std::cout<<Eva_stack.at(a).at(b).first<<std::endl;
            if(most_Eva<=1.0000*Eva_stack.at(a).at(b).first/Eva_stack.at(a).at(b).second){
                most_Eva=Eva_stack.at(a).at(b).first/Eva_stack.at(a).at(b).second;
                ans= std::make_pair(std::make_tuple(ways.at(a).at(b).first,age1.at(a).first,age1.at(a).second),std::make_tuple(ways.at(a).at(b).second,age2.at(b).first,age2.at(b).second));
            }
        }
    }

   std::cout<<"("<<std::get<0>(ans.first)<<","<<std::get<1>(ans.first)<<","<<std::get<2>(ans.first)<<")"<<"("<<std::get<0>(ans.second)<<","<<std::get<1>(ans.second)<<","<<std::get<2>(ans.second)<<")"<<std::endl;
    std::cout<<count<<std::endl;
    //std::cout<<most_Eva_value<<std::endl;
    return ans;
}
procon::Field beamsearch::agentmove(procon::Field ins_field, std::vector<std::vector<std::pair<int,std::pair<int,int>>>> pos)
{
    procon::Field field = ins_field;

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

        field.setAgent(elements.second.at(0).first, elements.second.at(0).second, elements.first.first, elements.first.second);
        field.setState(elements.first.first, elements.first.second, elements.second.at(0).first + 1);
    }

    for(auto elements : tile_map){
        // bool state_flag = true;
        if(elements.second.size() > 1)
            continue;


        for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
            for(int agent_num = 0; agent_num < 2; ++agent_num)
                if(field.getAgent(turn_flag, agent_num) == elements.first){
                    // state_flag = false;
                    break;
                }
    }
        return field;
}


