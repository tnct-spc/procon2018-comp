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
    int beam_range = 100;
    int beam_turn = 100;
    int Evaluation_Field(procon::Field field);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
    std::pair<int,int> agent1,agent2;
private:
    procon::Field agentmove(procon::Field field,std::vector<std::vector<std::pair<int,std::pair<int,int>>>> pos);
    std::shared_ptr<ProgresDock> progresdock_beamsearch;
};

#endif // BEAMSEARCH_H
