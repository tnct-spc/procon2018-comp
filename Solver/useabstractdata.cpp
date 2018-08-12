#include "useabstractdata.h"

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> UseAbstractData::agentAct(int now_turn){

    std::vector<std::vector<std::vector<int>>> abst_values(2);
    std::vector<std::vector<double>> eval_results(2, std::vector<double>(16));

    for(int agent = 0; agent < 2; ++agent)
        abst_values.at(agent) = getAbstractBasedAgent(side, agent);


    int point_sum = 0;
    const std::vector<std::vector<int>>& field_points = field.getValue();
    for(auto raw : field_points)
        for(auto val : raw)
            point_sum += val;

    double point_average = point_sum / (field.getSize().first * field.getSize().second);

    auto calc = [&](std::vector<int>& abst){
        double value = 0;

        double const_tile_count = 0.8;
        double const_empty_count = 1.2;
        double const_side_count = -0.7;
        double const_enemy_side_count = 0.8;
        double const_empty_sum = 1.2;
        double const_side_sum = -0.3;
        double const_enemy_side_sum = 0.8;
        double const_agent_count = -0.8;
        double const_enemy_agent_count = -0.8;

        // [0,1]
        value += 1.0 * abst.at(0) * const_tile_count / (field.getSize().first * field.getSize().second);

        value += 1.0 * abst.at(1) * const_empty_count / abst.at(0);
        value += 1.0 * abst.at(2) * const_side_count / abst.at(0);
        value += 1.0 * abst.at(3) * const_enemy_side_count / abst.at(0);

        // about [-1,1] sum neq 1
        value += 1.0 * abst.at(4) * const_empty_sum / (point_average * abst.at(0));
        value += 1.0 * abst.at(5) * const_side_sum / (point_average * abst.at(0));
        value += 1.0 * abst.at(6) * const_enemy_side_sum / (point_average * abst.at(0));

        value += 1.0 * abst.at(7) * const_agent_count;
        value += 1.0 * abst.at(8) * const_enemy_agent_count;

        return value;
    };

    auto eval = [&](bool agent, int move_int, bool is_delete){

        std::vector<int> move = {x_list.at(move_int), y_list.at(move_int)};
        std::vector<int> values(9, 0);
        int cnt = 0;

        for(int count = 0; count < 4; ++count)
            if((!move_int && count == 3) || ((count * 2 <= move_int) && (count * 2 + 2 >= move_int))){
                for(int index = 0; index < 9; ++index)
                    values.at(index) += abst_values.at(agent).at(count).at(index);
                ++cnt;
            }

        std::vector<int> pos = {field.getAgent(side, agent).first, field.getAgent(side, agent).second};
        for(int index = 0; index < 2; ++index){
            pos.at(index) += move.at(index);
            // out of range
            if(pos.at(index) < 0 || pos.at(index) >= (index ? field.getSize().second : field.getSize().first))
                return -(1e9 + 9);

            int state = field.getState(pos.at(0), pos.at(1)).first;
            // delete move
            if((!state && is_delete) || (state == (side ? 1 : 2) && !is_delete))
                return -(1e9 + 7);
        }

        double value = calc(values) / cnt;

        value *= (16 + field.getState(x_list.at(move_int), y_list.at(move_int)).second);

        return value;
    };

    for(int agent = 0; agent < 2; ++agent)
        for(int move = 0; move < 8; ++move)
            for(int is_delete = 0; is_delete < 2; ++is_delete)
                eval_results.at(agent).at(8 * is_delete + move) = eval(agent, move, is_delete);

    std::vector<std::pair<double, std::pair<int, int>>> moves;

    for(int move_1 = 0; move_1 < 16; ++move_1)
        for(int move_2 = 0; move_2 < 16; ++move_2)
            moves.push_back(std::make_pair(eval_results.at(0).at(move_1) * eval_results.at(1).at(move_2), std::make_pair(move_1, move_2)));

    std::sort(moves.begin(), moves.end(), std::greater<std::pair<double, std::pair<int, int>>>());

    for(auto move_pair : moves){

        std::vector<int> move_index = {move_pair.second.first, move_pair.second.second};
        std::vector<bool> is_delete = {move_index.at(0) > 8, move_index.at(1) > 8};
        std::vector<std::pair<int, int>> bef_pos = {field.getAgent(side, 0), field.getAgent(side, 1)};
        std::vector<std::pair<int, int>> aft_pos = bef_pos;

        for(int agent = 0; agent < 2; ++agent){
            aft_pos.at(agent).first += x_list.at(move_index.at(agent) % 8);
            aft_pos.at(agent).second += y_list.at(move_index.at(agent) % 8);
        }

        if(aft_pos.at(0) != aft_pos.at(1))
            return std::make_pair(std::make_tuple(is_delete.at(0), x_list.at(move_index.at(0) % 8), y_list.at(move_index.at(0) % 8)),
                                  std::make_tuple(is_delete.at(1), x_list.at(move_index.at(1) % 8), y_list.at(move_index.at(1) % 8)));
    }

    return std::make_pair(std::make_tuple(0,0,0), std::make_tuple(0,0,0));
}

