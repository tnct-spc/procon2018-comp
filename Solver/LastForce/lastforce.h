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
    bool calc(procon::Field field,int depth,int a, int b);
    procon::Field moveAgent(procon::Field field, std::vector<int> act);

    std::pair<int,int> ans;
    std::vector<std::vector<int>> winner;
};

#endif // LASTFORCE_H
