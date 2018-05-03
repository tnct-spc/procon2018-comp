#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "testalgorithm.h"
#include "simplemontecarlo/simplemontecarlo.h"
#include "montecarlotreesearch/montecarlotreesearch.h"
#include "dummyalgorithm.h"

GameManager::GameManager(const unsigned int x_size, const unsigned int y_size, QObject *parent)
    : QObject(parent)
{

    field = std::make_shared<procon::Field>(x_size, y_size, max_val, min_val);
    visualizer = std::make_shared<Visualizer>(*field);

    act_stack = std::vector<std::vector<std::tuple<int,int,int>>>(2, std::vector<std::tuple<int,int,int>>(2, std::make_tuple(0, 0, 0) ) );


    connect(visualizer.get(), &Visualizer::nextMove, this, &GameManager::changeMove);
    connect(this, &GameManager::signalAutoMode, visualizer.get(), &Visualizer::slotAutoMode);
    connect(this, &GameManager::setCandidateMove, visualizer.get(), &Visualizer::candidateMove);
}

void GameManager::startSimulation(QString my_algo, QString opponent_algo) {

    std::shared_ptr<GameManager> share(this);

    if (QString::compare("MonteCarloTreeSearch", my_algo) == 0) {
        team_1 = std::make_shared<MonteCarloTreeSearch>(share);
    } else if (QString::compare("SimpleMonteCalro", my_algo) == 0) {
        team_1 = std::make_shared<SimpleMonteCalro>(share);
    } else if (QString::compare("TestAlgorithm", my_algo) == 0) {
        team_1 = std::make_shared<TestAlgorithm>(share);
    } else if (QString::compare("DummyAlgorithm", my_algo) == 0) {
        team_1 = std::make_shared<DummyAlgorithm>(share);
    } else if (QString::compare("beamsearch", my_algo) == 0){
        team_1 = std::make_shared<beamsearch>(share);
    }

    if (QString::compare("MonteCarloTreeSearch", opponent_algo) == 0) {
        team_2 = std::make_shared<MonteCarloTreeSearch>(share);
    } else if (QString::compare("SimpleMonteCalro", opponent_algo) == 0) {
        team_2 = std::make_shared<SimpleMonteCalro>(share);
    } else if (QString::compare("TestAlgorithm", opponent_algo) == 0) {
        team_2 = std::make_shared<TestAlgorithm>(share);
    } else if (QString::compare("DummyAlgorithm", my_algo) == 0) {
        team_2 = std::make_shared<DummyAlgorithm>(share);
    } else if (QString::compare("beamsearch", opponent_algo) == 0){
        team_2 = std::make_shared<beamsearch>(share);
    }

    field = std::make_shared<procon::Field>(field->getSize().first, field->getSize().second, max_val, min_val);

    progresdock = std::make_shared<ProgresDock>();

    field_vec.push_back(std::make_shared<procon::Field>(*field));
    progresdock->addAnswer(*(field_vec.back()));

    visualizer->update();


    //うぇーいｗｗｗｗｗｗｗ
    if(is_auto){
        now_turn = 0;
        for(int now_turn = 0; now_turn < turn_max; ++now_turn){


            std::cout << "turn " << now_turn + 1 << " started" << std::endl << std::endl;

            std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> team_1_ans;// = team_1->agentAct(0);
            std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> team_2_ans;// = team_2->agentAct(1);

            /* マルチスレッドのテスト用
            std::thread th1([&]{team_1_ans =  team_1->agentAct(0);});
            std::thread th2([&]{team_2_ans =  team_2->agentAct(1);});

            th1.join();
            th2.join();
            */

            team_1_ans = team_1->agentAct(0);
            team_2_ans = team_2->agentAct(1);

            agentAct(0,0,team_1_ans.first);
            agentAct(0,1,team_1_ans.second);
            agentAct(1,0,team_2_ans.first);
            agentAct(1,1,team_2_ans.second);

            changeTurn();

            field_vec.push_back(std::make_shared<procon::Field>(*field));

            progresdock->addAnswer(*(field_vec.back()));



            setFieldCount(field_vec.size() - 1);
        }

        now_turn = -1;

        progresdock->show();

    }else{

        now_turn = 0;

        nextMoveForManualMode();

        //visualizerにもauto解除する事を伝える
        emit signalAutoMode(false);
    }

        // 探索→候補を表示→クリック待機
        // これをnow_turn回繰り返す

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

            if(type == 1){
                dest_map[ pos ].push_back( std::make_pair(turn_flag, agent_num) );
            }else
            if(type == 2){
                tile_map[ pos ].push_back( std::make_pair(turn_flag, agent_num) );
                dest_map[ field->getAgent(turn_flag,agent_num) ].push_back( std::make_pair(turn_flag, agent_num) );
            }
        }

    for(auto elements : dest_map){

        if(elements.second.size() > 1)
            continue;

        if(field->getState(elements.first.first, elements.first.second).first == (elements.second.at(0).first == 0 ? 2 : 1))
            continue;

        if(field->getAgent(elements.second.at(0).first,(elements.second.at(0).second ? 0 : 1 )) == elements.first)
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


}

void GameManager::setAutoMode(bool value){
    is_auto = value;
}

std::shared_ptr<Visualizer> GameManager::getVisualizer(){
    return visualizer;
}

int GameManager::getTurnCount(){
    return now_turn;
}

void GameManager::changeMove(const std::vector<std::vector<std::pair<int, int>>>& move){

    if(now_turn == -1)
        return ;

    std::cout << "turn : " << now_turn << std::endl << std::endl;

    for(int side = 0; side < 2; ++side)
        for(int agent = 0; agent < 2; ++agent){

            std::pair<int,int> origin_pos = field->getAgent(side, agent);

            std::pair<int,int> pos = move.at(side).at(agent);

            std::pair<int,int> new_pos = pos;

            new_pos.first -= origin_pos.first;
            new_pos.second -= origin_pos.second;

            agentAct(side, agent,  std::make_tuple( ( field->getState(pos.first, pos.second).first == (side == 0 ? 2 : 1) ? 2 : 1 ), new_pos.first, new_pos.second ) );

        }

    changeTurn();

    field_vec.push_back(std::make_shared<procon::Field>(*field));

    progresdock->addAnswer(*(field_vec.back()));



    setFieldCount(field_vec.size() - 1);

    now_turn++;

    visualizer->update();

    if(now_turn == turn_max){

        now_turn = -1;

        emit signalAutoMode(false);
        progresdock->show();
    }else
        nextMoveForManualMode();

}

void GameManager::nextMoveForManualMode(){

    visualizer->update();
    visualizer->repaint();

    std::vector<std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>>> candidate_move(2);
    candidate_move.at(0) = team_1->agentAct(0);
    candidate_move.at(1) = team_2->agentAct(1);

    std::vector<std::vector<std::pair<int,int>>> return_vec(2, std::vector<std::pair<int,int>>(2) );

    for(int side = 0; side < 2; ++side){

            return_vec.at(side).at(0) = std::make_pair( std::get<1>(candidate_move.at(side).first), std::get<2>(candidate_move.at(side).first) );
            return_vec.at(side).at(1) = std::make_pair( std::get<1>(candidate_move.at(side).second), std::get<2>(candidate_move.at(side).second) );
    }

    for(int side = 0; side < 2; ++side)
        for(int agent = 0; agent < 2; ++agent){
            return_vec.at(side).at(agent).first += field->getAgent(side, agent).first;
            return_vec.at(side).at(agent).second += field->getAgent(side, agent).second;
        }

    emit setCandidateMove(return_vec);

}
