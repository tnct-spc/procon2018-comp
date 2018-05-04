#ifndef FIELD_H
#define FIELD_H


#include <vector>
#include <utility>
#include <random>

namespace procon {

class Field
{
public:
    Field(const unsigned int size_x, const unsigned int size_y);
    Field(const unsigned int size_x, const unsigned int size_y, const std::vector<std::vector<int>>& input_val);
    Field(const unsigned int size_x, const unsigned int size_y,const int max_val,const int min_val);

    const std::vector<std::vector<std::pair<int,int>>>& getAgents() const;
    std::pair<int,int> getAgent(const unsigned int turn, const unsigned int number) const;
    const std::vector<std::vector<int>>& getField() const;
    const std::vector<std::vector<int>>& getValue() const;

    bool isPlaced(const unsigned int x, const unsigned int y);
    std::pair<int,int> getState(const unsigned int x, const unsigned int y) const;//pair<タイル状況,評価値>

    void setState(const unsigned int x, const unsigned int y, const unsigned int state);//タイル状況のみの変更
    void setAgent(const unsigned int turn, const unsigned int number, const unsigned int x_pos, const unsigned int y_pos);


    void setSize(const std::pair<int, int> &grid);
    void setValue(const std::vector<std::vector<int>> &value);
    void setStates(const std::vector<std::vector<int>>& values);
    void setAgents(const std::vector<std::vector<std::pair<int,int>>>& values);

    std::pair<int,int> getSize() const;


private:
    std::vector<std::vector<int>> field_data;
    std::vector<std::vector<int>> value_data;

    //players.at(turn_flag)みたいな事ができるのでvectorにしてます 変えてもいい
    //めっちゃ分かりづらいですね
    std::vector<std::vector<std::pair<int,int>>> agents;

    int grid_x;
    int grid_y;


};
}

#endif // FIELD_H
