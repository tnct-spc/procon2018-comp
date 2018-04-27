#ifndef GENETIC_H
#define GENETIC_H

#include <random>

#include "gamemanager.h"
#include "geneticalgo/geneticalgo.h"

class Genetic
{
public:
    Genetic();
    void run();

private:
    /*
    procon::Field field;
    GameSimulator sim;
    */

    std::random_device rnd;
    std::mt19937 mt;

    //先手の勝利回数を返す
    int buttleAgents(GeneticAgent& first, GeneticAgent& second);

    int retRandom(int st, int en);//[st,en]の乱数

    std::vector<GeneticAgent> agents;

    //集合の数
    const int agent_num = 100;
    //一回の試行でのプレイアウト回数
    const int buttile_count = 1000;
};

#endif // GENETIC_H
