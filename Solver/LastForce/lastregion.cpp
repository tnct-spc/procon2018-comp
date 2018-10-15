#include "lastregion.h"


const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> LastRegion::agentAct(int){
 //   std::cout<<side<<std::endl;

    std::stack<std::tuple<int, std::pair<int,int>, std::pair<int,int>, std::vector<std::pair<int, int>>,int, std::bitset<288>>> sta;

    std::bitset<288> _state;

   // std::cout<<"SIZE"<<field.getSize().first<<" "<<field.getSize().second<<std::endl;

    std::vector<std::vector<int>> points(12, std::vector<int>(12));
    for(int x = 0;x < field.getSize().first; x++){
        for(int y = 0;y < field.getSize().second; y++){
            points.at(x).at(y) = field.getState(x,y).second;
        }
    }
    procon::Field _field = field;

    std::vector<std::pair<std::pair<int,int>,int>> area = _field.ifBreakArea(side, 0);

    for(auto que : area){
        points.at(que.first.first).at(que.first.second) += que.second;
    }

    area = _field.ifBreakArea(side, 1);
    for(auto que : area){
        points.at(que.first.first).at(que.first.second) += que.second;
    }

    auto setState = [=](std::bitset<288>& bits, int state, int x , int y){
        if(!(0 <= x && x <= field.getSize().first - 1 && 0 <= y && y <= field.getSize().second - 1)){
            std::cerr<<"ERROR :  LastForce内setStateにて盤面外を指定しています!!"<<std::endl;
            std::abort();
        }
        std::bitset<288> w = 3;
        bits &= ~( w << (2*(12*y+x)));
        w = state;
        bits |= ( w << (2*(12*y+x)));
    };

    auto getState = [=](std::bitset<288>& bits, int x, int y){
        if(!(0 <= x && x <= field.getSize().first - 1 && 0 <= y && y <= field.getSize().second - 1)){
            std::cerr<<"ERROR : LastForce内getStateにて盤面外を指定しています!!"<<std::endl;
            std::abort();
        }
        std::bitset<288> w(0uL);
        w |= bits >> (2*(12*y+x));
        w &= 3;
        return w.to_ulong();
    };
    auto check_out_of_range = [&](std::pair<int,int> pos, int move){
        return (0 <= pos.first + LastRegion::dx.at(move)) &&(pos.first + LastRegion::dx.at(move) <= field.getSize().first - 1) &&( 0 <= pos.second + LastRegion::dy.at(move)) && (pos.second + LastRegion::dy.at(move) <= field.getSize().second -1) ;
    };


    for(int x = 0;x < field.getSize().first; x++){
        for(int y = 0;y < field.getSize().second; y++){
            setState(_state, field.getState(x,y).first, x , y);
        }
    }
    sta.push(std::make_tuple(0, field.getAgent(side, 0), field.getAgent(side, 1), std::vector<std::pair<int,int>>(), 0, _state));

    int ans_adv = -1e9;
    std::pair<int,int> ans;


    while(!sta.empty()){
  //      std::cout<<"HOGE"<<std::endl;
        std::tuple<int, std::pair<int,int>, std::pair<int,int>, std::vector<std::pair<int, int>>, int, std::bitset<288>> node = sta.top();
        sta.pop();

        int depth = std::get<0>(node);
        std::pair<int,int> agent1_pos = std::get<1>(node);
        std::pair<int,int> agent2_pos = std::get<2>(node);
        std::vector<std::pair<int,int>> log = std::get<3>(node);
        int adv = std::get<4>(node);
        std::bitset<288> node_state = std::get<5>(node);

        if(depth >= std::min(max_turn_count, field.getFinalTurn() - field.getTurnCount())){
            if(ans_adv <= adv){
                ans_adv = adv;
                ans = log.front();
            }
            continue;
        }

        std::vector<std::pair<int, std::pair<int, int>>> next_nodes;

  //     std::cout<<"MOKE"<<std::endl;

        for(int index_1 = 0;index_1 < 8;index_1++){
            if(!check_out_of_range(agent1_pos, index_1))continue;
            int way1;
            int adv1;
            int state1 = getState(node_state, agent1_pos.first + LastRegion::dx.at(index_1), agent1_pos.second + LastRegion::dy.at(index_1));

           // std::cout<<state1<<std::endl;

            if(state1 != side+1 && state1 != 0){
                way1 = 2;
            }else if(state1 == 0){
                way1 = 1;
            }else{
                way1 = 0;
            }

            adv1 = points.at(agent1_pos.first + LastRegion::dx.at(index_1)).at(agent1_pos.second + LastRegion::dy.at(index_1));
            if(way1 == 0)adv1 = 0;

            for(int index_2 = 0;index_2 < 8;index_2++){
                if(!check_out_of_range(agent2_pos, index_2))continue;

                int way2;
                int adv2;
                int state2 = getState(node_state, agent2_pos.first + LastRegion::dx.at(index_2), agent2_pos.second + LastRegion::dy.at(index_2));

           //     std::cout<<state2<<std::endl;

                if(state2 != side+1 && state2 != 0){
                    way2 = 2;
                }else if(state2 == 0){
                    way2 = 1;
                }else{
                    way2 = 0;
                }

                adv2 = points.at(agent2_pos.first + LastRegion::dx.at(index_2)).at(agent2_pos.second + LastRegion::dy.at(index_2));
                if(way2 == 0)adv2 = 0;

                std::pair<int,int> next_move_pos1 = std::make_pair(agent1_pos.first + LastRegion::dx.at(index_1), agent1_pos.second + LastRegion::dy.at(index_1));
                std::pair<int,int> next_move_pos2 = std::make_pair(agent2_pos.first + LastRegion::dx.at(index_2), agent2_pos.second + LastRegion::dy.at(index_2));

                if(way1 == 2 && agent1_pos == next_move_pos2){
                    continue;
                }
                if(way2 == 2 && agent2_pos == next_move_pos1){
                    continue;
                }
                if(next_move_pos1 == next_move_pos2){
                    continue;
                }

                next_nodes.push_back(std::make_pair(adv1 + adv2, std::make_pair(index_1, index_2)));
            }
        }
        std::sort(next_nodes.begin(), next_nodes.end());
        std::reverse(next_nodes.begin(),next_nodes.end());

  //      std::cout<<"NEM"<<std::endl;

        for(int co = 0;co < std::min((int)next_nodes.size(), move_count); co++){

           // std::cout<< agent1_pos.first + LastRegion::dx.at(next_nodes.at(co).second.first)<<" "<<agent1_pos.second + LastRegion::dy.at(next_nodes.at(co).second.first)<<" "<<agent2_pos.first + LastRegion::dx.at(next_nodes.at(co).second.second)<<" "<<agent2_pos.second + LastRegion::dy.at(next_nodes.at(co).second.second)<<std::endl;

            int state1 = getState(node_state, agent1_pos.first + LastRegion::dx.at(next_nodes.at(co).second.first), agent1_pos.second + LastRegion::dy.at(next_nodes.at(co).second.first));
            int state2 = getState(node_state, agent2_pos.first + LastRegion::dx.at(next_nodes.at(co).second.second), agent2_pos.second + LastRegion::dy.at(next_nodes.at(co).second.second));
            std::pair<int,int> next_pos1, next_pos2;

            std::bitset<288> next_state = node_state;

            if(state1 != 0 && state1 != side + 1){
                next_pos1 = agent1_pos;
                setState(next_state, 0,agent1_pos.first + LastRegion::dx.at(next_nodes.at(co).second.first), agent1_pos.second + LastRegion::dy.at(next_nodes.at(co).second.first));
            }else{
                next_pos1 = std::make_pair(agent1_pos.first + LastRegion::dx.at(next_nodes.at(co).second.first), agent1_pos.second + LastRegion::dy.at(next_nodes.at(co).second.first));
                setState(next_state, side + 1, agent1_pos.first + LastRegion::dx.at(next_nodes.at(co).second.first), agent1_pos.second + LastRegion::dy.at(next_nodes.at(co).second.first));
            }

            if(state2 != 0 && state2 != side + 1){
                next_pos2 = agent2_pos;
                setState(next_state, 0,agent2_pos.first + LastRegion::dx.at(next_nodes.at(co).second.second), agent2_pos.second + LastRegion::dy.at(next_nodes.at(co).second.second));
            }else{
                next_pos2 = std::make_pair(agent2_pos.first + LastRegion::dx.at(next_nodes.at(co).second.second), agent2_pos.second + LastRegion::dy.at(next_nodes.at(co).second.second));
                setState(next_state, side + 1, agent2_pos.first + LastRegion::dx.at(next_nodes.at(co).second.second), agent2_pos.second + LastRegion::dy.at(next_nodes.at(co).second.second));
            }
            log.push_back(next_nodes.at(co).second);
            sta.push(std::make_tuple(depth+1, next_pos1, next_pos2, log, adv + next_nodes.at(co).first, next_state));
        }
   //     std::cout<<"HEHEHE"<<std::endl;
    }
    return std::make_pair(std::make_tuple((field.getState(field.getAgents().at(side).at(0).first + LastRegion::dx.at(ans.first),  field.getAgents().at(side).at(0).second + LastRegion::dy.at(ans.first)).first != (side ? 1 : 2) ? 1 : 2), LastRegion::dx.at(ans.first), LastRegion::dy.at(ans.first)),
                          std::make_tuple((field.getState(field.getAgents().at(side).at(1).first + LastRegion::dx.at(ans.second), field.getAgents().at(side).at(1).second + LastRegion::dy.at(ans.second)).first != (side ? 1 : 2) ? 1 : 2), LastRegion::dx.at(ans.second), LastRegion::dy.at(ans.second)));
}

const std::vector<int> LastRegion::dx({1, 1, 0, -1, -1, -1, 0, 1, 0});
const std::vector<int> LastRegion::dy({0, -1, -1, -1, 0, 1, 1, 1, 0});
void LastRegion::setParams(std::vector<std::pair<QString, double>> params)
{

}
