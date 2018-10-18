#ifndef TESTALGORITHMPLAYOUT_H
#define TESTALGORITHMPLAYOUT_H

#include "gamemanager.h"
#include "depthfirstsearch.h"
#include "dummyalgorithm.h"

#define SPDLOG_EOL ""

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"


#include <random>
#include <QApplication>

// クソ
class TestAlgorithmPlayout
{
public:
    TestAlgorithmPlayout();


    void run();

private:
    void playout(DepthFirstSearch::Parameters& p1, DepthFirstSearch::Parameters& p2, bool iswrite = false);

    std::shared_ptr<spdlog::logger> logger;

    std::random_device device;
    std::mt19937 mt;

    const std::string path = "../../procon2018-comp/out";

    std::shared_ptr<GameManager> manager;
};

#endif // TESTALGORITHMPLAYOUT_H
