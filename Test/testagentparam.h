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

    //ランダムで生成するエージェントの数(試行回数なので木にしなくていいです)
    //というより無限に試行しないために一応付けているだけです
    int rand_agent_count = 1e5;

    //パラメータのバラケ具合(エージェントの数はrand_param_count^6になる)
    int rand_param_count = 11;

    //ここはrunRand用 エージェント一人につきこれだけの実行を(rand_agent_count)回繰り返す
    int one_agent_buttle_count = 500;


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

    std::vector<std::thread> threads;


};

#endif // TESTAGENTPARAM_H
