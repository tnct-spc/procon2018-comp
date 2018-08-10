#include "useabstractdata.h"

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> UseAbstractData::agentAct(int now_turn){

    for(int agent = 0; agent < 2; ++agent){
        std::vector<std::vector<int>> values = getAbstractBasedAgent(0, agent);
        for(auto vec : values){
            for(auto val : vec)
                std::cout << val << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    return std::make_pair(std::make_tuple(0,0,0), std::make_tuple(0,0,0));
}

// 4方向(右上右下…)の{マスの数,各色タイルの{数,得点の総和},エージェントの存在(0,1)}
// {マス数,数0,数1,数2,総和0,総和1,総和2,存在0,存在1} size:9
std::vector<std::vector<int>> UseAbstractData::getAbstractBasedAgent(bool side, bool agent){

    std::vector<int> agent_pos = {field.getAgent(side, agent).first, field.getAgent(side, agent).second};
    std::vector<int> search_pos(2, 0);

    /*
    auto calc_distance = [&](std::vector<int> pos){
        return 1.0 * std::max(std::abs(agent_pos.at(0) - pos.at(0)), std::abs(agent_pos.at(1) - pos.at(1)))
            +  0.5 * std::min(std::abs(agent_pos.at(0) - pos.at(0)), std::abs(agent_pos.at(1) - pos.at(1)));
    };
    */
    std::vector<std::vector<int>> return_values(4, std::vector<int>(9, 0));

    return_values.at(0).at(0) = (agent_pos.at(0) + 1) * (agent_pos.at(1) + 1);
    return_values.at(0).at(1) = (agent_pos.at(0) + 1) * (field.getSize().second - agent_pos.at(1));
    return_values.at(0).at(2) = (field.getSize().first - agent_pos.at(0)) * (agent_pos.at(1) + 1);
    return_values.at(0).at(3) = (field.getSize().first - agent_pos.at(0)) * (field.getSize().second - agent_pos.at(1));

    //位置と追加したい要素のインデックス、値を指定する
    auto add_value = [&](std::vector<int> pos, int index, int value){

        if(pos.at(0) >= agent_pos.at(0) && pos.at(1) >= agent_pos.at(1))
            return_values.at(0).at(index) += value;
        if(pos.at(0) >= agent_pos.at(0) && pos.at(1) <= agent_pos.at(1))
            return_values.at(1).at(index) += value;
        if(pos.at(0) <= agent_pos.at(0) && pos.at(1) >= agent_pos.at(1))
            return_values.at(2).at(index) += value;
        if(pos.at(0) <= agent_pos.at(0) && pos.at(1) <= agent_pos.at(1))
            return_values.at(3).at(index) += value;
    };

    for(;search_pos.at(0) < field.getSize().first; ++search_pos.at(0))
        for(;search_pos.at(1) < field.getSize().second; ++search_pos.at(1)){
            std::pair<int, int> state = field.getState(search_pos.at(0), search_pos.at(1));
            add_value(search_pos, 1 + state.first, 1);
            add_value(search_pos, 4 + state.first, state.second);
        }

   for(int count = 0; count < 4; ++count)
       add_value(std::vector<int>({field.getAgent(count & 2, count & 1).first, field.getAgent(count & 2, count & 1).second}), 7 + (count >> 1), 1);

    return return_values;
}
