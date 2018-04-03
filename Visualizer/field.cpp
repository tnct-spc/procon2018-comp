#include "field.h"

procon::Field::Field(unsigned int size_x ,unsigned int size_y)
{
    field_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));
    value_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));
}
procon::Field::Field(unsigned int size_x, unsigned int size_y, std::vector<std::vector<int>> input_val){
    field_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));
    value_data = input_val;
}

std::vector<std::vector<int>> procon::Field::getField(){
    return field_data;
}

bool procon::Field::isPlaced(int x, int y){
    return (field_data.at(x).at(y) != 0);
}

//pair<タイル状況,評価値>を返す
std::pair<int,int> procon::Field::getState(int x, int y){
    return std::make_pair(field_data.at(x).at(y), value_data.at(x).at(y));
}
