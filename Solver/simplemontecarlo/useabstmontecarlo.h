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
    unsigned int try_time = 3 * 1e7;

    std::vector<std::vector<double>> values = {
        {0.66, 0.62, 0.43, 0.62, 0.62, 0.47 ,0.62, 0.46, 0.46, 0.65},
        {0.585501, 0.936013, 0.750115, 0.136817, 0.649388, 0.589486, 0.882378, 0.090591, 0.946676, 0.339145},
        {0.424304, 0.870152, 0.969697, 0.994353, 0.79771, 0.339376, 0.567848, 0.212827, 0.662733, 0.526067},
        {0.751296, 0.665754, 0.556531, 0.205988, 0.606986, 0.87203, 0.808129, 0.282799, 0.703115, 0.371682},
        {0.659504, 0.610229, 0.103188, 0.237939, 0.979475, 0.337169, 0.937114, 0.351038, 0.584697, 0.322641},
        {0.787281, 0.883428, 0.667984, 0.828937, 0.868376, 0.101065, 0.72732, 0.130173, 0.862691, 0.387112},
        {0.959878, 0.891539, 0.705233, 0.963502, 0.921754, 0.789983, 0.92909, 0.137433, 0.685138, 0.297063}
    };

    //ここにパラメータを置く(いい結果が出たら他のアルゴも出せるようにする
    int values_size;

};

#endif // USEABSTMONTECARLO_H
