#ifndef BEAMSEARCH_H
#define BEAMSEARCH_H
#include "field.h"
#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "visualizer.h"
#include <queue>
#include <algorithm>
#include <iostream>

class beamsearch : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    beamsearch();
    const int beam_range = 1000;
    const int beam_turn = 10;
    int Evaluation_Field(procon::Field field,int side);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct();
    std::pair<int,int> agent1,agent2;
    const double var1=0,var2=0,var3=0,var4=0,var5=0;
private:
    procon::Field agentmove(procon::Field field,std::vector<std::vector<std::pair<int,std::pair<int,int>>>> pos);
    std::shared_ptr<ProgresDock> progresdock_beamsearch;
};

#endif // BEAMSEARCH_H
