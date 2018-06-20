#include "evaluateparam.h"

double EvaluateParam::evaluateMove(int move, bool is_delete){

    const std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    const std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    if(!manager->canPut(side, agent, move, false))
        return -300000;

    // 盤面データ
    procon::Field& field = manager->getField();

    // 移動前の位置
    std::pair<int,int> now_pos = field.getAgent(side, agent);

    // 移動後の位置と状態
    std::pair<int,int> new_pos = now_pos;
    new_pos.first += x_list.at(move);
    new_pos.second += y_list.at(move);

    // 移動先の(色,配点)
    std::pair<int,int> new_pos_state = field.getState(new_pos.first, new_pos.second);

    // 移動前の(タイルポイント, 領域ポイント) 変数名がクソ
    std::pair<int,int> now_team_point = field.getPoints(side, false);
    std::pair<int,int> now_enemy_point = field.getPoints(side, false);

    std::vector<std::function<double()>> func_vector(param_coun：t);

    func_vector.at(0) = [&]{

        return new_pos_state.second;
    };

    double point_sum = 0.0;

    for(auto func : func_vector)
        point_sum += func();

    return point_sum;
}
