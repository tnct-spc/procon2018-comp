#ifndef SIMPLEMCFORDUBLE_H
#define SIMPLEMCFORDUBLE_H

#include "gamemanager.h"
#include "doubleagent/agents/agentwrapper.h"
#include <future>
#include <memory>

class SimpleMCForDuble
{
public:
    SimpleMCForDuble(const procon::Field& field, bool side, int now_turn, int max_turn, std::vector<std::shared_ptr<AgentWrapper>> agents);
    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> calcMove();

private:
    std::mutex mtx;

    const procon::Field &field;
    std::vector<std::shared_ptr<AgentWrapper>> agents;

    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> changeTurn(std::shared_ptr<GameManager> manager_ptr);

    // 探索候補に含める最小値(これ以下は探索しない)
    const double minus_bound = -5000;

    // 重みにかける倍率
    const double value_ratio = 1.0;
    // 重みにかける累乗の値(大きいほど評価の高い値を優先的に探索する)
    const double value_weight = 1.0;

    // 計算に書ける時間(sec)
    const double calc_time = 2.0;

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::random_device rnd;
    std::mt19937 mt;

    bool side;
    int now_turn, max_turn, cpu_num;

    // ここでFieldのコピーをしてしまう (プレイアウト回数)だけコピーをしてしまうのは仕方ないです
    std::vector<std::shared_ptr<GameManager>> manager_vec;
};

#endif // SIMPLEMCFORDUBLE_H
