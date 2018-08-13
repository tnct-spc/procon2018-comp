#ifndef USEABSTRACTDATA_H
#define USEABSTRACTDATA_H

#include "algorithmwrapper.h"

#include <algorithm>

class UseAbstractData : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int now_turn);

private:
    std::vector<std::vector<int>> getAbstractBasedAgent(bool side, bool agent);

	// 盤面状態の評価に用いる倍率
	/*
	  先頭要素から
	  ・その方向にあるタイルの数 / 全体のタイルの数
	  ・{何も置いてない,自タイルのある,敵タイルのある}マスの数の割合(3要素の総和が1になる)
	  ・{何も置いてない,自タイルのある,敵タイルのある}マスの得点の合計を平均値で割ったもの(3要素の総和がほぼ1になる)
	  ・{自,敵}エージェントの数(自分自身は含まない)

	  このそれぞれに対して一定の倍率をかけて、その総和を評価値にする

	*/
    std::vector<double> const_values = {1.6, 1.2, -0.7, 1.2, 1.2, -0.3 ,1.2, -0.4, -0.4};

	// 斜め方向に移動する場合にかける倍率
    double diagonal_move = 1.5;

	// タイルの得点を評価値に反映させる関数
    std::function<void(double&, bool, int)> calc_value_func = [](double& eval_result, bool is_return, int tile_value){

        if(is_return)
            eval_result *= 12;
        else
            eval_result *= (16 + tile_value);
    };

	// 各エージェントの評価値から行動のペアの評価値を決める関数
    std::function<double(double, double)> calc_eval_sum = [](double inp_1, double inp_2){
        return inp_1 + inp_2;
    };


    const std::vector<int> x_list = {-1, -1, 0, 1, 1, 1, 0, -1};
    const std::vector<int> y_list = {0, -1, -1, -1, 0, 1, 1, 1};
};

#endif // USEABSTRACTDATA_H
