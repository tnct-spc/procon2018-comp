#include "lastforce.h"


const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> LastForce::agentAct(int){
    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    procon::Field ins_field = field;

    int nokori = ins_field.getFinalTurn() - ins_field.getTurnCount();

    if(EdgeCut){
        if(nokori >= 3){
            std::cerr<<"計算量危ないです。枝切りを行っていますが、2ターンが限度です"<<std::endl;
        }
    }else{
        if(nokori >= 2){
            std::cerr<<"計算量危ないです。枝切りを行っていませんので、1ターンが限度です"<<std::endl;
        }
    }

    winner = std::vector<std::vector<int>>(18,std::vector<int>(18,0));

    //std::cout<<"詰め"<<std::endl;

    calc(field,0,0,0);
    std::cout<<coun <<"回計算しました"<<std::endl;
    if(checkMate){
        std::cout<<"勝利が確定しました"<<std::endl;
    //    std::cout<<ans.first <<" "<<ans.second<<std::endl;
        return std::make_pair(std::make_tuple(ans.first/9+1,x_list[ans.first%9],y_list[ans.first%9]),std::make_tuple(ans.second/9+1,x_list[ans.second%9],y_list[ans.second%9]));
    }else{
        std::cout<<"不詰みです"<<std::endl;
        if(EdgeCut){
            return std::make_pair(std::make_tuple(0,0,0),std::make_tuple(0,0,0));
        }
        int co = -1;
        for(int a = 0;a < 17;a++){
            for(int b = 0;b < 17;b++){
                if(co < winner.at(a).at(b)){
                    co = winner.at(a).at(b);
                    ans.first = a;
                    ans.second = b;
                }
            }
        }
        //std::cout<<ans.first <<" "<<ans.second<<std::endl;
        return std::make_pair(std::make_tuple(ans.first/9+1,x_list[ans.first%9],y_list[ans.first%9]),std::make_tuple(ans.second/9+1,x_list[ans.second%9],y_list[ans.second%9]));
    }
}
bool LastForce::calc(procon::Field ins_field,int depth,int x,int y){
   coun++;
   // std::cout<<ins_field.getTurnCount()<<" "<<depth<<" "<<ins_field.getFinalTurn()<<std::endl;
   if(coun > 150000)return false;
    if(ins_field.getTurnCount() + depth  == ins_field.getFinalTurn()){
        bool result;
        std::vector<std::pair<int,int>> points = ins_field.getPoints(true);
        if(side == 0)result = points.at(0).first + points.at(0).second > points.at(1).first + points.at(1).second;
        if(side == 1)result = points.at(0).first + points.at(0).second < points.at(1).first + points.at(1).second;
        if(result){
            winner.at(x).at(y) +=  points.at(0).first++;
        }
        return result;
    }
    bool result_hoge = false;
    for(int a = 0;a < 18;a++){
        for(int b = 0;b < 18;b++){
            bool result = true;
            for(int c = 0;c < 18;c++){
                for(int d = 0;d < 18;d++){
                    std::vector<int> vec = {a,b,c,d};
                    if(depth == 0){
                        if(!calc(moveAgent(ins_field,vec),depth+1,a,b)){
                            result = false;
                    //      count++;
                      //    std::cout<<count<<std::endl;
                          if(EdgeCut)  break;
                        }
                    }else{
                        if(!calc(moveAgent(ins_field,vec),depth + 1,x,y)){
                            result = false;
                    //      count++;
                      //    std::cout<<count<<std::endl;
                        if(EdgeCut)    break;
                        }
                    }
                }
              if(!result && EdgeCut)break;
            }
            if(result && depth == 0){
                ans.first = a;
                ans.second = b;
                checkMate = true;
                result_hoge = true;
                if(EdgeCut)return true;
            }
            if(result){
               result_hoge = true;
               if(EdgeCut)return true;
            }
        }
    }
    return result_hoge;
}

void LastForce::setParams(std::vector<std::pair<QString, double>> params)
{

}

procon::Field LastForce::moveAgent(procon::Field field, std::vector<int> act){

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::vector<std::vector<std::tuple<int,int,int>>> act_stack(2, std::vector<std::tuple<int,int,int>>(2));
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

    if(side == 0){
        agentAct(0,0,making_tuple(act.at(0)));
        agentAct(0,1,making_tuple(act.at(1)));
        agentAct(1,0,making_tuple(act.at(2)));
        agentAct(1,1,making_tuple(act.at(3)));
    }else{
        agentAct(0,0,making_tuple(act.at(2)));
        agentAct(0,1,making_tuple(act.at(3)));
        agentAct(1,0,making_tuple(act.at(0)));
        agentAct(1,1,making_tuple(act.at(1)));
    }
    changeTurn();

    return field;

}
