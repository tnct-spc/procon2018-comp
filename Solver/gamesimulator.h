#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include "field.h"
#include <tuple>
#include <map>
#include <random>

#include <iostream>

class GameSimulator
{

public:
    GameSimulator(const std::vector<std::vector<int>>& value_data, int now_turn);

    bool startSimulation(const unsigned int side, const unsigned int agent_1_move, const unsigned int agent_2_move, const unsigned int final);
    bool randomizedGame(const int turn, const unsigned int side, const int agent_1_move = -1, const int agent_2_move = -1);

    //この関数は「置けるかどうか」ではなく「どう考えても酷い動作にならないか」を判断するつもり(同じ位置に二人がmove、除去の重なりetc…)
    bool canPut(const unsigned int side, const unsigned int move_1, const unsigned int move_2);
    void resetField(const std::vector<std::vector<int>>& inp_field, const std::vector<std::vector<std::pair<int,int>>>& inp_agent);

    void agentAct(const int turn, const int agent, const int x_inp, const int y_inp);
    void agentAct(const int turn, const int agent, const int move);

private:
    // procon::Field field;

    const std::vector<std::vector<int>>& value_data;

    unsigned int now_turn;
    //これはコンストラクタで指定する「残りのターン数」なので注意
    unsigned int final_turn;

    void changeTurn();

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::vector<std::vector<std::tuple<int,int,int>>> act_stack;

    std::vector<std::vector<int>> field_data;
    std::vector<std::vector<std::pair<int,int>>> agent_data;
};

#endif // GAMESIMULATOR_H
