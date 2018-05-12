#ifndef SIMPLEALGORITHM_H
#define SIMPLEALGORITHM_H

#include "algorithmwrapper.h"
#include "gamemanager.h"

class SimpleAlgorithm : public AlgorithmWrapper
{
public:
    SimpleAlgorithm(std::shared_ptr<GameManager> manager_ptr, const GeneticAgent& agent = GeneticAgent(10));
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);

    double evaluateMove(int side, std::pair<int,int> move);

private:
    GeneticAgent agent_data;
};

#endif // SIMPLEALGORITHM_H
