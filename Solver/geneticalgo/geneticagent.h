#ifndef GENETICAGENT_H
#define GENETICAGENT_H

#include <random>
#include <vector>

class GeneticAgent
{
public:
    //flagがtrueなら乱択で初期パラメータ生成
    GeneticAgent(bool flag = true);

    bool operator<(const GeneticAgent &other) const;

    void setData(const std::vector<double>& inp);
    std::vector<double> getData();

    //パラメータを持ってて実質構造体

    double rand = 0.5; //乱択度合い(高いほど乱択)
    double minus = 0.5; //マイナスの回避度(高いほど避ける)
    double next_p = 0.5; //次の手での得点の優先度(高いほど優先)
    double region = 0.5; //領域ポイントの優先度(高いほど優先)
    double removal = 0.5; //タイル除去の優先度(高いほど優先)
    double nomove = 0.5; //移動しなかった時のペナルティ(高いほどペナ大)
    double backmove = 0.5; //もう塗られてる所に行った時のペナルティ(高いほどペナ大)
    
    int try_count = 0;//試行回数と勝利回数
    int win_count = 0;

    int size = 7;//パラメータの数

};

#endif // GENETICAGENT_H
