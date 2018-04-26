#ifndef GENETICAGENT_H
#define GENETICAGENT_H

#include <random>

class GeneticAgent
{
public:
    GeneticAgent();

    //パラメータを持ってて実質構造体

    double rand = 0.5; //乱択度合い(高いほど乱択)
    double minus = 0.5; //マイナスの回避度(高いほど避ける)
    double next_p = 0.5; //次の手での得点の優先度(高いほど優先)
    double region = 0.5; //領域ポイントの優先度(高いほど優先)
    double removal = 0.5; //タイル除去の優先度(高いほど優先)
    


};

#endif // GENETICAGENT_H
