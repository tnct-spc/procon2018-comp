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
    bool calc(procon::Field field, int depth);
    procon::Field moveAgent(std::shared_ptr<procon::Field> field, std::vector<int> act);
};

#endif // LASTFORCE_H
