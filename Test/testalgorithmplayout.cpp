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

    std::vector<std::reference_wrapper<params>> param_vector;
    param_vector.emplace_back(param_1);
    param_vector.emplace_back(param_2);

    for(int index = 0; index < 2; ++index){
        agents.at(index) = std::make_shared<UseAbstractData>(manager->getField(), manager->getFinalTurn(), index);
        params& param = param_vector.at(index);
        agents.at(index)->setParameters(param.values, param.diagonal, param.tile_value_func, param.eval_sum_func);
    }

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

	// 同点なら-1を、それ以外なら勝ったside(0もしくは1)を返す
    if(points.at(0) == points.at(1))
        return -1;

    return points.at(0) < points.at(1);
}

void TestAlgorithmPlayout::run(){

    std::vector<double> const_values_1 = {1.6, 1.2, -0.7, 1.2, 1.2, -0.3 ,1.2, -0.4, -0.4};
    std::vector<double> const_values_2 = {1.2, 1.7, -0.4, 1.5, 1.0, 0 ,0.8, -0.4, -0.2};

    double diagonal_move_1 = 1.5;
    double diagonal_move_2 = 1.0;

    std::function<void(double&, bool, int)> calc_value_func_1 = [](double& eval_result, bool is_return, int tile_value){
            eval_result *= (is_return ? 12 : 16 + tile_value);
    };
    std::function<void(double&, bool, int)> calc_value_func_2 = [](double& eval_result, bool is_return, int tile_value){
            eval_result *= (is_return ? 8 : 16 + tile_value);
    };

    std::function<double(double, double)> calc_eval_sum_1 = [](double inp_1, double inp_2){
        return inp_1 + inp_2;
    };
    std::function<double(double, double)> calc_eval_sum_2 = [](double inp_1, double inp_2){
        return inp_1 * inp_2;
    };

    params p1(const_values_1, diagonal_move_1, calc_value_func_1, calc_eval_sum_1);
    params p2(const_values_2, diagonal_move_2, calc_value_func_2, calc_eval_sum_2);

    std::vector<int> win_count(3);

    for(int hoge = 0; hoge < 100; ++hoge)
        ++win_count.at(playout(p1, p2) + 1);

    std::cout << std::endl << "side 0" << std::endl;
    std::cout << "win : " << win_count.at(1) << std::endl;
    std::cout << "lose : " << win_count.at(2) << std::endl;
    std::cout << "draw : " << win_count.at(0) << std::endl;
}
