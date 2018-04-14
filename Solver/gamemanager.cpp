#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "testalgorithm.h"
#include "simplemontecarlo/simplemontecarlo.h"

GameManager::GameManager(const unsigned int x_size, const unsigned int y_size){

    field = std::make_shared<procon::Field>(x_size, y_size, max_val, min_val);
    visualizer = std::make_shared<Visualizer>(*field);

    act_stack = std::vector<std::vector<std::tuple<int,int,int>>>(2, std::vector<std::tuple<int,int,int>>(2, std::make_tuple(0, 0, 0) ) );

    std::shared_ptr<GameManager> share(this); //これ自身を参照するshared_ptr

    team_1 = std::make_shared<SimpleMonteCalro>(share);
    team_2 = std::make_shared<TestAlgorithm>(share);
}

void GameManager::startSimulation(){

    field = std::make_shared<procon::Field>(field->getSize().first, field->getSize().second, max_val, min_val);
    visualizer = std::make_shared<Visualizer>(*field);
    field_vec.push_back(*field);

    visualizer->show();


    for(int turn_count = 0; turn_count < turn_max; ++turn_count){

        std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> team_1_ans = team_1->agentAct(0);
        std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> team_2_ans = team_2->agentAct(1);

        agentAct(0,0,team_1_ans.first);
        agentAct(0,1,team_1_ans.second);
        agentAct(1,0,team_2_ans.first);
        agentAct(1,1,team_2_ans.second);

        changeTurn();

        field_vec.push_back(*field);

        setFieldCount(field_vec.size() - 1);
        visualizer->update();
    }

}

const procon::Field& GameManager::getField(){
    return *field;
}

unsigned int GameManager::getFieldCount(){
    return now_field;
}
void GameManager::setFieldCount(const unsigned int number){
    if(number >= field_vec.size())return ;
    visualizer->setField(field_vec.at(number));
    now_field = number;
    visualizer->update();
}

unsigned int GameManager::getFinalTurn(){
    return turn_max;
}

void GameManager::agentAct(const int turn, const int agent, const std::tuple<int, int, int> tuple_val){

    int type, x_inp, y_inp;
    std::tie(type, x_inp, y_inp) = tuple_val;


    std::pair<int,int> agent_pos = field->getAgent(turn, agent);
    std::pair<int,int> grid_size = field->getSize();

    int x_pos = agent_pos.first + x_inp;
    int y_pos = agent_pos.second + y_inp;

    if(
        x_pos < 0 || x_pos >= grid_size.first ||
        y_pos < 0 || y_pos >= grid_size.second ||
        (type == 1 && field->getState(x_pos, y_pos).first == (turn==1 ? 1 : 2)) ||
        (type == 2 && field->getState(x_pos, y_pos).first != (turn==1 ? 1 : 2))
        ){
        act_stack.at(turn).at(agent) = std::make_tuple(0, 0, 0);
        return ;
    }
    act_stack.at(turn).at(agent) = std::make_tuple(type, x_pos, y_pos);

}

void GameManager::changeTurn(){

    std::map<std::pair<int,int>,std::vector<std::pair<int,int>>> dest_map;
    std::map<std::pair<int,int>,std::vector<std::pair<int,int>>> tile_map;

    int type, pos_x, pos_y;

    for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
        for(int agent_num = 0; agent_num < 2; ++agent_num){

            std::tie(type, pos_x, pos_y) = act_stack.at(turn_flag).at(agent_num);
            std::pair<int,int> pos = std::make_pair(pos_x, pos_y);

            dest_map[ pos ].push_back( std::make_pair(turn_flag, agent_num) );

            if(type == 2){
                tile_map[ pos ].push_back( std::make_pair(turn_flag, agent_num) );
            }
        }

    for(auto elements : dest_map){

        if(elements.second.size() > 1)
            continue;

        if(field->getState(elements.first.first, elements.first.second).first == (elements.second.at(0).first == 0 ? 2 : 1))
            continue;

        field->setAgent(elements.second.at(0).first, elements.second.at(0).second, elements.first.first, elements.first.second);
        field->setState(elements.first.first, elements.first.second, elements.second.at(0).first + 1);
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
