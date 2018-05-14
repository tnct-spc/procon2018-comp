#ifndef SIMPLEALGORITHM_H
#define SIMPLEALGORITHM_H

#include <queue>

#include "algorithmwrapper.h"
#include "gamemanager.h"

class SimpleAlgorithm : public AlgorithmWrapper
{
public:
    SimpleAlgorithm(std::shared_ptr<GameManager> manager_ptr, const GeneticAgent& agent = GeneticAgent(11));
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);

    double evaluateMove(int side, std::pair<int,int> move);

private:
    GeneticAgent agent_data;
    std::random_device rnd;
    std::mt19937 mt;

    std::uniform_real_distribution<> rand_double;
};

#endif // SIMPLEALGORITHM_H
