#include "testalgorithmplayout.h"

TestAlgorithmPlayout::TestAlgorithmPlayout() :
    mt(device())
{
    manager = std::make_shared<GameManager>(-1, 10, false);
    logger = spdlog::basic_logger_mt("TestAlgorithmPlayout", path);
    logger->set_pattern("%v");

}

void TestAlgorithmPlayout::playout(DepthFirstSearch::Parameters& p1, DepthFirstSearch::Parameters& p2, bool iswrite){

    manager->resetManager(0, 0, false);

    manager->getField();
    manager->getField().getFinalTurn();

    std::vector<std::shared_ptr<DepthFirstSearch>> agents;

    DepthFirstSearch dep(manager->getField(), manager->getField().getFinalTurn(), 0);

    for(int side = 0; side < 2; ++side)
        agents.emplace_back(std::make_shared<DepthFirstSearch>(manager->getField(), manager->getField().getFinalTurn(), side));

    agents.at(0)->setParams(p1);
    agents.at(1)->setParams(p2);

    while(manager->getTurnCount() < manager->getFinalTurn()){
        for(int side = 0; side < 2; ++side){
            std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> act = agents.at(side)->agentAct(manager->getTurnCount());
            manager->agentAct(side, 0, act.first);
            manager->agentAct(side, 1, act.second);
        }
        manager->changeTurn(false);
    }

    manager->getField().updatePoint();

    std::vector<int> points(2);
    const std::vector<std::pair<int,int>>& field_points = manager->getField().getPoints(false);

    for(int side = 0; side < 2; ++side)
        points.at(side) = field_points.at(side).first + field_points.at(side).second;

    if(iswrite){
        std::stringstream outstream;
        outstream << p1.pena_ratio_val << "," << p2.pena_ratio_val << ",";
        outstream << points.at(0) << "," << points.at(1) << "\n";
        logger->info(outstream.str());
        logger->flush();
    }

}

void TestAlgorithmPlayout::run(){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<> rand_param(0.5, 5);

    for(int count = 0; count < 1e7; ++count){
        std::cout << "count : " << count << std::endl;
        DepthFirstSearch::Parameters params_1, params_2;
        params_1.beam_width = 100;
        params_2.beam_width = 100;
        params_1.predict_weight = 0.65;
        params_2.predict_weight = 0.65;
        do{
        params_1.pena_ratio_val = rand_param(mt);
        params_2.pena_ratio_val = rand_param(mt);
        }while(0);
        if(params_1.pena_ratio_val > params_2.pena_ratio_val)
            std::swap(params_1, params_2);

        playout(params_1, params_2, true);

    }


}
