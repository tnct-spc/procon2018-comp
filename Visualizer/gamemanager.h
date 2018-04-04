#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "field.h"
#include <memory>

class GameManager
{
public:

    GameManager(unsigned int x_size, unsigned int y_size);

    procon::Field getField();

private:
    std::shared_ptr<procon::Field> field;

    bool turn_flag = false;

    //ここは後で仕様を変える
    const int max_val = 5;
    const int min_val = -2;

};

#endif // GAMEMANAGER_H
