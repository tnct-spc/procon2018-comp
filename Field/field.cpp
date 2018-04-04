#include "field.h"

procon::Field::Field(unsigned int size_x ,unsigned int size_y){

    //初期化処理がめっちゃ分かりづらいですが、四隅にagentを配置してます
    agents = { { std::make_pair(0, 0), std::make_pair(size_x - 1, size_y - 1) }, { std::make_pair(size_x - 1, 0), std::make_pair(0, size_y - 1) } };

    field_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));
    value_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));
}

procon::Field::Field(unsigned int size_x, unsigned int size_y, std::vector<std::vector<int>> input_val){

    agents = { { std::make_pair(0, 0), std::make_pair(size_x - 1, size_y - 1) }, { std::make_pair(size_x - 1, 0), std::make_pair(0, size_y - 1) } };

    field_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));
    value_data = input_val;
}

procon::Field::Field(unsigned int size_x, unsigned int size_y, int max_val, int min_val, double minus_per){

    agents = { { std::make_pair(0, 0), std::make_pair(size_x - 1, size_y - 1) }, { std::make_pair(size_x - 1, 0), std::make_pair(0, size_y - 1) } };

    field_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));
    value_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y));

    std::random_device rnd;
    std::mt19937 mt (rnd());

    std::uniform_int_distribution<> plus_rnd(0,max_val);
    std::uniform_int_distribution<> minus_rnd(min_val,-1);

    std::uniform_real_distribution<> double_rnd(0.0,1.0);

    for(unsigned int x = 0; x < size_x; ++x){
        for(unsigned int y = 0; y < size_y; ++y){

            value_data.at(x).at(y) = (double_rnd(mt) > minus_per
                                      ? plus_rnd(mt)
                                      : minus_rnd(mt) );
        }
    }
}

std::vector<std::vector<int>> procon::Field::getField(){
    return field_data;
}


std::vector<std::vector<std::pair<int,int>>> procon::Field::getAgents(){
    return agents;
}

bool procon::Field::isPlaced(int x, int y){
    return (field_data.at(x).at(y) != 0);
}

//pair<タイル状況,評価値>を返す
std::pair<int,int> procon::Field::getState(int x, int y){
    return std::make_pair(field_data.at(x).at(y), value_data.at(x).at(y));
}
