#include "testalgorithmplayout.h"

TestAlgorithmPlayout::TestAlgorithmPlayout() :
    mt(device()),
    rand_size(8, 12),
    rand_turn(60, 120),
    agents(2)
{
    manager = std::make_shared<GameManager>(8, 10, false, 60);

}

int TestAlgorithmPlayout::playout(params& param_1, params& param_2){

    manager->resetManager(rand_size(mt), rand_size(mt), false, rand_turn(mt));

    std::vector<params&> param_vector(2);
    param_vector.at(0) = param_1;
    param_vector.at(1) = param_2;

    for(int index = 0; index < 2; ++index){
        agents.at(index) = std::make_shared<UseAbstractData>(manager->getField(), manager->getFinalTurn(), index);
        params& param = param_vector.at(index);
        agents.at(index)->setParameters(param.values, param.diagonal, param.tile_value_func, param.eval_sum_func);
    }

    for(int turn = 0; turn < manager->getFinalTurn(); ++turn){
        for(int side = 0; side < 2; ++side){
            std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> act = agents.at(side)->agentAct(manager->getField().getTurnCount());
            manager->agentAct(side, 0, act.first);
            manager->agentAct(side, 1, act.second);
        }
        manager->changeTurn(false);
    }

    std::vector<int> points(2);
    const std::vector<std::pair<int,int>>& field_points = manager->getField().getPoints();

    for(int side = 0; side < 2; ++side)
        points.at(side) = field_points.at(side).first + field_points.at(side).second;


	// 同点なら-1を、それ以外なら勝ったside(0もしくは1)を返す
    if(points.at(0) == points.at(1))
        return -1;

    return points.at(0) < points.at(1);
}

void TestAlgorithmPlayout::run(){

}
