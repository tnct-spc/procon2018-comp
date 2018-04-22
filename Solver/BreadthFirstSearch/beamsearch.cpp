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


    const procon::Field field = manager->getField();

    auto sortEva = [&](std::pair<int,std::tuple<procon::Field,std::tuple<int,int,int>,std::tuple<int,int,int>>> const& a,std::pair<int,std::tuple<procon::Field,std::tuple<int,int,int>,std::tuple<int,int,int>>> const& b)->bool
    {
      return a.first < b.first;
    };
  //  cout<<"unya"<<endl;
    std::vector<std::vector<std::pair<int,int>>> Agents = field.getAgents();

    if(side==0){
        agent1=Agents.at(0).at(0);
        agent2=Agents.at(0).at(1);
    }else{
        agent1=Agents.at(1).at(0);
        agent2=Agents.at(1).at(1);
    }
   // cout<<"nya"<<endl;
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
 //   int count = 0;
    for(int turn = 0;turn < beam_turn;turn++){
      //  cout<<"turn開始"<<endl;
        if(turn==0){
            for(int a = 0;a < 8;a++){
                if(age1.at(a).first+agent1.first<0)continue;
                if(age1.at(a).first+agent1.first>=field_size.first)continue;
                if(age1.at(a).second+agent1.second<0)continue;
                if(age1.at(a).second+agent1.second>=field_size.second)continue;

                for(int b = 0;b < 8;b++){
                    if(age2.at(b).first+agent2.first<0)continue;
                    if(age2.at(b).first+agent2.first>=field_size.first)continue;
                    if(age2.at(b).second+agent2.second<0)continue;
                    if(age2.at(b).second+agent2.second>=field_size.second)continue;
                    procon::Field ins_field = field;
                    int way1,way2;
              //      cout<<ins_field.getState(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second).first<<endl;
                    if(ins_field.getState(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second).first==side+1){

                    ins_field.setAgent(side,0,agent1.first+age1.at(a).first,agent1.second+age1.at(a).second);
                    way1 = 1;

                    }else if(ins_field.getState(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second).first==0){

                        ins_field.setState(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second,side+1);
                        ins_field.setAgent(side,0,agent1.first+age1.at(a).first,agent1.second+age1.at(a).second);
                        way1 = 1;

                    }else{

                        ins_field.setState(agent1.first+age1.at(a).first,agent1.second+age1.at(a).second,0);
                        way1 = 2;

                    }
                    if(ins_field.getState(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second).first==side+1){

                    ins_field.setAgent(side,1,agent2.first+age2.at(b).first,agent2.second+age2.at(b).second);
                    way2 = 1;

                    }else if(ins_field.getState(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second).first==0){

                        ins_field.setState(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second,side+1);
                        ins_field.setAgent(side,1,agent2.first+age2.at(b).first,agent2.second+age2.at(b).second);
                        way2 = 1;

                    }else{

                        ins_field.setState(agent2.first+age2.at(b).first,agent2.second+age2.at(b).second,0);
                        way2 = 2;

                    }
                    int v = Evaluation_Field(ins_field);
                    beam.push_back(std::make_pair(v,std::make_tuple(ins_field,std::make_tuple(way1,age1.at(a).first,age1.at(a).second),std::make_tuple(way2,age2.at(b).first,age2.at(b).second))));
                }
            }
            sort(beam.begin(),beam.end(),sortEva);
        }else{
           //cout<<"2回め以上"<<endl;
            std::vector<std::pair<int,std::tuple<procon::Field,std::tuple<int,int,int>,std::tuple<int,int,int>>>> ins_beam;
            int ins_size = beam.size();
            for(int range = 0;range < std::min(beam_range,ins_size);range++){
                std::pair<int,std::tuple<procon::Field,std::tuple<int,int,int>,std::tuple<int,int,int>>> ins_value = beam.at(range);
                procon::Field Provisional_ins_Field = std::get<0>(ins_value.second);
                for(int a = 0;a < 8;a++){
                    if(age1.at(a).first+Provisional_ins_Field.getAgent(side,0).first<0)continue;
                    if(age1.at(a).first+Provisional_ins_Field.getAgent(side,0).first>=field_size.first)continue;
                    if(age1.at(a).second+Provisional_ins_Field.getAgent(side,0).second<0)continue;
                    if(age1.at(a).second+Provisional_ins_Field.getAgent(side,0).second>=field_size.second)continue;
                 //   cout<<"Ne"<<endl;
                    for(int b = 0;b < 8;b++){
                        //cout<<age2.at(b).first+Provisional_ins_Field.getAgent(side,1).first<<" "<<age2.at(b).second+Provisional_ins_Field.getAgent(side,1).second<<endl;
                        if(age2.at(b).first+Provisional_ins_Field.getAgent(side,1).first<0)continue;
                        if(age2.at(b).first+Provisional_ins_Field.getAgent(side,1).first>=field_size.first)continue;
                        if(age2.at(b).second+Provisional_ins_Field.getAgent(side,1).second<0)continue;
                        if(age2.at(b).second+Provisional_ins_Field.getAgent(side,1).second>=field_size.second)continue;
                        procon::Field ins_field = Provisional_ins_Field;

                        if(ins_field.getState(Provisional_ins_Field.getAgent(side,0).first+age1.at(a).first,Provisional_ins_Field.getAgent(side,0).second+age1.at(a).second).first==side+1){
                        ins_field.setAgent(side,0,Provisional_ins_Field.getAgent(side,0).first+age1.at(a).first,Provisional_ins_Field.getAgent(side,0).second+age1.at(a).second);
                        }else if(ins_field.getState(Provisional_ins_Field.getAgent(side,0).first+age1.at(a).first,Provisional_ins_Field.getAgent(side,0).second+age1.at(a).second).first==0){
                            ins_field.setState(Provisional_ins_Field.getAgent(side,0).first+age1.at(a).first,Provisional_ins_Field.getAgent(side,0).second+age1.at(a).second,side+1);
                            ins_field.setAgent(side,0,Provisional_ins_Field.getAgent(side,0).first+age1.at(a).first,Provisional_ins_Field.getAgent(side,0).second+age1.at(a).second);
                        }else{
                            ins_field.setState(Provisional_ins_Field.getAgent(side,0).first+age1.at(a).first,Provisional_ins_Field.getAgent(side,0).second+age1.at(a).second,0);
                        }
                        if(ins_field.getState(Provisional_ins_Field.getAgent(side,1).first+age2.at(b).first,Provisional_ins_Field.getAgent(side,1).second+age2.at(b).second).first==side+1){
                        ins_field.setAgent(side,1,Provisional_ins_Field.getAgent(side,1).first+age2.at(b).first,Provisional_ins_Field.getAgent(side,1).second +age2.at(b).second);
                        }else if(ins_field.getState(Provisional_ins_Field.getAgent(side,1).first+age2.at(b).first,Provisional_ins_Field.getAgent(side,1).second+age2.at(b).second).first==0){
                            ins_field.setState(Provisional_ins_Field.getAgent(side,1).first+age2.at(b).first,Provisional_ins_Field.getAgent(side,1).second+age2.at(b).second,side+1);
                            ins_field.setAgent(side,1,Provisional_ins_Field.getAgent(side,1).first+age2.at(b).first,Provisional_ins_Field.getAgent(side,1).second+age2.at(b).second);
                        }else{
                            ins_field.setState(Provisional_ins_Field.getAgent(side,1).first+age2.at(b).first,Provisional_ins_Field.getAgent(side,1).second+age2.at(b).second,0);
                        }
                        int v = Evaluation_Field(ins_field);
                        ins_beam.push_back(std::make_pair(v,std::make_tuple(ins_field,std::get<1>(ins_value.second),std::get<2>(ins_value.second))));
                    }
                }
            }
            beam = ins_beam;
            ins_beam.clear();
            std::sort(beam.begin(),beam.end(),sortEva);
        }
    }
    cout<<"("<<agent1.first<<","<<agent1.second<<")"<<"("<<agent2.first<<","<<agent2.second<<")"<<endl;
    cout<<"("<<std::get<0>(std::get<1>(beam.front().second))<<","<<std::get<1>(std::get<1>(beam.front().second))<<","<<std::get<2>(std::get<1>(beam.front().second))<<")"<<"("<<std::get<0>(std::get<2>(beam.front().second))<<","<<std::get<1>(std::get<2>(beam.front().second))<<","<<std::get<2>(std::get<2>(beam.front().second))<<")"<<endl;
    return std::make_pair(std::get<2>(beam.front().second),std::get<1>(beam.front().second));
}
