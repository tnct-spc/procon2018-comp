#ifndef GENETIC_H
#define GENETIC_H

#include <fstream>
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

    std::vector<GameManager*> managers;

    std::random_device rnd;
    std::mt19937 mt;

    //先手が勝利したかどうかを返す
    bool buttleAgents(GeneticAgent& first, GeneticAgent& second);

    void startTournament();
    void generateAgents();

    int retRandom(int st, int en);//[st,en]の乱数

    std::vector<GeneticAgent> agents;

    int cpu_num;

    double mutate_per = 0.3;//突然変異率(これ以外は交叉)

    double cross_change_per = 0.3;//交叉時にパラメータを少し変化させる確率
    double cross_change_val = 0.2;//交叉時のパラメータの最大変化量

    //集合の数
    const unsigned int agent_num = 100;
    //トーナメントの数
    const int tournament_count = 20;
    //トーナメントの大きさ
    const int tournament_size = 8;

    //トーナメントでの対戦数
    const int tournament_buttle = 5;
    //一回の試行でのプレイアウト回数
    const int buttle_count = 100;
    //世代数の最大
    const int max_gen = 200;
};

#endif // GENETIC_H
