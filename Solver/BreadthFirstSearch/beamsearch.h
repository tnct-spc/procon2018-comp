#ifndef BEAMSEARCH_H
#define BEAMSEARCH_H
#include "field.h"
#include "gamemanager.h"
#include "algorithmwrapper.h"

class beamsearch : public AlgorithmWrapper
{

public:
    beamsearch();
    int beam_range;
    int Evaluation_Field(procon::Field field,int turn);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
    std::pair<int,int> agent1,agent2;
};

#endif // BEAMSEARCH_H
