#include "testlastforce.h"

TestLastForce::TestLastForce() :
    mt(device()),
    rand_size(8, 12),
    rand_turn(60, 120),
    agents(2),
    Last(2)
{
    manager = std::make_shared<GameManager>(8, 10, false, 60);

}

int TestLastForce::playout(){



    manager->resetManager(rand_size(mt), rand_size(mt), false, rand_turn(mt));


    for(int index = 0; index < 2; ++index){
        agents.at(index) = std::make_shared<GeneticAlgo>(manager->getField(), manager->getFinalTurn(), index);
    }

    while(manager->getTurnCount() < manager->getFinalTurn()){
        if(manager->getFinalTurn() - manager->getTurnCount() <= 2){
            for(int index = 0; index < 2; ++index){
                Last.at(index) = std::make_shared<LastForce>(manager->getField(), manager->getFinalTurn(), index);
            }
            std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> act = Last.at(1)->agentAct(manager->getTurnCount());
            if(act == std::make_pair(std::make_tuple(0,0,0),std::make_tuple(0,0,0))){
                std::cout<<"GAを読んでいる"<<std::endl;
                act = agents.at(1)->agentAct(manager->getTurnCount());
            }
            manager->agentAct(1, 0, act.first);
            manager->agentAct(1, 1, act.second);

            act = agents.at(0)->agentAct(manager->getTurnCount());
            manager->agentAct(0, 0, act.first);
            manager->agentAct(0, 1, act.second);

        }else{
            for(int side = 0; side < 2; ++side){
                std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> act = agents.at(side)->agentAct(manager->getTurnCount());
                manager->agentAct(side, 0, act.first);
                manager->agentAct(side, 1, act.second);
            }
        }
        manager->changeTurn(false);
    }

    manager->getField().updatePoint();

    std::vector<int> points(2);
    const std::vector<std::pair<int,int>>& field_points = manager->getField().getPoints(false);

    for(int side = 0; side < 2; ++side)
        points.at(side) = field_points.at(side).first + field_points.at(side).second;
    // 同点なら-1を、それ以外なら勝ったside(0もしくは1)を返す
    if(points.at(0) == points.at(1))
        return -1;

    return (points.at(0) < points.at(1));
}

void TestLastForce::run(){


    std::vector<int> win_count(3, 0);

    for(int hoge = 0; hoge < 5000; ++hoge){
        std::cout<<hoge<<"回目のプレイアウトだよ"<<std::endl;
        int s = playout()+1;
        if(s == 0){
            std::cout<<"引き分け"<<std::endl;
        }
        if(s == 1){
            std::cout<<"先手の勝ち"<<std::endl;
        }
        if(s == 2){
            std::cout<<"後手の勝ち"<<std::endl;
        }

        ++win_count.at(s);
        std::cout << std::endl << "side 0" << std::endl;
        std::cout << "win : " << win_count.at(1) << std::endl;
        std::cout << "lose : " << win_count.at(2) << std::endl;
        std::cout << "draw : " << win_count.at(0) << std::endl;
    }

    std::cout << std::endl << "side 0" << std::endl;
    std::cout << "win : " << win_count.at(1) << std::endl;
    std::cout << "lose : " << win_count.at(2) << std::endl;
    std::cout << "draw : " << win_count.at(0) << std::endl;
}
