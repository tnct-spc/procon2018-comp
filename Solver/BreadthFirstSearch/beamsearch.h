#ifndef BEAMSEARCH_H
#define BEAMSEARCH_H
#include "field.h"
#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "visualizer.h"
#include <queue>
#include <algorithm>
#include <iostream>
class beamsearch : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    beamsearch();
    const int beam_range = 100;
    int beam_turn = 15;
    double Evaluation_Field(procon::Field field,int side);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
    std::pair<int,int> agent1,agent2;
    const double var1=1,var2=0,var3=0,var4=0,var5=0;
    //マスのポイント、乱択要素、味方との距離、マイナスのポイントをいかに踏んでいないか、敵との距離
private:
    procon::Field agentmove(procon::Field field,std::vector<std::vector<std::pair<int,std::pair<int,int>>>> pos);
    std::shared_ptr<ProgresDock> progresdock_beamsearch;
};

#endif // BEAMSEARCH_H