// 4方向(右上右下…)の{マスの数,各色タイルの{数,得点の総和},エージェントの存在(0,1)}
// {マス数,数0,数1,数2,総和0,総和1,総和2,存在0,存在1} size:9
// 12
// 43 みたいにする
std::vector<std::vector<int>> UseAbstractData::getAbstractBasedAgent(bool eval_side, bool agent){

    std::vector<int> agent_pos = {field.getAgent(eval_side, agent).first, field.getAgent(eval_side, agent).second};

    /*
    auto calc_distance = [&](std::vector<int> pos){
        return 1.0 * std::max(std::abs(agent_pos.at(0) - pos.at(0)), std::abs(agent_pos.at(1) - pos.at(1)))
            +  0.5 * std::min(std::abs(agent_pos.at(0) - pos.at(0)), std::abs(agent_pos.at(1) - pos.at(1)));
    };
    */
    std::vector<std::vector<int>> return_values(4, std::vector<int>(9, 0));

    return_values.at(0).at(0) = (agent_pos.at(0) + 1) * (agent_pos.at(1) + 1);
    return_values.at(1).at(0) = (field.getSize().first - agent_pos.at(0)) * (agent_pos.at(1) + 1);
    return_values.at(3).at(0) = (agent_pos.at(0) + 1) * (field.getSize().second - agent_pos.at(1));
    return_values.at(2).at(0) = (field.getSize().first - agent_pos.at(0)) * (field.getSize().second - agent_pos.at(1));

    //位置と追加したい要素のインデックス、値を指定する
    auto add_value = [&](std::vector<int> pos, int index, int value){

        if(pos.at(0) <= agent_pos.at(0) && pos.at(1) <= agent_pos.at(1))
            return_values.at(0).at(index) += value;
        if(pos.at(0) >= agent_pos.at(0) && pos.at(1) <= agent_pos.at(1))
            return_values.at(1).at(index) += value;
        if(pos.at(0) <= agent_pos.at(0) && pos.at(1) >= agent_pos.at(1))
            return_values.at(3).at(index) += value;
        if(pos.at(0) >= agent_pos.at(0) && pos.at(1) >= agent_pos.at(1))
            return_values.at(2).at(index) += value;
    };

    std::vector<int> search_pos(2, 0);

    for(;search_pos.at(0) < field.getSize().first; ++search_pos.at(0))
        for(search_pos.at(1) = 0;search_pos.at(1) < field.getSize().second; ++search_pos.at(1)){
            std::pair<int, int> state = field.getState(search_pos.at(0), search_pos.at(1));
            add_value(search_pos, 1 + state.first, 1);
            add_value(search_pos, 4 + state.first, state.second);
        }

   for(int count = 0; count < 4; ++count)
       if(!((eval_side << 1) + agent == count))
           add_value(std::vector<int>({field.getAgent(count >> 1, count & 1).first, field.getAgent(count >> 1, count & 1).second}), 7 + (count >> 1), 1);

    return return_values;
}
