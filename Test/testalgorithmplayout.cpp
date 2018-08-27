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

int TestAlgorithmPlayout::playout(params& param_1, params& param_2, bool iswrite){

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
            outstream << (it != std::prev(features.end()) ? "," : ":");
        }

        for(auto value : param_1.values){
            outstream << std::noshowpoint << value;
            outstream << ",";
        }
        outstream << std::noshowpoint << param_1.diagonal;
        outstream << ":";

        for(auto value : param_2.values){
            outstream << std::noshowpoint << value;
            outstream << ",";
        }
        outstream << std::noshowpoint << param_2.diagonal;
        outstream << ":";

        outstream << points.at(0) << "," << points.at(1) << "\n";
        logger->info(outstream.str());
    }

    // 同点なら-1を、それ以外なら勝ったside(0もしくは1)を返す
    if(points.at(0) == points.at(1))
        return -1;

    return (points.at(0) < points.at(1));
}

void TestAlgorithmPlayout::run2(double v1, double v2){

    std::vector<double> const_values_1 = {1.6, 1.2, -0.7, 1.2, 1.2, -0.3 ,1.2, -0.4, -0.4};
    std::vector<double> const_values_2 = {1.2, 1.7, -0.4, 1.5, 1.0, 0 ,0.8, -0.4, -0.2};

    double diagonal_move_1 = 1.5;
    double diagonal_move_2 = 1.0;
    diagonal_move_1 = v1;
    diagonal_move_2 = v2;

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
    params p2(const_values_2, diagonal_move_2, calc_value_func_1, calc_eval_sum_1);

    std::vector<int> win_count(3, 0);

    std::uniform_real_distribution<> rand_param(-15, 15);

    for(int hoge = 0; hoge < 1000; ++hoge){
        for(auto& va : const_values_1)
            va = rand_param(mt);
        for(auto& va : const_values_2)
            va = rand_param(mt);
        p1.values = const_values_1;
        p2.values = const_values_2;
        ++win_count.at(playout(p1, p2) + 1);
        // ++win_count.at(playout(p1, p2, true) + 1);
    }

    std::stringstream outstream;
    outstream << v1 << " , " << v2 << " , ";
    outstream << win_count.at(1) << " , ";
    outstream << win_count.at(2) << " , ";
    outstream << win_count.at(0) << std::endl;
    logger->info(outstream.str());
}

void TestAlgorithmPlayout::run(){
    for(double hog = 0.0; hog <= 2.5; hog+=0.1)
        for(double hog2 = hog + 0.1; hog2 <= 2.5; hog2+=0.1){
            run2(hog, hog2);
        }
}
