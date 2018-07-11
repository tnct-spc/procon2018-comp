#include "evaluateparam.h"

EvaluateParam::EvaluateParam(int side, const procon::Field& field, int final_turn, int agent_num, const GeneticAgent &agent_data) :
    AgentWrapper(side, field, final_turn, agent_num, 1, agent_data)
{

}

double EvaluateParam::evaluateMove(int move, bool is_delete, int now_turn, int eval_side){

    if(eval_side == -1)
        eval_side = side;

    // 莫大なコピーコストがかかってしまう…
    procon::Field copy_field = field;

    const std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    const std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    if(!copy_field.canPut(eval_side, agent, move, false))
        return -300000;

    // 移動前の位置
    std::pair<int,int> now_pos = copy_field.getAgent(eval_side, agent);

    // 移動後の位置と状態
    std::pair<int,int> new_pos = now_pos;
    new_pos.first += x_list.at(move);
    new_pos.second += y_list.at(move);

    // 移動先の(色,配点)
    std::pair<int,int> new_pos_state = copy_field.getState(new_pos.first, new_pos.second);

    // 移動前の(タイルポイント, 領域ポイント) 変数名がクソ
    std::pair<int,int> now_team_point = copy_field.getPoints(eval_side, false);
    std::pair<int,int> now_enemy_point = copy_field.getPoints(eval_side, false);

    // fieldのサイズ
    std::pair<int, int> field_size = copy_field.getSize();

    // fieldの各マスの状態
    std::vector<std::vector<int>> now_field = copy_field.getField();

    std::vector<std::function<double()>> func_vector(param_count);

    // 次の取得タイルポイント
    func_vector.at(0) = [&]{

        return new_pos_state.first;
    };

    // 次の取得領域ポイント
    func_vector.at(1) = [&]{

        return new_pos_state.first;
    };

    // 味方のタイルポイント
    func_vector.at(2) = [&]{

        return now_team_point.first;
    };

    // 味方の領域ポイント
    func_vector.at(3) = [&]{

        return now_team_point.second;
    };

    // 敵のタイルポイント
    func_vector.at(4) = [&]{

        return now_enemy_point.first;
    };

    // 敵の領域ポイント
    func_vector.at(5) = [&]{

        return now_enemy_point.second;
    };

    // 味方エージェントとの距離
    func_vector.at(4) = [&]{

        std::pair<int, int> team_agent = field.getAgent(eval_side, agent == 0 ? 1 : 0);

        int x = now_pos.first - team_agent.first;
        int y = now_pos.second - team_agent.second;

        return sqrt(x * x + y * y);
    };

    // 敵エージェントとの距離
    auto distance = [&]
    {
        std::vector<double> dis;

        for (int i = 0; i < 2; i++) {
            std::pair<int, int> enemy_agent = field.getAgent(eval_side == 0 ? 1 : 0, i);

            int x = now_pos.first - enemy_agent.first;
            int y = now_pos.second - enemy_agent.second;

            dis.push_back(sqrt(x * x + y * y));
        }

        return dis;
    };

    // 近い敵エージェントとの距離
    func_vector.at(5) = [&]{

        std::vector<double> dis = distance();

        return dis.at(0) < dis.at(1) ? 0 : 1;
    };

    // 遠い敵エージェントとの距離
    func_vector.at(6) = [&]{

        std::vector<double> dis = distance();

        return dis.at(0) > dis.at(1) ? 0 : 1;
    };

    // 現在のターン数 / 全体のターン数
    func_vector.at(7) = [&]{

        return now_turn / final_turn;
    };

    // 空きグリッドの数
    func_vector.at(8) = [&]{

        double count = 0;

        for (int y = 0; y < field_size.second; y++) {
            for (int x = 0; x < field_size.first; x++) {
                count += (now_field.at(x).at(y) == 0 ? 1 : 0);
            }
        }

        return count;
    };

    // 空きグリットの総合タイルポイント
    func_vector.at(9) = [&]{

        double count = 0;

        for (int y = 0; y < field_size.second; y++) {
            for (int x = 0; x < field_size.first; x++) {
                if (now_field.at(x).at(y) == 0) {
                    std::pair<int, int> state = copy_field.getState(x, y);
                    count += state.second;
                }
            }
        }

        return count;
    };

    // 空きグリットの総合領域ポイント
    func_vector.at(10) = [&]{

        double count = 0;

        for (int y = 0; y < field_size.second; y++) {
            for (int x = 0; x < field_size.first; x++) {
                if (now_field.at(x).at(y) == 0) {
                    std::pair<int, int> state = copy_field.getState(x, y);
                    count += std::abs(state.second);
                }
            }
        }

        return count;
    };

    double point_sum = 0.0;

    for(auto func : func_vector)
        point_sum += func();

    return point_sum;
}
