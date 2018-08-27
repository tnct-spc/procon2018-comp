#include "testalgorithmplayout.h"

TestAlgorithmPlayout::TestAlgorithmPlayout() :
    mt(device()),
    rand_size(8, 12),
    rand_turn(60, 120),
    agents(2)
{
    manager = std::make_shared<GameManager>(8, 10, false, 60);
    logger = spdlog::basic_logger_mt("TestAlgorithmPlayout", path);
    logger->set_pattern("%v");

}

void TestAlgorithmPlayout::playout(bool iswrite){

    manager->resetManager(rand_size(mt), rand_size(mt), false, rand_turn(mt));

    for(int index = 0; index < 2; ++index)
        agents.at(index) = std::make_shared<UseAbstractData>(manager->getField(), manager->getFinalTurn(), index);

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
        // パス渡されたら適当にコンマ区切りで出すようにしたので、適当に確認お願いします
        // 盤面の特徴 -> side=0側のデータ -> side=1側のデータ -> お互いの得点


        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::uniform_int_distribution<> rnd_int(0, INT_MAX);

        const std::vector<double>& features = manager->getField().getFeatures();

        std::stringstream outstream;

        for(auto it = features.begin(); it != features.end(); ++it){
            outstream <<  std::noshowpoint << *it;
            outstream << (it != std::prev(features.end()) ? "," : ",");
        }

        for(auto value : agents.at(0)->const_values){
            outstream << std::noshowpoint << value;
            outstream << ",";
        }
        outstream << std::noshowpoint << agents.at(0)->diagonal_move;
        outstream << ",";

        for(auto value : agents.at(1)->const_values){
            outstream << std::noshowpoint << value;
            outstream << ",";
        }
        outstream << std::noshowpoint << agents.at(1)->diagonal_move;
        outstream << ",";

        outstream << points.at(0) << "," << points.at(1) << "\n";
        logger->info(outstream.str());
    }

}

void TestAlgorithmPlayout::run(){
    for(int count = 0; count < 1e7; ++count)
        playout(true);
}
