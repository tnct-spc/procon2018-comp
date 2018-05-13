#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <fstream>
#include <cmath>
#include <mutex>

#include "gamemanager.h"
#include "geneticalgo/geneticalgo.h"

class SimulatedAnnealing
{
public:
    SimulatedAnnealing(int algo_number);
    void run();

private:

    void updateAgent(int now_count);
    double buttleAgents(GeneticAgent& first, GeneticAgent& second, bool is_rand);

    int retRandom(int st, int en);

    std::vector<GameManager*> managers;

    std::mutex mtx;

    int cpu_num;

    int algo_number;

    std::random_device rnd;
    std::mt19937 mt;

    double max_change_val = 0.2;//パラメータの最大変化量
    //合計の試行回数
    const int max_try = 1e4;


    //乱択相手の対戦回数と一回あたりの対戦数
    const std::pair<int,int> buttle_rand = std::make_pair(60, 10);
    //直接対決の重みと回数
    const std::pair<int,int> buttle_direct = std::make_pair(10, 100);

    //開始時の温度(温度は線形で変化させる)
    const double start_temp = 0.03 * (buttle_rand.first + buttle_direct.first);
    const double end_temp = 1;

    GeneticAgent* agent;
};

#endif // SIMULATEDANNEALING_H
