#include "agentmanager.h"

AgentManager::AgentManager(std::shared_ptr<GameManager> manager_ptr, int algorithm_number) :
    AlgorithmWrapper(manager_ptr)
{
    // if(algorithm_number == 0)

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> AgentManager::agentAct(int){
    return std::make_pair(agents.at(0)->agentMove(side), agents.at(1)->agentMove(side));
}
