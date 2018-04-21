#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "testalgorithm.h"
#include "simplemontecarlo/simplemontecarlo.h"
#include "montecarlotreesearch/montecarlotreesearch.h"

GameManager::GameManager(const unsigned int x_size, const unsigned int y_size, QObject *parent)
    : QObject(parent)
{

    field = std::make_shared<procon::Field>(x_size, y_size, max_val, min_val);
    visualizer = std::make_shared<Visualizer>(*field);

    act_stack = std::vector<std::vector<std::tuple<int,int,int>>>(2, std::vector<std::tuple<int,int,int>>(2, std::make_tuple(0, 0, 0) ) );

    std::shared_ptr<GameManager> share(this); //これ自身を参照するshared_ptr

    team_1 = std::make_shared<MonteCarloTreeSearch>(share);
    team_2 = std::make_shared<MonteCarloTreeSearch>(share);


    connect(visualizer.get(), &Visualizer::nextMove, this, &GameManager::changeMove);
    connect(this, &GameManager::signalAutoMode, visualizer.get(), &Visualizer::slotAutoMode);
    connect(this, &GameManager::signalAutoMode, [=](bool value){std::cout << value << std::endl;});

}

void GameManager::startSimulation(){

    field = std::make_shared<procon::Field>(field->getSize().first, field->getSize().second, max_val, min_val);

    progresdock = std::make_shared<ProgresDock>();

    field_vec.push_back(std::make_shared<procon::Field>(*field));
    progresdock->addAnswer(*(field_vec.back()));

    visualizer->update();


    //うぇーいｗｗｗｗｗｗｗ
    if(is_auto){
        for(int turn_count = 0; turn_count < turn_max; ++turn_count){

            std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> team_1_ans;// = team_1->agentAct(0);
            std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> team_2_ans;// = team_2->agentAct(1);

            std::thread th1([&]{team_1_ans =  team_1->agentAct(0);});
            std::thread th2([&]{team_2_ans =  team_2->agentAct(1);});

            th1.join();
            th2.join();

            agentAct(0,0,team_1_ans.first);
            agentAct(0,1,team_1_ans.second);
            agentAct(1,0,team_2_ans.first);
            agentAct(1,1,team_2_ans.second);

            changeTurn();

            field_vec.push_back(std::make_shared<procon::Field>(*field));

            progresdock->addAnswer(*(field_vec.back()));


            std::cout << "turn : " << turn_count << std::endl;

            setFieldCount(field_vec.size() - 1);
        }

        progresdock->show();

    }else{
        // この変数、-1でない場合はターン数を記憶します
        humanpower_mode_turn = 0;

        //visualizerにもauto解除する事を伝える
        emit signalAutoMode(false);
    }

        // 探索→候補を表示→クリック待機
        // これをturn_count回繰り返す

        /*
        {

            std::vector<std::vector<std::pair<int,int>>> args(2, std::vector<std::pair<int,int>>(2) );

            args.at(0).at(0) = std::make_pair(std::get<1>(team_1_ans.first), std::get<2>(team_1_ans.first));
            args.at(0).at(1) = std::make_pair(std::get<1>(team_1_ans.second), std::get<2>(team_1_ans.second));
            args.at(1).at(0) = std::make_pair(std::get<1>(team_2_ans.first), std::get<2>(team_2_ans.first));
            args.at(1).at(1) = std::make_pair(std::get<1>(team_2_ans.second), std::get<2>(team_2_ans.second));


            std::vector<std::vector<std::pair<int,int>>> act_val = visualizer->clickWait( args );//ここからクリックされるまで待機

            visualizer->update();

            for(int side = 0; side < 2; ++side){
                for(int agent = 0; agent < 2; ++agent){
                    std::pair<int,int> pos = act_val.at(side).at(agent);

                    int type = (field->getState(pos.first, pos.second).first == (side == 0 ? 2 : 1) ? 2 : 1);

                    agentAct(side, agent, std::make_tuple(type, pos.first, pos.second));
                }
            }

        }
        */




}

procon::Field& GameManager::getField(){
    return *field;
}

unsigned int GameManager::getFieldCount(){
    return now_field;
}
void GameManager::setFieldCount(const unsigned int number){
    if(number >= field_vec.size())return ;
    visualizer->setField(*field_vec.at(number));
    now_field = number;
    visualizer->update();
    visualizer->repaint();
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

void GameManager::setAutoMode(bool value){
    is_auto = value;
}

std::shared_ptr<Visualizer> GameManager::getVisualizer(){
    return visualizer;
}

void GameManager::changeMove(const std::vector<std::vector<std::pair<int, int>>>& move){
    std::cout << "change" << std::endl;

}
