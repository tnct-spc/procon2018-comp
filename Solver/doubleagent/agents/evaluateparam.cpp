#include "evaluateparam.h"

EvaluateParam::EvaluateParam(int side, procon::Field& field, int final_turn, int agent_num, const GeneticAgent &agent_data) :
    AgentWrapper(side, field, final_turn, agent_num, 1, agent_data)
{

}

double EvaluateParam::evaluateMove(int move, bool is_delete, int now_turn, int eval_side){

    if(eval_side == -1)
        eval_side = side;

    const std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    const std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    if(!field.canPut(eval_side, agent, move, false))
        return -300000;

    // 移動前の位置
    std::pair<int,int> now_pos = field.getAgent(eval_side, agent);

    // 移動後の位置と状態
    std::pair<int,int> new_pos = now_pos;
    new_pos.first += x_list.at(move);
    new_pos.second += y_list.at(move);

    // 移動先の(色,配点)
    std::pair<int,int> new_pos_state = field.getState(new_pos.first, new_pos.second);

    // 現在の得点(領域点は考慮しない！)
    std::vector<std::pair<int,int>> point = field.getPoints(false);


    // fieldのサイズ
    std::pair<int, int> field_size = field.getSize();

    std::vector<std::function<double()>> func_vector;

    // 各要素ごとに評価値がおよそ[-300,300]程度に収まるようにする

    // 敵エージェントとの距離
    std::vector<double> dis;

    for (int i = 0; i < 2; i++) {
        std::pair<int, int> enemy_agent = field.getAgent(eval_side == 0 ? 1 : 0, i);

        int x = now_pos.first - enemy_agent.first;
        int y = now_pos.second - enemy_agent.second;

        dis.push_back(sqrt(x * x + y * y));
    }

    if(use_region){

        // 移動後の取得ポイント
        std::vector<std::pair<int,int>> post_point = field.getPoints(std::make_pair(std::make_pair(side, is_delete), new_pos), false);
        // 自分の取得タイルポイント
        func_vector.push_back([&]{
            return 18.5 * (post_point.at(eval_side).second - point.at(eval_side).second);
        });

        // 自分の取得領域ポイント
        func_vector.push_back([&]{
            return 18.5 * (post_point.at(eval_side).second - point.at(eval_side).second);
        });


        // 相手の取得領域ポイント
        func_vector.push_back([&]{
            return 7.5 * (post_point.at(!eval_side).second - point.at(!eval_side).second);
        });
    }else{
        // 自分の得点変化量
        func_vector.push_back([&]{
            return (new_pos_state.first == side + 1 ? (is_delete ? -new_pos_state.second : 0): new_pos_state.second);
        });
    }

    // 味方エージェントとの距離
    func_vector.push_back([&]{

        std::pair<int, int> team_agent = field.getAgent(eval_side, agent == 0 ? 1 : 0);

        int x = now_pos.first - team_agent.first;
        int y = now_pos.second - team_agent.second;

        return sqrt(x * x + y * y);
    });

    // 近い敵エージェントとの距離
    func_vector.push_back([&]{

        return dis.at(0) < dis.at(1) ? dis.at(0) : dis.at(1);
    });

    // 遠い敵エージェントとの距離
    func_vector.push_back([&]{

        return dis.at(0) > dis.at(1) ? dis.at(0) : dis.at(1);
    });

    // 現在のターン数 / 全体のターン数
    func_vector.push_back([&]{

        return now_turn / final_turn;
    });

    // 空きグリッドの数
    func_vector.push_back([&]{

        double count = 0;

        for (int y = 0; y < field_size.second; y++) {
            for (int x = 0; x < field_size.first; x++) {
                count += (field.getState(x, y).first == 0 ? 1 : 0);
            }
        }

        return count;
    });

    // 空きグリットの総合タイルポイント
    func_vector.push_back([&]{

        double count = 0;

        for (int y = 0; y < field_size.second; y++) {
            for (int x = 0; x < field_size.first; x++) {
                if (field.getState(x, y).first == 0) {
                    std::pair<int, int> state = field.getState(x, y);
                    count += state.second;
                }
            }
        }

        return count;
    });

    // 空きグリットの総合領域ポイント
    func_vector.push_back([&]{

        double count = 0;

        for (int y = 0; y < field_size.second; y++) {
            for (int x = 0; x < field_size.first; x++) {
                if (field.getState(x, y).first == 0) {
                    std::pair<int, int> state = field.getState(x, y);
                    count += std::abs(state.second);
                }
            }
        }

        return count;
    });

    // もし進む先が相手のタイルの場合、除去したときの相手チームのタイルポイントにおける減点
    func_vector.at(13) = [&]{

        return post_point.at(side == 0 ? 1 : 0).first - point.at(side == 0 ? 1 : 0).first;
    };

    // もし進む先が相手のタイルの場合、除去したときの相手チームの領域ポイントにおける減点
    func_vector.at(14) = [&]{

        return post_point.at(side == 0 ? 1 : 0).second - point.at(side == 0 ? 1 : 0).second;
    };

    double point_sum = 0.0;

    int param_count = func_vector.size();

    const std::vector<double>& data = agent_data.getData();

    for(int index = 0; index < param_count; ++index)
        point_sum += data.at(index) * func_vector.at(index)();

    return point_sum;
}
