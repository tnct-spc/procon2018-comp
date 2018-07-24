#ifndef TESTGETFIELDDATA_H
#define TESTGETFIELDDATA_H

#include <thread>
#include <mutex>
#include <random>
#include "gamemanager.h"

class TestGetFieldData
{
public:
    const double calc_time = 20.0;
    int cpu_num;
    std::random_device rnd;
    std::mt19937 mt;
    std::mutex mtx;

    std::vector<std::shared_ptr<GameManager>> manager_vec;

    TestGetFieldData();
    void run();
};

#endif // TESTGETFIELDDATA_H
