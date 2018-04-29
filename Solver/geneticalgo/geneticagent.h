#ifndef GENETICAGENT_H
#define GENETICAGENT_H

#include <random>
#include <vector>

class GeneticAgent
{
public:
    //flagがtrueなら乱択で初期パラメータ生成
    GeneticAgent(int size, bool flag = true);


    void setData(const std::vector<double>& inp);
    const std::vector<double>& getData() const;

    int try_count = 0;//試行回数と勝利回数
    int win_count = 0;

    int size;//パラメータの数

private:

    std::vector<double> status;

    bool operator<(const GeneticAgent &other) const;



    


};

#endif // GENETICAGENT_H
