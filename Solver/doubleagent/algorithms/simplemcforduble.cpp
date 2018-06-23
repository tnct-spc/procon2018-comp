#include "simplemcforduble.h"

SimpleMCForDuble::SimpleMCForDuble(std::shared_ptr<GameManager> manager, std::vector<std::shared_ptr<AgentWrapper>> agents, int side) :
    manager(manager),
    side(side),
    field(manager->getField()),
    agents(agents)
{
}

void SimpleMCForDuble::changeTurn(){

    std::vector<std::vector<std::pair<double,std::tuple<int,int,int>>>> can_move_list;

    for(int index = 0; index < 2; ++index)
        can_move_list.at(index).push_back(std::make_pair(0.0, std::make_tuple(0, 0, 0)));

    auto agent_move = [&](int agent){

        std::pair<int,int> agent_pos = field.getAgent(side, agent);

        auto evaluate = [&](int count, bool is_delete){

            double value = agents.at(agent)->evaluateMove(count, is_delete);

            if(value > minus_bound)//置けないパターンがあるのでそれを切る
                can_move_list.at(agent).push_back(std::make_pair(std::pow(value - minus_bound, value_weight), std::make_tuple(is_delete + 1, x_list.at(count), y_list.at(count))));

        };

        for(int count = 0; count < 9; ++count){

            std::pair<int,int> new_pos = agent_pos;
            new_pos.first += x_list.at(count);
            new_pos.second += y_list.at(count);

            if(new_pos.first < 0 || new_pos.first >= field.getSize().first || new_pos.second < 0 || new_pos.second >= field.getSize().second)
                continue;

            int state = field.getState(new_pos.first, new_pos.second).first;

            if(state != (side ? 1 : 2))
                evaluate(count, false);
            if(state && count != 8)
                evaluate(count, true);
        }

    };

    agent_move(0);
    agent_move(1);

}

std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> SimpleMCForDuble::calcMove(){

}
