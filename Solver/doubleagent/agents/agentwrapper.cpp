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


    for(int count = 0; count < 9; ++count){
        std::pair<double,bool> value = evaluateMove(count);

        if(value.first > -200000)//置けないパターンがあるのでそれを着る
            return_val.push_back(std::make_pair(value.first, std::make_tuple(value.second + 1, x_list.at(count), y_list.at(count))));
    }
    //昇順ソート
    sort(return_val.begin(), return_val.end(), std::greater<std::pair<double,std::tuple<int,int,int>>>());

    return std::move(return_val);

}

void AgentWrapper::setAgent(const GeneticAgent &agent){
    agent_data = agent;
}
