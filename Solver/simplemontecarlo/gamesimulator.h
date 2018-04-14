#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include "field.h"

class GameSimulator
{

public:
    GameSimulator(const procon::Field& inp_field, const unsigned int final);

    bool startSimulation(const unsigned int side, const unsigned int agent_1_move, const unsigned int agent_2_move);
    bool canPut(const unsigned int side, const unsigned int agent, const unsigned int move);

private:
    procon::Field field;
    unsigned int final_turn;
    unsigned int now_turn;

    std::vector<int> x_list = {1, 1, 1, 0, 0, 0, -1, -1, -1};
    std::vector<int> y_list = {-1, 0, 1, -1, 0, 1, -1, 0, 1};

};

#endif // GAMESIMULATOR_H
