#ifndef TESTGETFIELDDATA_H
#define TESTGETFIELDDATA_H

#include <mutex>
#include <thread>
#include <random>
#include <bitset>
#include <streambuf>

#include "gamemanager.h"
#include "binaryio.h"

#define SPDLOG_EOL ""

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#pragma push_macro("slots")
#undef slots
#include <boost/python.hpp>
#pragma pop_macro("slots")

class TestGetFieldData
{
public:
    const double calc_time = 1.0;
    int cpu_num;
    std::random_device rnd;
    std::mt19937 mt;
    std::mutex mtx;

    std::shared_ptr<spdlog::logger> logger;

    std::vector<std::shared_ptr<GameManager>> manager_vec;

    int calc_count = 100000;

    TestGetFieldData();
    void run();
};

#endif // TESTGETFIELDDATA_H
