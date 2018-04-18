#ifndef BEAMSEARCH_H
#define BEAMSEARCH_H
#include "field.h"
#include "gamemanager.h"
#include "algorithmwrapper.h"
#include <queue>
#include <bits/stdc++.h>

class beamsearch : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    beamsearch();
    int beam_range;
    int beam_turn;
    int Evaluation_Field(procon::Field field);
    std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
    std::pair<int,int> agent1,agent2;

};

#endif // BEAMSEARCH_H
