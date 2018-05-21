#include "agentmanager.h"
#include "agents/testdoubleagentalgo.h"

AgentManager::AgentManager(std::shared_ptr<GameManager> manager_ptr, int side, int algorithm_number) :
    AlgorithmWrapper(manager_ptr)
{

    agents.resize(2);

    if(algorithm_number == 0){
        agents.at(0) = std::make_shared<TestDoubleAgentAlgo>(getManagerPtr(), side, 0);
        agents.at(1) = std::make_shared<TestDoubleAgentAlgo>(getManagerPtr(), side, 1);
    }

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> AgentManager::agentAct(int side){
    return std::make_pair(agents.at(0)->agentMove(side), agents.at(1)->agentMove(side));
}
