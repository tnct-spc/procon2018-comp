#ifndef TESTEVALUATESPEED_H
#define TESTEVALUATESPEED_H

#include "field.h"

class TestEvaluateSpeed
{
public:
    TestEvaluateSpeed();
    void run();

private:
    std::random_device rnd;
    std::mt19937 mt;
};

#endif // TESTEVALUATESPEED_H
