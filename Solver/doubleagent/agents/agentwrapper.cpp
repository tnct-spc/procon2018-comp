#include "agentwrapper.h"

AgentWrapper::AgentWrapper(int side, int agent_num, std::shared_ptr<GameManager> manager_ptr, int param_count, const GeneticAgent& agent_data) :
    side(side),
    agent(agent_num),
    manager(manager_ptr),
    agent_data(agent_data)
{
    if(this->agent_data.size != param_count)
        this->agent_data = GeneticAgent(param_count, 2);

}

const std::vector<std::pair<double, std::tuple<int,int,int>>> AgentWrapper::agentMove(){

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::vector<std::pair<double,std::tuple<int,int,int>>> return_val;

    return_val.push_back(std::make_pair(-200000, std::make_tuple(0, 0, 0)));


    procon::Field& field = manager->getField();
    std::pair<int,int> agent_pos = field.getAgent(side, agent);

    auto evaluate = [&](int count, bool is_delete){
        double value = evaluateMove(count, is_delete);

        if(value > -200000)//置けないパターンがあるのでそれを切る
            return_val.push_back(std::make_pair(value, std::make_tuple(is_delete + 1, x_list.at(count), y_list.at(count))));

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

    //昇順ソート
    sort(return_val.begin(), return_val.end(), std::greater<std::pair<double,std::tuple<int,int,int>>>());

    return std::move(return_val);

}

void AgentWrapper::setAgent(const GeneticAgent &agent){
    agent_data = agent;
}
