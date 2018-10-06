#include "testalgorithmplayout.h"

TestAlgorithmPlayout::TestAlgorithmPlayout() :
    mt(device()),
    rand_size(8, 12),
    rand_turn(60, 120),
    agents(2),
    agent(10),
    enemy(10)
{
    manager = std::make_shared<GameManager>(8, 10, false, 60);
    logger = spdlog::basic_logger_mt("TestAlgorithmPlayout", path);
    logger->set_pattern("%v");

    mt = std::mt19937(device());
    rand_dist = std::uniform_int_distribution<>(0, params.size() - 1);

}

int TestAlgorithmPlayout::playout(bool iswrite){

    manager->resetManager(rand_size(mt), rand_size(mt), false, rand_turn(mt));

    enemy.setData(params.at(rand_dist(mt)));

    agents.at(0) = std::make_shared<UseAbstractData>(manager->getField(), manager->getFinalTurn(), 0, agent);

    agents.at(1) = std::make_shared<UseAbstractData>(manager->getField(), manager->getFinalTurn(), 1, enemy);

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

    if(points.at(0) == points.at(1))
        return 0;

    return (points.at(0) > points.at(1) ? 1 : -1);

}

void TestAlgorithmPlayout::run(){
    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_real_distribution<> rand_param(0, 1);
    for(int count = 0; count < 1e7; ++count){
        std::vector<double> param(10,0);
        for(auto& p : param)
            p = rand_param(mt);

        int point = 0;
        agent.setData(param);
        for(int play = 0; play < 300; ++play){
            point += playout();
        }

        if(point > -20){
            std::stringstream out;
            out << point << std::endl;
            out << "{";
            for(auto it = param.begin(); it != param.end(); ++it)
                out << (*it) << (std::next(it) != param.end() ? ", ": "");
            out << "}" << std::endl;
            logger->info(out.str());
            std::cout << out.str();
        }
    }
}
