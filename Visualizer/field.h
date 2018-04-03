#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <utility>

namespace procon {

class Field
{
private:
    std::vector<std::vector<int>> field_data;
    std::vector<std::vector<int>> value_data;

public:
    Field(unsigned int size_x, unsigned int size_y);
    Field(unsigned int size_x, unsigned int size_y, std::vector<std::vector<int>> input_val);

    std::vector<std::vector<int>> getField();
    bool isPlaced(int x, int y);
    std::pair<int,int> getState(int x, int y);//pair<タイル状況,評価値>

};
}

#endif // FIELD_H
