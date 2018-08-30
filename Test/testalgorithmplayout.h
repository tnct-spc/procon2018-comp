#ifndef TESTALGORITHMPLAYOUT_H
#define TESTALGORITHMPLAYOUT_H

#include "gamemanager.h"
#include "useabstractdata.h"

#define SPDLOG_EOL ""

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include <random>

// クソ
struct params;

class TestAlgorithmPlayout
{
public:
    TestAlgorithmPlayout();


    void run();

private:
    int playout(bool iswrite = false);

    std::shared_ptr<spdlog::logger> logger;

    std::random_device device;
    std::mt19937 mt;

    const std::string path = "../../procon2018-comp/out";

    std::uniform_int_distribution<> rand_size;
    std::uniform_int_distribution<> rand_turn;
    std::uniform_int_distribution<> rand_dist;

    std::shared_ptr<GameManager> manager;

    std::vector<std::shared_ptr<UseAbstractData>> agents;

    GeneticAgent agent, enemy;

    std::vector<std::vector<double>> params = {
        {0.66, 0.62, 0.43, 0.62, 0.62, 0.47 ,0.62, 0.46, 0.46, 0.65},
        {0.585501, 0.936013, 0.750115, 0.136817, 0.649388, 0.589486, 0.882378, 0.090591, 0.946676, 0.339145},
        {0.424304, 0.870152, 0.969697, 0.994353, 0.79771, 0.339376, 0.567848, 0.212827, 0.662733, 0.526067},
        {0.751296, 0.665754, 0.556531, 0.205988, 0.606986, 0.87203, 0.808129, 0.282799, 0.703115, 0.371682},
        {0.659504, 0.610229, 0.103188, 0.237939, 0.979475, 0.337169, 0.937114, 0.351038, 0.584697, 0.322641},
        {0.787281, 0.883428, 0.667984, 0.828937, 0.868376, 0.101065, 0.72732, 0.130173, 0.862691, 0.387112},
        {0.959878, 0.891539, 0.705233, 0.963502, 0.921754, 0.789983, 0.92909, 0.137433, 0.685138, 0.297063}
    };

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
