#ifndef LASTFORCE_H
#define LASTFORCE_H
#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "field.h"
#include "precompile.h"

class LastForce : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:

    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int);

private:
    bool checkMate = false;
    bool calc(procon::Field field,int depth);
    procon::Field moveAgent(procon::Field field, std::vector<int> act);

    std::pair<int,int> ans;

    int now_turn;
};

#endif // LASTFORCE_H
