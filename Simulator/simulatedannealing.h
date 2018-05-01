#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <fstream>
#include <cmath>

#include "gamemanager.h"
#include "geneticalgo/geneticalgo.h"

class SimulatedAnnealing
{
public:
    SimulatedAnnealing(int algo_number);
    void run();

private:

    void updateAgent(int now_count);
    bool buttleAgents(GeneticAgent& first, GeneticAgent& second);

    int retRandom(int st, int en);

    std::vector<GameManager*> managers;

    int cpu_num;

    int algo_number;

    std::random_device rnd;
    std::mt19937 mt;

    double max_change_val = 0.2;//パラメータの最大変化量
    //合計の試行回数
    const int max_try = 1e5;

    const int end_temp = 1;
    double const_val = 1.0000485;//温度調節のための定数

    //一回の試行でのプレイアウト回数
    const int buttle_count = 100;
    //乱択相手の対戦回数
    const int buttle_rand = 100;
    //直接対決の重み
    const int buttle_val = 30;

    GeneticAgent* agent;
};

#endif // SIMULATEDANNEALING_H
