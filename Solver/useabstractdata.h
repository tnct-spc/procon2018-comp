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

    /*
    std::vector<int> x_list = {-1, 0, 1, 1, 1, 0, -1, -1};
    std::vector<int> y_list = {1, 1, 1, 0, -1, -1, -1, 0};
    */

    std::vector<double> const_values = {1.6, 1.2, -0.7, 1.2, 1.2, -0.3 ,1.2, -0.4, -0.4};

    double diagonal_move = 1.5;

    std::function<void(double&, bool, int)> calc_value_func = [](double& eval_result, bool is_return, int tile_value){

        if(is_return)
            eval_result *= 12;
        else
            eval_result *= (16 + tile_value);
    };

    std::function<double(double, double)> calc_eval_sum = [](double inp_1, double inp_2){
        return inp_1 + inp_2;
    };

    std::vector<int> x_list = {-1, -1, 0, 1, 1, 1, 0, -1};
    std::vector<int> y_list = {0, -1, -1, -1, 0, 1, 1, 1};
    /*

      12
      43

      111 333
      4 222 444

      */
};

#endif // USEABSTRACTDATA_H
