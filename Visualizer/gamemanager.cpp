#include "gamemanager.h"

GameManager::GameManager(unsigned int x_size, unsigned int y_size){
    field = std::make_shared<procon::Field>(x_size, y_size, max_val, min_val);
    act_stack = std::vector<std::vector<std::pair<int,std::pair<int,int>>>>(2, std::vector<std::pair<int,std::pair<int,int>>>(2,std::make_pair(-1, std::make_pair(-1,-1))));
}

procon::Field GameManager::getField(){
    return *field;
}

bool GameManager::stayAgent(bool turn, int number){
    return agentAct(turn, number, 1, 0, 0);
}

bool GameManager::moveAgent(bool turn, int number, int x_pos, int y_pos){
    return agentAct(turn, number, 1, x_pos, y_pos);
}

bool GameManager::deleteTile(bool turn, int number, int x_pos, int y_pos){
    return agentAct(turn, number, 2, x_pos, y_pos);
}

bool GameManager::agentAct(int turn, int agent, int type, int x_pos, int y_pos){

    std::pair<int,int> agent_pos = field->getAgent(turn, agent);
    std::pair<int,int> grid_size = field->getSize();

    //クッソ長い例外処理
    if(abs(x_pos) > 1 || abs(y_pos) > 1 || (!x_pos && !y_pos) ||
        agent_pos.first + x_pos < 0 || agent_pos.first + x_pos >= grid_size.first ||
        agent_pos.second + y_pos < 0 || agent_pos.second + y_pos >= grid_size.second)
        return false;

    act_stack[turn][agent] = std::make_pair(type, std::make_pair(x_pos, y_pos));

    for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
        for(int agent_num = 0; agent_num < 2; ++agent_num)
            if(act_stack[turn_flag][agent_num].first == -1)
                return true;

    changeTurn();
    return true;
}

void GameManager::changeTurn(){
    for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
        for(int agent_num = 0; agent_num < 2; ++agent_num)
            ;


    field->setTurnCount(field->getTurnCount() + 1);
}
