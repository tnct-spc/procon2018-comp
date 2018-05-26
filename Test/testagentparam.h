#ifndef TESTAGENTPARAM_H
#define TESTAGENTPARAM_H

#include "gamemanager.h"
#include "doubleagent/agents/agentwrapper.h"
#include "doubleagent/agents/testdoubleagentalgo.h"
#include "doubleagent/agentmanager.h"

#include <fstream>
#include <mutex>

class TestAgentParam
{
public:
    TestAgentParam();
    void runFix();
    void runRand();

private:
    std::mutex mtx;

    //ランダムで生成するエージェントの数
    int rand_agent_count = 1e5;

    //パラメータのバラケ具合(エージェントの数はrand_param_count^6になる)
    int rand_param_count = 11;

    int buttle_count = 1;

    double rand_param_diff;
    int agent_count;

    int cpu_num;

    std::random_device rnd;
    std::mt19937 mt;
    std::uniform_int_distribution<> rand_turn;
    std::uniform_int_distribution<> rand_size;


    int buttle(GeneticAgent& agent_1_data, GeneticAgent& agent_2_data, int index);

    std::vector<GameManager*> managers;

    std::vector<GeneticAgent> buttle_agents;

    std::vector<int> win_count;
    std::vector<int> try_count;

};

#endif // TESTAGENTPARAM_H
