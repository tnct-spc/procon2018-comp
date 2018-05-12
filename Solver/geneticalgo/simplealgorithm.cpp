#include "simplealgorithm.h"

SimpleAlgorithm::SimpleAlgorithm(std::shared_ptr<GameManager> manager_ptr, const GeneticAgent& agent) :
    AlgorithmWrapper(manager_ptr),
    agent_data(agent)
{
}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> SimpleAlgorithm::agentAct(int side){

    procon::Field& field = manager->getField();
    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    double max_value = -1000000007;
    int max_count = 0;

    for(int count = 0; count < 81; ++count){

        double value = evaluateMove(side, std::make_pair( count / 9, count % 9 ));

        if(max_value < value){

            max_value = value;
            max_count = count;
        }

    }

    std::vector<int> move_vec(2);
    move_vec.at(0) = max_count / 9;
    move_vec.at(1) = max_count % 9;

    std::vector<std::pair<int,int>> agent_pos(2);
    for(int index = 0; index < 2; ++index){
        agent_pos.at(index) = field.getAgent(side, index);
        agent_pos.at(index).first += x_list.at(move_vec.at(index));
        agent_pos.at(index).second += y_list.at(move_vec.at(index));
    }
    std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> tup;//たぷー

    tup.first = std::make_tuple((field.getState(agent_pos.at(0).first, agent_pos.at(0).second).first == (side == 0 ? 2 : 1) ? 2 : 1),
                                x_list.at(move_vec.at(0)),
                                y_list.at(move_vec.at(0)));

    tup.second = std::make_tuple((field.getState(agent_pos.at(1).first, agent_pos.at(1).second).first == (side == 0 ? 2 : 1) ? 2 : 1),
                                x_list.at(move_vec.at(1)),
                                y_list.at(move_vec.at(1)));

    return tup;
}

double SimpleAlgorithm::evaluateMove(int side, std::pair<int, int> move){

    //例外処理
    if(!manager->canPut(side, move.first, move.second))
        return -1000000007;


    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};


    procon::Field& field = manager->getField();

    std::vector<int> move_vec(2); //ここの実装くそ
    move_vec.at(0) = move.first;
    move_vec.at(1) = move.second;

    std::vector<std::pair<int,int>> now_pos(2); //移動前
    std::vector<std::pair<int,int>> move_pos(2); //移動後
    std::vector<std::pair<int,int>> state(2);//移動先のデータ

    for(int index = 0; index < 2; ++index){
        now_pos.at(index) = field.getAgent(side, index);
        move_pos.at(index) = now_pos.at(index);
        move_pos.at(index).first += x_list.at(move_vec.at(index));
        move_pos.at(index).second += y_list.at(move_vec.at(index));
        state.at(index) = field.getState(move_pos.at(index).first, move_pos.at(index).second);
    }

    const std::vector<double>& data = agent_data.getData();


    double const_minus_move = -1.0 * data.at(0) * 100 + 30;//マイナスを踏んだ時の定数
    double const_plus_move = data.at(1) * 100 - 30;

    double per_minus_move = data.at(2) * 20 - 6;//マイナスを踏んだ時の得点にかかる倍率
    double per_plus_move = data.at(3) * 20 - 6;

    double const_back_move = -1.0 * data.at(4) * 100 + 30;//自分の陣地に戻った時の定数

    double const_no_move = -1.0 * data.at(5) * 100 + 30;//動かなかった時の定数

    double const_delete_move = data.at(6) * 100 - 30;//タイル除去にかかる定数
    double per_delete_move = data.at(7) * 20 - 6;//タイル除去にかかる倍率

    double per_region = data.at(8) * 20 - 6;//領域ポイントにかかる倍率

    double per_point = data.at(9) * 100;//移動先の得点にかかる倍率


    //得点計算
    auto calc = [&](int agent){
        double value = 0;

        int reg = state.at(agent).first;
        int point = state.at(agent).second;

        // 動かない時(この時はconst_back_moveと二重判定が起きる)
        if(move_vec.at(agent) == 8)
            value += const_no_move;

        if(reg == side + 1)//もう自分の色な場合
            value += const_back_move;

        if(reg == (side == 0 ? 2 : 1)){ //除去の場合

            //除去するため得点増減が逆になる
            point *= -1.0;

            value += const_delete_move;
            value += per_delete_move * point;

        }

        if(point > 0){

            value += const_plus_move;
            value += per_plus_move * point;

        }else if(point < 0){

            value += const_minus_move;
            value += per_minus_move;

        }

        value += point * per_point;

        return value;
    };

    return calc(0) + calc(1);

}
