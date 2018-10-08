#ifndef LASTREGION_H
#define LASTREGION_H
#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "field.h"
#include <stack>
#include <algorithm>
#include <vector>
#include <utility>
#include <random>
#include <queue>
#include <complex>
#include <iostream>
#include <set>
#include <bitset>
#include <tuple>
#include <map>
#include <functional>
#include <fstream>
#include <sstream>
#include<math.h>
#include <stdlib.h>

class LastRegion : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:


    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int);
    void setParams(std::vector<std::pair<QString, double>> params);

private:

    const static std::vector<int> dx,dy;
    const int move_count = 8;
    const static int max_turn_count = 7;

};
#endif // LASTREGION_H
