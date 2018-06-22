#ifndef TESTEVALUATESPEED_H
#define TESTEVALUATESPEED_H

#include "gamemanager.h"
#include "doubleagent/agents/evaluateparam.h"
#include "doubleagent/agentmanager.h"

class TestEvaluateSpeed
{
public:
    TestEvaluateSpeed();
    void run();

private:
    std::random_device rnd;
    std::mt19937 mt;


    const int calc_count = 10000;
};

#endif // TESTEVALUATESPEED_H
