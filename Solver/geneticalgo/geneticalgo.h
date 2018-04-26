#ifndef GENETICALGO_H
#define GENETICALGO_H

#include "algorithmwrapper.h"
#include "gamemanager.h"
#include "geneticagent.h"

class GeneticAlgo : public AlgorithmWrapper
{
    // using AlgorithmWrapper::AlgorithmWrapper;
public:

    GeneticAlgo(std::shared_ptr<GameManager> manager_ptr);
    void setAgentData(GeneticAgent inp);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);

private:
    GeneticAgent agent_data;

    //こっちはパラメータの通りに動くやつ
};

#endif // GENETICALGO_H
