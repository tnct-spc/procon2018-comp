#ifndef LASTREGION_H
#define LASTREGION_H
#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "field.h"
#include <stack>
#include <algorithm>
#include <vector>
#include <queue>
#include <iostream>

class LastRegion : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:


    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int);
    void setParams(std::vector<std::pair<QString, double>> params);
    void setRandomParams(std::vector<std::pair<QString, double>> params,bool flag);

private:

    const static std::vector<int> dx,dy;
    const int move_count = 3;
    const int max_turn_count = 9;

};
#endif // LASTREGION_H
