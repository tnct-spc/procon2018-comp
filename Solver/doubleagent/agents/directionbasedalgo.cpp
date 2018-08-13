#include "directionbasedalgo.h"

DirectionBasedAlgo::DirectionBasedAlgo(int side, const procon::Field& field, int final_turn, int agent_num, const GeneticAgent& agent_data) :
    AgentWrapper(side, field, final_turn, agent_num, 9, agent_data)
{

    const std::vector<int> x_l = {-1, -1, 0, 1, 1, 1, 0, -1, 0};
    const std::vector<int> y_l = {0, -1, -1, -1, 0, 1, 1, 1, 0};

    move_map.resize(3, std::vector<int>(3));

    for(int index = 0; index < 9; ++index)
        move_map.at(x_l.at(index) + 1).at(y_l.at(index) + 1) = index;

}

double DirectionBasedAlgo::evaluateMove(int move, bool is_delete, int now_turn, int eval_side){

    const std::vector<double>& data = agent_data.getData();

    if(eval_side == -1)
        eval_side = side;

    std::pair<int,int> bef_pos = field.getAgent(eval_side, agent);
    std::pair<int,int> aft_pos = bef_pos;
    aft_pos.first += x_list.at(move);
    aft_pos.first += y_list.at(move);

    std::pair<int,int> state = field.getState(aft_pos.first, aft_pos.second);

    if((state.first == (side ? 2 : 1)) ^ is_delete)
        return -300000;

    return 0.0;
}


std::vector<std::vector<int>> DirectionBasedAlgo::getAbstractBasedAgent(bool eval_side, bool eval_agent){

    std::vector<int> eval_agent_pos = {field.getAgent(eval_side, eval_agent).first, field.getAgent(eval_side, eval_agent).second};

    std::vector<std::vector<int>> return_values(4, std::vector<int>(9, 0));

    return_values.at(0).at(0) = (eval_agent_pos.at(0) + 1) * (eval_agent_pos.at(1) + 1);
    return_values.at(1).at(0) = (field.getSize().first - eval_agent_pos.at(0)) * (eval_agent_pos.at(1) + 1);
    return_values.at(3).at(0) = (eval_agent_pos.at(0) + 1) * (field.getSize().second - eval_agent_pos.at(1));
    return_values.at(2).at(0) = (field.getSize().first - eval_agent_pos.at(0)) * (field.getSize().second - eval_agent_pos.at(1));

    //位置と追加したい要素のインデックス、値を指定する
    auto add_value = [&](std::vector<int> pos, int index, int value){

        if(pos.at(0) <= eval_agent_pos.at(0) && pos.at(1) <= eval_agent_pos.at(1))
            return_values.at(0).at(index) += value;
        if(pos.at(0) >= eval_agent_pos.at(0) && pos.at(1) <= eval_agent_pos.at(1))
            return_values.at(1).at(index) += value;
        if(pos.at(0) <= eval_agent_pos.at(0) && pos.at(1) >= eval_agent_pos.at(1))
            return_values.at(3).at(index) += value;
        if(pos.at(0) >= eval_agent_pos.at(0) && pos.at(1) >= eval_agent_pos.at(1))
            return_values.at(2).at(index) += value;
    };

    std::vector<int> search_pos(2, 0);

    for(;search_pos.at(0) < field.getSize().first; ++search_pos.at(0))
        for(search_pos.at(1) = 0;search_pos.at(1) < field.getSize().second; ++search_pos.at(1)){
            std::pair<int, int> state = field.getState(search_pos.at(0), search_pos.at(1));

            if(eval_side && state.first)
                state.first = (state.first == 1 ? 2 : 1);

            add_value(search_pos, 1 + state.first, 1);
            add_value(search_pos, 4 + state.first, state.second);
        }

   for(int count = 0; count < 4; ++count)
       if(!((eval_side << 1) + eval_agent == count))
           add_value(std::vector<int>({field.getAgent(count >> 1, count & 1).first, field.getAgent(count >> 1, count & 1).second}), 7 + (eval_side ^ (count >> 1)), 1);

    return return_values;
}
