#include "geneticalgo.h"

GeneticAlgo::GeneticAlgo(std::shared_ptr<GameManager> manager_ptr) :
    AlgorithmWrapper(manager_ptr)
{

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> GeneticAlgo::agentAct(int side)
{

}

void GeneticAlgo::setAgentData(GeneticAgent inp){
    agent_data = inp;
}
