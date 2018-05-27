#ifndef DOUBLEAGENTGENETIC_H
#define DOUBLEAGENTGENETIC_H

#include "gamemanager.h"
#include <mutex>

class DoubleAgentGenetic
{
public:

    DoubleAgentGenetic(int algo_number);
    void run();

private:
    int algo_number;
    int cpu_num;

    std::random_device rnd;
    std::mt19937 mt;

    std::mutex mtx;

    int retRandom(int st, int en);//[st,en]の乱数

    std::vector<GameManager*> managers;
    std::vector<GeneticAgent> agents;

    const unsigned int agent_num = 100;

};

#endif // DOUBLEAGENTGENETIC_H
