#include "gamesimulator.h"

GameSimulator::GameSimulator(const procon::Field &inp_field, const unsigned int final) :
    field(inp_field)
{
    final_turn = final;
    now_turn = field.getTurnCount();

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
