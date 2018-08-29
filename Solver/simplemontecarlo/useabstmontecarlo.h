#ifndef USEABSTMONTECARLO_H
#define USEABSTMONTECARLO_H

#include <mutex>

// コピペ(†悪魔的行為†)
#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "useabstractdata.h"

class UseAbstMonteCarlo : public AlgorithmWrapper
{
public:
    UseAbstMonteCarlo(const procon::Field& field, int final_turn, bool side);

    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int now_turn);

private:

    std::mutex mtx;
    std::random_device rnd;
    std::mt19937 mt;

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::vector<GameManager*> mgr;

    std::vector<std::thread> threads;

    unsigned int cpu_num;

    const double ucb_val = 1.4;//定数

    //合計での試行回数
    unsigned int try_time = 3 * 1e6;

    std::vector<std::vector<double>> values = {
        {1.6, 1.2, -0.7, 1.2, 1.2, -0.3 ,1.2, -0.4, -0.4, 1.5}
    };

    //ここにパラメータを置く(いい結果が出たら他のアルゴも出せるようにする
    int values_size;

};

#endif // USEABSTMONTECARLO_H
