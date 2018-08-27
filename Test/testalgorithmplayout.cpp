#include <boost/random.hpp>
#include <time.h>

#include "testalgorithmplayout.h"

TestAlgorithmPlayout::TestAlgorithmPlayout() :
    mt(device()),
    rand_size(8, 12),
    rand_turn(60, 120),
    agents(2)
{
    logger = spdlog::basic_logger_mt("TestAlgorithmPlayout", "../../procon2018-comp/Data/TestAlgorithmPlayout/result.csv");
    logger->set_pattern("%v");
    manager = std::make_shared<GameManager>(8, 10, false, 60);

}

int TestAlgorithmPlayout::playout(params& param_1, params& param_2, std::string write_path){

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

    if(!write_path.empty()){
        // パス渡されたら適当にコンマ区切りで出すようにしたので、適当に確認お願いします
        // 盤面の特徴 -> side=0側のデータ -> side=1側のデータ -> お互いの得点


        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::uniform_int_distribution<> rnd_int(0, INT_MAX);

        //std::shared_ptr<spdlog::logger> logger = spdlog::basic_logger_mt(std::to_string(static_cast<int>(rnd_int(mt))), write_path);

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

void TestAlgorithmPlayout::run(){
    std::vector<double> r;
    boost::random::uniform_real_distribution<> dist(-10.0,10.0);
    boost::random::mt19937_64 mt((unsigned)time(NULL));
    for(int i=0;i<20;i++)r.push_back(dist(mt));

    std::vector<double> const_values_1 = {r[0], r[1], r[2], r[3], r[4], r[5] ,r[6], r[7], r[8]};
    std::vector<double> const_values_2 = {r[9], r[10], r[11], r[12], r[13], r[14] ,r[15], r[16], r[17]};

    double diagonal_move_1 = r[18];
    double diagonal_move_2 = r[19];

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

    std::vector<int> win_count(3, 0);

    for(int hoge = 0; hoge < 1000; ++hoge)
        ++win_count.at(playout(p1, p2, "../../procon2018-comp/Data/TestAlgorithmPlayout/result.csv") + 1);
        // ++win_count.at(playout(p1, p2) + 1);

    std::cout << std::endl << "side 0" << std::endl;
    std::cout << "win : " << win_count.at(1) << std::endl;
    std::cout << "lose : " << win_count.at(2) << std::endl;
    std::cout << "draw : " << win_count.at(0) << std::endl;
}
