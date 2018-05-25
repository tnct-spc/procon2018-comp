#ifndef GENETICAGENT_H
#define GENETICAGENT_H

#include <random>
#include <vector>
#include <algorithm>

class GeneticAgent
{
public:
    //flagが0ならパラメータは初期値
    //flagが1なら乱択
    //flagが2なら総和1の乱択
    GeneticAgent(int size, int flag = 1);


    void setData(const std::vector<double>& inp);
    const std::vector<double>& getData() const;

    int try_count = 0;//試行回数と勝利回数
    int win_count = 0;

    int size = 114514;//パラメータの数

private:

    std::vector<double> status;

    bool operator<(const GeneticAgent &other) const;



    


};

#endif // GENETICAGENT_H
