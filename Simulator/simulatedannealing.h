#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <fstream>

#include "gamemanager.h"
#include "geneticalgo/geneticalgo.h"

class SimulatedAnnealing
{
public:
    SimulatedAnnealing(int algo_number);
    void run();

private:

    void updateAgent();
    bool buttleAgents(GeneticAgent& first, GeneticAgent& second);

    int retRandom(int st, int en);

    std::vector<GameManager*> managers;

    int cpu_num;

    int algo_number;

    std::random_device rnd;
    std::mt19937 mt;

    //合計の試行回数
    const int max_try = 1e5;
    //一回の試行でのプレイアウト回数
    const int buttle_count = 100;

    GeneticAgent* agent;
};

#endif // SIMULATEDANNEALING_H
