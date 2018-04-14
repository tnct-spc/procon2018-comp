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


    return true;
}

bool GameSimulator::canPut(const unsigned int side, const unsigned int move_1, const unsigned int move_2){

    auto outofrange_calc = [&](int agent){

        std::pair<int,int> agent_pos = field.getAgent(side, agent);
        int move = (agent == 0 ? move_1 : move_2);

        agent_pos.first += x_list.at(move);
        agent_pos.second += y_list.at(move);

        return !(agent_pos.first < 0 || agent_pos.second < 0 || agent_pos.first >= field.getSize().first || agent_pos.second >= field.getSize().second);
    };
    return ( outofrange_calc(0) && outofrange_calc(1) );
}
