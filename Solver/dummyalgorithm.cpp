#include "dummyalgorithm.h"

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> DummyAlgorithm::agentAct(int){
    return std::make_pair(std::make_tuple(0,0,0), std::make_tuple(0,0,0));
}
