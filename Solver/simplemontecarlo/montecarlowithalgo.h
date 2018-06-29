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
#include <set>

class MontecarloWithAlgo : public AlgorithmWrapper
{
    //深さ1のノード(81パターン)を生成してUCBでやる
    //二手目以降はお互いにGeneticAlgoからの厳選パターン(10種程度)を適用する

    //using AlgorithmWrapper::AlgorithmWrapper;

public:
    MontecarloWithAlgo(const procon::Field& field, int final_turn, bool side);

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


    //ここにパラメータを置く(いい結果が出たら他のアルゴも出せるようにする
    int values_size = 20;

    std::vector<std::vector<double>> values = {
        { 0.140183, 0.65865, 0.281883, 0.208004, 0.328724, 0.72446, 0.744416 },
        { 0.0835999, 0.832337, 0.424252, 0.133784, 0.41572, 0.565226, 0.950078 },
        { 0.0835999, 0.832337, 0.424252, 0.199968, 0.41572, 0.565226, 0.950078 },
        { 0.0835999, 0.832337, 0.424252, 0.133784, 0.41572, 0.565226, 0.830148 },
        { 0.14837, 0.952218, 0.32119, 0.149178, 0.510368, 0.461491, 0.842321 },
        { 0.0179378, 0.889089, 0.136547, 0.133784, 0.41572, 0.565226, 0.89526 },
        { 0.00293812, 0.809793, 0.439889, 0.270579, 0.459446, 0.488728, 0.905059 },
        { 0.140183, 0.715716, 0.281883, 0.0537743, 0.328724, 0.72446, 0.842321 },
        { 0.23115, 0.810836, 0.628294, 0.0994511, 0.237475, 0.606622, 0.821908 },
        { 0.140183, 0.715716, 0.281883, 0.208004, 0.328724, 0.72446, 0.842321 },
        { 0.0179378, 0.889089, 0.136547, 0.133784, 0.544001, 0.565226, 0.89526 },
        { 0.270962, 0.810836, 0.628294, 0.0994511, 0.402688, 0.606622, 0.821908 },
        { 0.0179378, 0.889089, 0.136547, 0.133784, 0.544001, 0.565226, 0.89526 },
        { 0.0835999, 0.832337, 0.424252, 0.133784, 0.41572, 0.565226, 0.950078 },
        { 0.287496, 0.971228, 0.32119, 0.149178, 0.510368, 0.461491, 0.842321 },
        { 0.0835999, 0.832337, 0.424252, 0.133784, 0.41572, 0.565226, 0.950078 },
        { 0.262301, 0.805416, 0.32119, 0.149178, 0.510368, 0.461491, 0.842321 },
        { 0.270962, 0.810836, 0.767541, 0.288439, 0.402688, 0.606622, 0.821908 },
        { 0.14837, 0.971228, 0.32119, 0.149178, 0.510368, 0.461491, 0.842321 },
        { 0.14837, 0.882086, 0.32119, 0.149178, 0.510368, 0.461491, 0.842321 }
    };

    /*
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
    */
};



#endif // MONTECARLOWITHALGO_H
