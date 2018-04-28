#ifndef GENETIC_H
#define GENETIC_H

#include <random>
#include <unordered_set>

#include "gamemanager.h"
#include "geneticalgo/geneticalgo.h"

class Genetic
{
public:
    Genetic();
    void run();

    // (今の所は)遺伝的アルゴリズムで最適なやつを出していく

private:
    /*
    procon::Field field;
    GameSimulator sim;
    */

    GameManager manager;

    std::random_device rnd;
    std::mt19937 mt;

    //先手が勝利したかどうかを返す
    bool buttleAgents(GeneticAgent& first, GeneticAgent& second);

    void startTournament();

    int retRandom(int st, int en);//[st,en]の乱数

    std::vector<GeneticAgent> agents;

    //集合の数
    const unsigned int agent_num = 100;
    //トーナメントの数(今回は10ずつrandで最優秀の10体を取る)
    const int tournament_size = 10;
    //トーナメントでの対戦数
    const int tournament_buttle = 5;
    //一回の試行でのプレイアウト回数
    const int buttle_count = 1001;
    //世代数の最大
    const int max_gen = 200;
};

#endif // GENETIC_H
