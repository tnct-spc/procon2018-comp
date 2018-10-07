#include "lastregion.h"


const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> LastRegion::agentAct(int){

    std::stack<std::tuple<int, std::pair<int,int>, std::pair<int,int>, std::vector<std::pair<int, int>>>> sta;

    sta.push(std::make_tuple(0, field.getAgent(side, 0), field.getAgent(side, 1), std::vector<std::pair<int,int>>()));
}

const std::vector<int> LastRegion::dx({1, 1, 0, -1, -1, -1, 0, 1, 0});
const std::vector<int> LastRegion::dy({0, -1, -1, -1, 0, 1, 1, 1, 0});
void LastRegion::setParams(std::vector<std::pair<QString, double>> params)
{

}
