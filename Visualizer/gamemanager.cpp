#include "gamemanager.h"

GameManager::GameManager(unsigned int x_size, unsigned int y_size){
    field = std::make_shared<procon::Field>(x_size, y_size, max_val, min_val);
    act_stack = std::vector<std::vector<std::pair<int,int>>>(2, std::vector<std::pair<int,int>>(2,std::make_pair(-1, -1)));
}

procon::Field GameManager::getField(){
    return *field;
}

bool GameManager::stayAgent(bool turn, int number){
    return  agentAct(turn, number, 0, 0);
}

bool GameManager::moveAgent(bool turn, int number, int rotate_num){
    return agentAct(turn, number, 1, rotate_num);
}

bool GameManager::deleteTile(bool turn, int number, int rotate_num){
    return agentAct(turn, number, 2, rotate_num);
}

bool GameManager::agentAct(int turn, int agent, int type, int number){

    if(act_stack[turn][agent] != std::make_pair(-1, -1))
        return false;

    act_stack[turn][agent] = std::make_pair(type, number);

    for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
        for(int agent_num = 0; agent_num < 2; ++agent_num)
            if(act_stack[turn_flag][agent_num] == std::make_pair(-1, -1) )
                return true;

    return changeTurn();
}

bool GameManager::changeTurn(){


    return true;
}
