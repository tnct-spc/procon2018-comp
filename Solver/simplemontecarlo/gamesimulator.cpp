#include "gamesimulator.h"

GameSimulator::GameSimulator(const procon::Field &inp_field, const unsigned int final) :
    field(inp_field)
{
    final_turn = final;
    now_turn = field.getTurnCount();
    act_stack.resize(2,std::vector<std::tuple<int,int,int>>(2,std::tuple<int,int,int>(0,0,0)));

}

bool GameSimulator::startSimulation(const unsigned int side, const unsigned int agent_1_move, const unsigned int agent_2_move){

    if(canPut(side, 0, agent_1_move) == false || canPut(side, 1, agent_2_move) == false )
        return false;

    return randomizedGame(final_turn, side, agent_1_move, agent_2_move);
}

bool GameSimulator::randomizedGame(const int turn, const unsigned int side, const int agent_1_move, const int agent_2_move){
    if(turn < 0){
        //ここで得点計算処理
        return true;
    }

    if(agent_1_move != -1){

    }



    return randomizedGame(turn - 1, side);
}

bool GameSimulator::canPut(const unsigned int side, const unsigned int move_1, const unsigned int move_2){

    auto check_outofrange = [&](int agent){

        std::pair<int,int> agent_pos = field.getAgent(side, agent);
        int move = (agent == 0 ? move_1 : move_2);

        agent_pos.first += x_list.at(move);
        agent_pos.second += y_list.at(move);

        return !(agent_pos.first < 0 || agent_pos.second < 0 || agent_pos.first >= field.getSize().first || agent_pos.second >= field.getSize().second);
    };

    auto check_conflict = [&]{

        std::pair<int,int> agent_pos_1 = field.getAgent(side, 0);

        if(field.getState(agent_pos_1.first + x_list.at(move_1) , agent_pos_1.second + y_list.at(move_1) ).first != (side == 0 ? 2 : 1) ){

            agent_pos_1.first += x_list.at(move_1);
            agent_pos_1.second += y_list.at(move_1);
        }

        std::pair<int,int> agent_pos_2 = field.getAgent(side, 1);

        if(field.getState(agent_pos_2.first + x_list.at(move_2) , agent_pos_2.second + y_list.at(move_2) ).first != (side == 0 ? 2 : 1) ){

            agent_pos_2.first += x_list.at(move_2);
            agent_pos_2.second += y_list.at(move_2);
        }

        return (agent_pos_1 != agent_pos_2);
    };


    return ( check_outofrange(0) && check_outofrange(1) && check_conflict());
}

void GameSimulator::agentAct(const int turn, const int agent, const int x_inp, const int y_inp){

    std::pair<int,int> agent_pos = field.getAgent(turn, agent);
    std::pair<int,int> grid_size = field.getSize();


    int type = (field.getState(agent_pos.first, agent_pos.second).first == (turn == 0 ? 2 : 1) ? 2 : 1 );


    int x_pos = agent_pos.first + x_inp;
    int y_pos = agent_pos.second + y_inp;

    if(
        x_pos < 0 || x_pos >= grid_size.first ||
        y_pos < 0 || y_pos >= grid_size.second ||
        (type == 1 && field.getState(x_pos, y_pos).first == (turn==1 ? 1 : 2)) ||
        (type == 2 && field.getState(x_pos, y_pos).first != (turn==1 ? 1 : 2))
        ){
        act_stack.at(turn).at(agent) = std::make_tuple(0, 0, 0);
        return ;
    }
    act_stack.at(turn).at(agent) = std::make_tuple(type, x_pos, y_pos);

}

void GameSimulator::changeTurn(){

    std::map<std::pair<int,int>,std::vector<std::pair<int,int>>> dest_map;
    std::map<std::pair<int,int>,std::vector<std::pair<int,int>>> tile_map;

    int type, pos_x, pos_y;

    for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
        for(int agent_num = 0; agent_num < 2; ++agent_num){

            std::tie(type, pos_x, pos_y) = act_stack.at(turn_flag).at(agent_num);
            std::pair<int,int> pos = std::make_pair(pos_x, pos_y);

            dest_map[ pos ].push_back( std::make_pair(turn_flag, agent_num) );

            if(type == 2){
                tile_map[ pos ].push_back( std::make_pair(turn_flag, agent_num) );
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
        bool state_flag = true;
        if(elements.second.size() > 1)
            continue;


        for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
            for(int agent_num = 0; agent_num < 2; ++agent_num)
                if(field.getAgent(turn_flag, agent_num) == elements.first){
                    state_flag = false;
                    break;
                }
        if(state_flag)
            field.setState(elements.first.first, elements.first.second, 0);
    }
}
