#ifndef MAJORITYRULEWITHABSTDATA_H
#define MAJORITYRULEWITHABSTDATA_H


#include "algorithmwrapper.h"
#include "useabstractdata.h"

class MajorityRuleWithAbstData : public AlgorithmWrapper
{
    // using AlgorithmWrapper::AlgorithmWrapper;
public:
    MajorityRuleWithAbstData(const procon::Field& field, int final_turn, bool side);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int now_turn);

private:
    std::vector<std::vector<double>> params = {

    };
    std::vector<GeneticAgent> agents;
};

#endif // MAJORITYRULEWITHABSTDATA_H
