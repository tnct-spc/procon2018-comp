#include "dummyalgorithm.h"

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> DummyAlgorithm::agentAct(int){
    return std::make_pair(std::make_tuple(0,0,0), std::make_tuple(0,0,0));
}

void DummyAlgorithm::setParams(std::vector<std::pair<QString, double>> params)
{

}

void DummyAlgorithm::setRandomParams(std::vector<std::pair<QString, double>> params){

}
