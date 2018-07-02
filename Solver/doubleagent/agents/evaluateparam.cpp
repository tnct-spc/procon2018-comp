#include "evaluateparam.h"

EvaluateParam::EvaluateParam(int side, const procon::Field& field, int final_turn, int agent_num, const GeneticAgent &agent_data) :
    AgentWrapper(side, field, final_turn, agent_num, 1, agent_data)
{

}

double EvaluateParam::evaluateMove(int move, bool is_delete){

    // 莫大なコピーコストがかかってしまう…
    procon::Field copy_field = field;

    const std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    const std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    if(!copy_field.canPut(side, agent, move, false))
        return -300000;

    // 移動前の位置
    std::pair<int,int> now_pos = copy_field.getAgent(side, agent);

    // 移動後の位置と状態
    std::pair<int,int> new_pos = now_pos;
    new_pos.first += x_list.at(move);
    new_pos.second += y_list.at(move);

    // 移動先の(色,配点)
    std::pair<int,int> new_pos_state = copy_field.getState(new_pos.first, new_pos.second);

    // 移動前の(タイルポイント, 領域ポイント) 変数名がクソ
    std::pair<int,int> now_team_point = copy_field.getPoints(side, false);
    std::pair<int,int> now_enemy_point = copy_field.getPoints(side, false);

    std::vector<std::function<double()>> func_vector(param_count);

    func_vector.at(0) = [&]{

        return new_pos_state.second;
    };

    double point_sum = 0.0;

    for(auto func : func_vector)
        point_sum += func();

    return point_sum;
}
