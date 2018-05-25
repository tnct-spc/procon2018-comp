#ifndef TESTAGENTPARAM_H
#define TESTAGENTPARAM_H

#include "gamemanager.h"
#include "doubleagent/agents/agentwrapper.h"
#include "doubleagent/agents/testdoubleagentalgo.h"
#include "doubleagent/agentmanager.h"

class TestAgentParam
{
public:
    TestAgentParam();
    void run();

private:
    //ランダムで生成するエージェントの数
    int rand_agent_count = 100;

    //パラメータのバラケ具合(エージェントの数はrand_param_count^6になる)
    int rand_param_count = 4;

    double rand_param_diff;
    int agent_count;

    int cpu_num;

    std::vector<GameManager*> managers;

    std::vector<GeneticAgent> random_agents;
    std::vector<GeneticAgent> buttle_agents;

    std::vector<int> win_count;

};

#endif // TESTAGENTPARAM_H
