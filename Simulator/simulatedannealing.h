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
    bool buttleAgents(GeneticAgent& first, GeneticAgent& second, bool is_rand);

    int retRandom(int st, int en);

    std::vector<GameManager*> managers;

    std::mutex mtx;

    int cpu_num;

    int algo_number;

    std::random_device rnd;
    std::mt19937 mt;

    double max_change_val_st = 0.4;//パラメータの最大変化量
    double max_change_val_en = 0.1;//時間によって最大変化量が変化する
    //合計の試行回数
    const int max_try = 1e4;


    //乱択相手の(対戦回数,一回辺りの対戦数)
    const std::pair<int,int> buttle_rand = std::make_pair(60, 7);
    //直接対決の(重み,一回辺りの対戦数)
    //総得点が等しくなるのを防ぐために重みを小数にしている
    const std::pair<double,int> buttle_direct = std::make_pair(4.5, 9);

    //開始時の温度(温度は線形で変化させる)
    const double start_temp = 0.125 * (buttle_rand.first + buttle_direct.first);
    const double end_temp = 1;

    GeneticAgent* agent;
};

#endif // SIMULATEDANNEALING_H
