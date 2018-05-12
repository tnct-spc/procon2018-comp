#include "simplealgorithm.h"

SimpleAlgorithm::SimpleAlgorithm(std::shared_ptr<GameManager> manager_ptr, const GeneticAgent& agent) :
    AlgorithmWrapper(manager_ptr),
    agent_data(agent)
{

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> SimpleAlgorithm::agentAct(int side){

}

double SimpleAlgorithm::evaluateMove(int side, std::pair<int, int> move){

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

    /*

      除去,タイル無しに移動,自タイルに移動
      0点除去,+除去,-除去,+移動,-移動,0点移動,自タイル移動

    */

    double const_minus_move = -1.0 * data.at(0) * 100 + 30;//マイナスを踏んだ時の定数
    double const_plus_move = data.at(1) * 100 - 30;

    double per_minus_move = -1.0 * data.at(2) * 20 + 6;//マイナスを踏んだ時の得点にかかる倍率
    double per_plus_move = data.at(3) * 20 - 6;

    double const_back_move = -1.0 * data.at(4) * 100 + 30;//自分の陣地に戻った時の定数

    double const_no_move = -1.0 * data.at(5) * 100 + 30;//動かなかった時の定数

    double const_delete_move = data.at(6) * 100 - 30;//タイル除去にかかる定数
    double per_delete_move = data.at(7) * 20 - 6;//タイル除去にかかる倍率

    double per_region = data.at(8) * 20 - 6;//領域ポイントにかかる倍率



}
