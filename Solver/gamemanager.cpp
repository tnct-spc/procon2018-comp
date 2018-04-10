#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "testalgorithm.h"

GameManager::GameManager(unsigned int x_size, unsigned int y_size){

    field = std::make_shared<procon::Field>(x_size, y_size, max_val, min_val);

    std::shared_ptr<GameManager> share(this); //これ自身を参照するshared_ptr

    team_1 = std::make_shared<TestAlgorithm>(share);
    team_2 = std::make_shared<TestAlgorithm>(share);



    act_stack = std::vector<std::vector<std::pair<int,std::pair<int,int>>>>(2, std::vector<std::pair<int,std::pair<int,int>>>(2,std::make_pair(-1, std::make_pair(-1,-1))));
}

procon::Field GameManager::getField(){
    return *field;
}

bool GameManager::stayAgent(bool turn, int number){
    return agentAct(turn, number, 0, 0, 0);
}

bool GameManager::moveAgent(bool turn, int number, int x_pos, int y_pos){
    return agentAct(turn, number, 1, x_pos, y_pos);
}

bool GameManager::deleteTile(bool turn, int number, int x_pos, int y_pos){
    return agentAct(turn, number, 2, x_pos, y_pos);
}

void GameManager::startSimulation(){

}

bool GameManager::agentAct(int turn, int agent, int type, int x_pos, int y_pos){

    std::pair<int,int> agent_pos = field->getAgent(turn, agent);
    std::pair<int,int> grid_size = field->getSize();

    //クッソ長い例外処理
    if(type && (abs(x_pos) > 1 || abs(y_pos) > 1 || (!x_pos && !y_pos) ||
        agent_pos.first + x_pos < 0 || agent_pos.first + x_pos >= grid_size.first ||
        agent_pos.second + y_pos < 0 || agent_pos.second + y_pos >= grid_size.second ||
        (type == 1 && field->getState(agent_pos.first + x_pos, agent_pos.second + y_pos).first == (turn ? 2 : 1)) ||
        (type == 2 && field->getState(agent_pos.first + x_pos, agent_pos.second + y_pos).first != (turn ? 1 : 2))
         ))
        return false;

    act_stack.at(turn).at(agent) = std::make_pair(type, std::make_pair(agent_pos.first + x_pos, agent_pos.second + y_pos));

    for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
        for(int agent_num = 0; agent_num < 2; ++agent_num)
            if(act_stack.at(turn_flag).at(agent_num).first == -1)
                return true;

    changeTurn();
    return true;
}

void GameManager::changeTurn(){

    std::map<std::pair<int,int>,std::vector<std::pair<int,int>>> dest_map;
    std::map<std::pair<int,int>,std::vector<std::pair<int,int>>> tile_map;

    for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
        for(int agent_num = 0; agent_num < 2; ++agent_num){
            if(act_stack.at(turn_flag).at(agent_num).first == 1)
                dest_map[act_stack.at(turn_flag).at(agent_num).second].push_back( std::make_pair(turn_flag, agent_num) );
            else if(act_stack.at(turn_flag).at(agent_num).first == 2){
                tile_map[act_stack.at(turn_flag).at(agent_num).second].push_back( std::make_pair(turn_flag, agent_num) );
            }
        }

    for(auto elements : dest_map){
        if(elements.second.size() > 1)
            continue;
        field->setAgent(elements.second.at(0).first, elements.second.at(0).second, elements.first.first, elements.first.first);
        field->setState(elements.first.first, elements.first.second, elements.second.at(0).first);
    }

    for(auto elements : tile_map){
        bool state_flag = true;
        if(elements.second.size() > 1)
            continue;

        for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
            for(int agent_num = 0; agent_num < 2; ++agent_num)
                if(field->getAgent(turn_flag, agent_num) == elements.first){
                    state_flag = false;
                    break;
                }
        if(state_flag)
            field->setState(elements.first.first, elements.first.second, 0);
    }


    field->setTurnCount(field->getTurnCount() + 1);
}
