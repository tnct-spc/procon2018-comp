#ifndef MONTECARLOWITHALGO_H
#define MONTECARLOWITHALGO_H

#include "algorithmwrapper.h"
#include "gamemanager.h"

#include "geneticalgo/geneticagent.h"

#include <thread>
#include <vector>
#include <cmath>
#include <queue>
#include <mutex>

class MontecarloWithAlgo : public AlgorithmWrapper
{
    //深さ1のノード(81パターン)を生成してUCBでやる
    //二手目以降はお互いにGeneticAlgoからの厳選パターン(10種程度)を適用する

    //using AlgorithmWrapper::AlgorithmWrapper;

public:
    MontecarloWithAlgo(std::shared_ptr<GameManager> manager_ptr);

    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);

private:

    std::mutex mtx;

    std::random_device rnd;
    std::mt19937 mt;

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::vector<GameManager*> mgr;

    std::vector<std::thread> threads;

    unsigned int cpu_num;

    const double ucb_val = 0.5;//定数

    //合計での試行回数
    unsigned int try_time = 3 * 1e6;

    std::vector<int> win_count;
    std::vector<int> try_count;
    int try_sum = 0;

    //ここにパラメータを置く(いい結果が出たら他のアルゴも出せるようにする
    int values_size = 20;
    std::vector<std::vector<double>> values = {
        {0.114629, 0.535032, 0.457792, 0.641425, 0.443441, 0.38267, 0.72858 },
        {0.114629, 0.535032, 0.457792, 0.641425, 0.443441, 0.683652, 0.72858 },
        {0.138082, 0.317515, 0.207727, 0.169893, 0.481225, 0.348022, 0.51468 },
        {0.123749, 0.391013, 0.234974, 0.343781, 0.500612, 0.61291, 0.967436 },
        {0.114629, 0.739755, 0.457792, 0.246165, 0.443441, 0.683652, 0.843045 },
        {0.0929378, 0.533791, 0.0876191, 0.108085, 0.408152, 0.37141, 0.396507 },
        {0.114629, 0.195881, 0.241557, 0.945, 0.443441, 0.392248, 0.672781 },
        {0.114629, 0.739755, 0.457792, 0.945, 0.386814, 0.658599, 0.534448 },
        {0.148093, 0.350101, 0.234313, 0.343781, 0.500612, 0.530818, 0.822709 },
        {0.123749, 0.846724, 0.17317, 0.343781, 0.500612, 0.461087, 0.828749 },
        {0.114629, 0.3151, 0.457792, 0.246165, 0.470427, 0.683652, 0.72858 },
        {0.103139, 0.226823, 0.0876191, 0.108085, 0.408152, 0.37141, 0.396507 },
        {0.138082, 0.317515, 0.207727, 0.169893, 0.526492, 0.348022, 0.461421 },
        {0.123749, 0.391013, 0.0396838, 0.343781, 0.500612, 0.461087, 0.822709 },
        {0.0315575, 0.533791, 0.257031, 0.223323, 0.408152, 0.37141, 0.568098 },
        {0.0929378, 0.0216476, 0.0876191, 0.0773608, 0.408152, 0.37141, 0.396507 },
        {0.123749, 0.493469, 0.0396838, 0.343781, 0.500612, 0.61291, 0.822709 },
        {0.114629, 0.739755, 0.457792, 0.246165, 0.443441, 0.683652, 0.72858 },
        {0.0624369, 0.846724, 0.106074, 0.196693, 0.500612, 0.461087, 0.822709 },
        {0.114629, 0.752632, 0.457792, 0.861125, 0.379194, 0.487628, 0.672781 }
    };
};



#endif // MONTECARLOWITHALGO_H
