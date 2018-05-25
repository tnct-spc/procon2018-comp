#ifndef TESTAGENTPARAM_H
#define TESTAGENTPARAM_H

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
    int rand_agent_number = 100;

    //パラメータのバラケ具合
    int rand_param_count = 10;

    std::vector<GeneticAgent> random_agents;

};

#endif // TESTAGENTPARAM_H
