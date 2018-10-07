#ifndef LASTREGION_H
#define LASTREGION_H
#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "field.h"
#include <stack>


class LastRegion : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:


    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int);
    void setParams(std::vector<std::pair<QString, double>> params);
    bool EdgeCut = true;

private:
    bool checkMate = false;
    bool calc(procon::Field field,int depth,int a, int b);
    procon::Field moveAgent(procon::Field field, std::vector<int> act);

    std::pair<int,int> ans;
    std::vector<std::vector<int>> winner;
    int coun = 0;

    const static std::vector<int> dx,dy;
};
#endif // LASTREGION_H
