#ifndef TESTALGORITHMPLAYOUT_H
#define TESTALGORITHMPLAYOUT_H

#include "gamemanager.h"
#include "useabstractdata.h"

// クソ
struct params;

class TestAlgorithmPlayout
{
public:
    TestAlgorithmPlayout();


    void run();

private:
    int playout(params& param_1, params& param_2);

    std::random_device device;
    std::mt19937 mt;

    std::uniform_int_distribution<> rand_size;
    std::uniform_int_distribution<> rand_turn;

    std::shared_ptr<GameManager> manager;

    std::vector<std::shared_ptr<UseAbstractData>> agents;

};

struct params{

	// クソ記法
    params(std::vector<double>& values, double diagonal, std::function<void(double&, bool, int)>& tile_value_func, std::function<double(double, double)>& eval_sum_func) :
        values(values),
        diagonal(diagonal),
        tile_value_func(tile_value_func),
        eval_sum_func(eval_sum_func){}

    std::vector<double> values;
    double diagonal;
    std::function<void(double&, bool, int)> tile_value_func;
    std::function<double(double, double)> eval_sum_func;
};

#endif // TESTALGORITHMPLAYOUT_H
