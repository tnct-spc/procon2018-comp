#include "lastforce.h"


const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> LastForce::agentAct(int){

}
bool LastForce::calc(procon::Field field, int depth){

}

procon::Field LastForce::moveAgent(procon::Field field, std::vector<int> act){
    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};


    std::vector<std::vector<std::tuple<int,int,int>>> act_stack;
    auto agentAct = [&](const int turn, const int agent, const std::tuple<int, int, int> tuple_val){

        int type, x_inp, y_inp;
        std::tie(type, x_inp, y_inp) = tuple_val;

        std::pair<int,int> agent_pos = field.getAgent(turn, agent);
        std::pair<int,int> grid_size = field.getSize();
        int x_pos = agent_pos.first + x_inp;
        int y_pos = agent_pos.second + y_inp;

        if(
            type == 0 ||
            x_pos < 0 || x_pos >= grid_size.first ||
            y_pos < 0 || y_pos >= grid_size.second ||
            (type == 1 && field.getState(x_pos, y_pos).first == (turn==1 ? 1 : 2)) ||
            (type == 2 && field.getState(x_pos, y_pos).first == 0)
            ){
            act_stack.at(turn).at(agent) = std::make_tuple(1, agent_pos.first, agent_pos.second);
            return ;
        }
        act_stack.at(turn).at(agent) = std::make_tuple(type, x_pos, y_pos);
    };

    auto changeTurn = [&](){

        //[(x,y)]:(上書き時の色,(色,エージェント)) わかりづらいね
        std::map<std::pair<int,int>,std::pair<int,std::pair<int,int>>> counts;

        int type, pos_x, pos_y;


        //移動しようとしたエージェントが失敗した時に呼ばれる
        std::function<void(std::pair<int,int>)> delete_move = [&](std::pair<int,int> agent_data){

            std::pair<int,int> not_move = field.getAgent(agent_data.first, agent_data.second);

            //もう既に埋まっていて、それが移動予定erなら

            if(counts[not_move].first >= 0){

                std::pair<int,int> next_delete_move = counts[not_move].second;

                counts[not_move] = std::make_pair(-1, std::make_pair(-1, -1));
                //循環参照ケースの回避
                if(next_delete_move != agent_data)
                    delete_move(next_delete_move);
            }

            counts[not_move] = std::make_pair(-1, std::make_pair(-1, -1));
        };


        for(int side = 0; side < 2; ++side){
            for(int agent = 0; agent < 2; ++agent){
                std::tie(type, pos_x, pos_y) = act_stack.at(side).at(agent);

                //移動しないなら
                if(type != 1){
                    std::pair<int,int> not_move = field.getAgent(side, agent);

                    if(counts[not_move].first > 0){//移動しようとしているアレのコンフリクト
                        delete_move(counts[not_move].second);
                        delete_move(std::make_pair(side, agent));
                    }

                    counts[not_move] = std::make_pair(-1,std::make_pair(-1, -1));
                }

                //もう既に存在しているなら
                if(counts.count(std::make_pair(pos_x, pos_y) )){

                    if(counts[std::make_pair(pos_x, pos_y)].first > 0){
                        delete_move(counts[std::make_pair(pos_x, pos_y)].second);
                        delete_move(std::make_pair(side, agent));
                    }

                    counts[std::make_pair(pos_x, pos_y)] = std::make_pair(-1, std::make_pair(-1, -1));
                }else{
                    int color = 0;
                    if(type != 2)
                        color = side + 1;

                    counts[std::make_pair(pos_x, pos_y)] = std::make_pair(color ,std::make_pair(side,agent));
                }
            }
        }

        for(auto moves : counts){
            if(moves.second.first == -1)
                continue;

            field.setState(moves.first.first, moves.first.second, moves.second.first);

            if(moves.second.first != 0)
                field.setAgent(moves.second.second.first, moves.second.second.second, moves.first.first, moves.first.second);
        }


    };
    auto making_tuple = [&](int act_num){
        return std::make_tuple((act_num/9 + 1),x_list[act_num % 9],y_list[act_num % 9]);
    };

    agentAct(0,0,making_tuple(act.at(0)));
    agentAct(0,1,making_tuple(act.at(1)));
    agentAct(1,0,making_tuple(act.at(2)));
    agentAct(1,1,making_tuple(act.at(3)));

    changeTurn();

    return field;

}
