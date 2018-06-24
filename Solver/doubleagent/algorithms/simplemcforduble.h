#ifndef SIMPLEMCFORDUBLE_H
#define SIMPLEMCFORDUBLE_H

#include <future>

#include "gamemanager.h"
#include "doubleagent/agents/agentwrapper.h"

class SimpleMCForDuble
{
public:
    SimpleMCForDuble(std::shared_ptr<GameManager> manager, std::vector<std::shared_ptr<AgentWrapper>> agents, int side);
    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> calcMove();

private:
    std::shared_ptr<GameManager> manager;
    std::vector<std::shared_ptr<AgentWrapper>> agents;

    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> changeTurn(std::shared_ptr<GameManager> manager_ptr);

    // 探索候補に含める最小値(これ以下は探索しない)
    const double minus_bound = -5000;
    // 重みにかける累乗の(大きいほど評価の高い値を優先的に探索する)
    const double value_weight = 1.0;

    // 計算に書ける時間(sec)
    const double calc_time = 2.0;

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::random_device rnd;
    std::mt19937 mt;

    int side;
    int cpu_num;

    // ここでFieldのコピーをしてしまう (プレイアウト回数)だけコピーをしてしまうのは仕方ないです
    procon::Field field;
};

#endif // SIMPLEMCFORDUBLE_H
