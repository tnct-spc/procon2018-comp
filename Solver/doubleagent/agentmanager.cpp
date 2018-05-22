#include "agentmanager.h"
#include "agents/testdoubleagentalgo.h"

AgentManager::AgentManager(std::shared_ptr<GameManager> manager_ptr, int side, int algorithm_number) :
    AlgorithmWrapper(manager_ptr)
{

    agents.resize(2);

    std::shared_ptr<TestDoubleAgentAlgo> hoge;
    hoge = std::make_shared<TestDoubleAgentAlgo>(side, 0, manager_ptr);

    if(algorithm_number == 0){
        agents.at(0) = std::make_shared<TestDoubleAgentAlgo>(side, 0, getManagerPtr());
        agents.at(1) = std::make_shared<TestDoubleAgentAlgo>(side, 1, getManagerPtr());
    }

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> AgentManager::agentAct(int side){
    std::vector<std::pair<double,std::tuple<int,int,int>>> move_1 = agents.at(0)->agentMove();
    std::vector<std::pair<double,std::tuple<int,int,int>>> move_2 = agents.at(1)->agentMove();
}
