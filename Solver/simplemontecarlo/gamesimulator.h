#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include "field.h"

class GameSimulator
{

public:
    GameSimulator(const procon::Field& inp_field, const unsigned int final);

    bool startSimulation(const unsigned int side, const unsigned int agent_1_move, const unsigned int agent_2_move);

    //この関数は「置けるかどうか」ではなく「どう考えても酷い動作にならないか」を判断するつもり(同じ位置に二人がmove、除去の重なりetc…)
    bool canPut(const unsigned int side, const unsigned int move_1, const unsigned int move_2);

private:
    procon::Field field;
    unsigned int final_turn;
    unsigned int now_turn;

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

};

#endif // GAMESIMULATOR_H
