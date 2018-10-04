#ifndef GENETICALGO_H
#define GENETICALGO_H

#include "algorithmwrapper.h"
#include "gamemanager.h"
#include "geneticagent.h"

class GeneticAlgo : public AlgorithmWrapper
{
    // using AlgorithmWrapper::AlgorithmWrapper;
public:
    GeneticAlgo(const procon::Field& field, int final_turn, bool side, const GeneticAgent& agent = GeneticAgent(7));
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int now_turn);
    void setParams(std::vector<std::pair<QString, double>> params);

private:
    GeneticAgent agent_data;

    //こっちはパラメータの通りに動くやつ
};

#endif // GENETICALGO_H
