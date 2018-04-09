#include "field.h"

procon::Field::Field(unsigned int size_x ,unsigned int size_y){
    grid_x = size_x;
    grid_y = size_y;

    //初期化処理がめっちゃ分かりづらいですが、四隅にagentを配置してます
    agents = { { std::make_pair(0, 0), std::make_pair(size_x - 1, size_y - 1) }, { std::make_pair(size_x - 1, 0), std::make_pair(0, size_y - 1) } };

    field_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));
    value_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));
}

procon::Field::Field(unsigned int size_x, unsigned int size_y, std::vector<std::vector<int>> input_val){
    grid_x = size_x;
    grid_y = size_y;

    agents = { { std::make_pair(0, 0), std::make_pair(size_x - 1, size_y - 1) }, { std::make_pair(size_x - 1, 0), std::make_pair(0, size_y - 1) } };

    field_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));
    value_data = input_val;
}

procon::Field::Field(unsigned int size_x, unsigned int size_y, int max_val, int min_val, double minus_per){
    grid_x = size_x;
    grid_y = size_y;

    agents = { { std::make_pair(0, 0), std::make_pair(size_x - 1, size_y - 1) }, { std::make_pair(size_x - 1, 0), std::make_pair(0, size_y - 1) } };

    field_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y, 0 ));
    value_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y, 0 ));

    std::random_device rnd;
    std::mt19937 mt (rnd());

    std::uniform_int_distribution<> plus_rnd(0,max_val);
    std::uniform_int_distribution<> minus_rnd(min_val,-1);

    std::uniform_real_distribution<> double_rnd(0.0,1.0);

    //ここは「x軸かつy軸方向に垂直」で解釈します


    for(unsigned int x = 0; x < size_x / 2 + 1; ++x){
        for(unsigned int y = 0; y < size_y / 2 + 1; ++y){

            int value = (double_rnd(mt) > minus_per
                                      ? plus_rnd(mt)
                                      : minus_rnd(mt) );
            value_data.at(x).at(y) = value;
            value_data.at(grid_x - x - 1).at(grid_y - y - 1) = value;
            value_data.at(x).at(grid_y - y - 1) = value;
            value_data.at(grid_x - x - 1).at(y) = value;
        }
    }
}

std::vector<std::vector<int>> procon::Field::getField(){
    return field_data;
}

std::vector<std::vector<int>> procon::Field::getValue(){
    return value_data;
}

std::pair<int,int> procon::Field::getSize(){
    return std::make_pair(grid_x, grid_y);
}


std::vector<std::vector<std::pair<int,int>>> procon::Field::getAgents(){
    return agents;
}

std::pair<int,int> procon::Field::getAgent(int turn, int number){
    return agents.at(turn).at(number);
}

bool procon::Field::isPlaced(int x, int y){
    return (field_data.at(x).at(y) != 0);
}

//pair<タイル状況,評価値>を返す
std::pair<int,int> procon::Field::getState(int x, int y){
    return std::make_pair(field_data.at(x).at(y), value_data.at(x).at(y));
}

void procon::Field::setState(int x, int y, int state){
    field_data.at(x).at(y) = state;
}

void procon::Field::setAgent(int turn, int number, int x_pos, int y_pos){
    agents.at(turn).at(number) = std::make_pair(x_pos, y_pos);
}

int procon::Field::getTurnCount(){
    return turn_count;
}

void procon::Field::setTurnCount(int turn){
    turn_count = turn;
}
