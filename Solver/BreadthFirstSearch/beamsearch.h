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
    int beam_range = 10;
    int beam_turn = 3;
    int Evaluation_Field(procon::Field field);
    std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
    std::pair<int,int> agent1,agent2;
private:
    procon::Field agentmove(procon::Field field,std::vector<std::vector<std::pair<int,std::pair<int,int>>>> pos);
};

#endif // BEAMSEARCH_H
