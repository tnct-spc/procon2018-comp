#ifndef SIMPLEALGORITHM_H
#define SIMPLEALGORITHM_H

#include <queue>

#include "algorithmwrapper.h"
#include "gamemanager.h"

class SimpleAlgorithm : public AlgorithmWrapper
{
public:
    SimpleAlgorithm(const procon::Field& field, int final_turn, bool side, const GeneticAgent& agent = GeneticAgent(11));
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
    void setParams(std::vector<std::pair<QString, double>> params);

    double evaluateMove(int side, std::pair<int,int> move);

private:
    GeneticAgent agent_data;
    std::random_device rnd;
    std::mt19937 mt;

    std::uniform_real_distribution<> rand_double;
};

#endif // SIMPLEALGORITHM_H
