#ifndef FIELD_H
#define FIELD_H


#include <vector>
#include <utility>
#include <random>

namespace procon {

class Field
{
public:
    Field(unsigned int size_x, unsigned int size_y);
    Field(unsigned int size_x, unsigned int size_y, std::vector<std::vector<int>> input_val);
    Field(unsigned int size_x, unsigned int size_y,int max_val,int min_val,double minus_per = 0.3);

    std::vector<std::vector<std::pair<int,int>>> getAgents();
    std::vector<std::vector<int>> getField();
    bool isPlaced(int x, int y);
    std::pair<int,int> getState(int x, int y);//pair<タイル状況,評価値>

private:
    std::vector<std::vector<int>> field_data;
    std::vector<std::vector<int>> value_data;

    bool turn_flag = true;

    //players.at(turn_flag)みたいな事ができるのでvectorにしてます 変えてもいい
    //めっちゃ分かりづらいですね
    std::vector<std::vector<std::pair<int,int>>> agents;


};
}

#endif // FIELD_H
