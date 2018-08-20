#ifndef TESTLASTFORCE_H
#define TESTLASTFORCE_H


#include "gamemanager.h"
#include "useabstractdata.h"
#include "geneticalgo/geneticagent.h"
#include "LastForce/lastforce.h"

// クソ
struct params;

class TestLastForce
{
public:
    TestLastForce();


    void run();

private:
    int playout();

    std::random_device device;
    std::mt19937 mt;

    std::uniform_int_distribution<> rand_size;
    std::uniform_int_distribution<> rand_turn;

    std::shared_ptr<GameManager> manager;

    std::vector<std::shared_ptr<GeneticAlgo>> agents;
    std::vector<std::shared_ptr<LastForce>> Last;
};


#endif // TESTLASTFORCE_H
