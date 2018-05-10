#ifndef BEAMSEARCH_H
#define BEAMSEARCH_H
#include <queue>
#include <algorithm>
#include <iostream>
#include <thread>

#include "field.h"
#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "visualizer.h"
#include "geneticalgo/geneticagent.h"

class BeamSearch : public AlgorithmWrapper
{
    // using AlgorithmWrapper::AlgorithmWrapper;
public:
    BeamSearch(std::shared_ptr<GameManager> manager_ptr, const GeneticAgent& agent = GeneticAgent(5));

    //beamsearch();
    const int beam_range = 1000;
    int beam_turn = 10;
    double Evaluation_Field(procon::Field field,int side);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int side);
    std::pair<int,int> agent1,agent2;
    //マスのポイント、乱択要素、味方との距離、マイナスのポイントをいかに踏んでいないか、敵との距離
private:
    procon::Field agentmove(procon::Field field,std::vector<std::vector<std::pair<int,std::pair<int,int>>>> pos);
    std::shared_ptr<ProgresDock> progresdock_beamsearch;

    double most_Eva_value = 0;

    GeneticAgent agent_data;
};

#endif // BEAMSEARCH_H
