#include "gamemanager.h"

GameManager::GameManager(unsigned int x_size, unsigned int y_size){
    field = std::make_shared<procon::Field>(x_size, y_size, max_val, min_val);
}

procon::Field GameManager::getField(){
    return *field;
}
