#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "testalgorithm.h"
#include "simplemontecarlo/simplemontecarlo.h"
#include "montecarlotreesearch/montecarlotreesearch.h"
#include "dummyalgorithm.h"
#include "simplemontecarlo/montecarlowithalgo.h"
#include "BreadthFirstSearch/beamsearch.h"
#include "geneticalgo/simplealgorithm.h"

GameManager::GameManager(const unsigned int x_size, const unsigned int y_size, bool vis_show, const int turn_max, QObject *parent)
    : QObject(parent),
    share(std::shared_ptr<GameManager>(this)),
    turn_max(turn_max),
    vis_show(vis_show)
{

    field = std::make_shared<procon::Field>(x_size, y_size, max_val, min_val);

    act_stack = std::vector<std::vector<std::tuple<int,int,int>>>(2, std::vector<std::tuple<int,int,int>>(2, std::make_tuple(0, 0, 0) ) );

    if(vis_show){
        visualizer = std::make_shared<Visualizer>(*field);
        connect(visualizer.get(), &Visualizer::nextMove, this, &GameManager::changeMove);
        connect(this, &GameManager::signalAutoMode, visualizer.get(), &Visualizer::slotAutoMode);
        connect(this, &GameManager::setCandidateMove, visualizer.get(), &Visualizer::candidateMove);
    }else{
        is_auto = true;//この場合は自動進行
    }
}

void GameManager::resetManager(const unsigned int x_size, const unsigned int y_size, bool v_show, const int t_max){
    turn_max = t_max;
    vis_show = v_show;

    now_turn = 0;

    field = std::make_shared<procon::Field>(x_size, y_size, max_val, min_val);

    act_stack = std::vector<std::vector<std::tuple<int,int,int>>>(2, std::vector<std::tuple<int,int,int>>(2, std::make_tuple(0, 0, 0) ) );

    if(vis_show){
        visualizer = std::make_shared<Visualizer>(*field);
        connect(visualizer.get(), &Visualizer::nextMove, this, &GameManager::changeMove);
        connect(this, &GameManager::signalAutoMode, visualizer.get(), &Visualizer::slotAutoMode);
        connect(this, &GameManager::setCandidateMove, visualizer.get(), &Visualizer::candidateMove);
    }else{
        is_auto = true;//この場合は自動進行
    }

}

void GameManager::setField(const procon::Field &pro, int now_t, int max_t){

    now_turn = now_t;
    turn_max = max_t;
    field = std::make_shared<procon::Field>(pro);
}

void GameManager::startSimulation(QString my_algo, QString opponent_algo) {

   // std::shared_ptr<GameManager> share(this);

    if (QString::compare("DummyAlgorithm", my_algo) == 0) {
        team_1 = std::make_shared<DummyAlgorithm>(share);
    } else if (QString::compare("GeneticAlgo", my_algo) == 0) {
        team_1 = std::make_shared<GeneticAlgo>(share);
    } else if (QString::compare("MontecarloWithAlgo", my_algo) == 0) {
        team_1 = std::make_shared<MontecarloWithAlgo>(share);
    }else if(QString::compare("SimpleAlgorithm", my_algo) == 0){
        team_1 = std::make_shared<SimpleAlgorithm>(share);
    }else if(QString::compare("BeamSearch", my_algo) == 0){
        team_1 = std::make_shared<beamsearch>(share);
    }

    if (QString::compare("DummyAlgorithm", opponent_algo) == 0) {
        team_2 = std::make_shared<DummyAlgorithm>(team_1->getManagerPtr());
    } else if (QString::compare("GeneticAlgo", opponent_algo) == 0) {
        team_2 = std::make_shared<GeneticAlgo>(team_1->getManagerPtr());
    } else if (QString::compare("MontecarloWithAlgo", opponent_algo) == 0) {
        team_2 = std::make_shared<MontecarloWithAlgo>(team_1->getManagerPtr());
    } else if (QString::compare("SimpleAlgorithm", opponent_algo) == 0) {
        team_2 = std::make_shared<SimpleAlgorithm>(team_1->getManagerPtr());
    }else if(QString::compare("BeamSearch", opponent_algo)==0){
        team_2 = std::make_shared<beamsearch>(team_1->getManagerPtr());
    }

    field = std::make_shared<procon::Field>(field->getSize().first, field->getSize().second, max_val, min_val);




    // progressdockは一旦表示しない事にします(使う事があまりないため)
    /*
    progresdock = std::make_shared<ProgresDock>();
    field_vec.push_back(std::make_shared<procon::Field>(*field));
    progresdock->addAnswer(*(field_vec.back()));
    */

    if(vis_show){
        visualizer->update();
        visualizer->setField(*field, 0, turn_max);
    }


    //うぇーいｗｗｗｗｗｗｗ
    if(is_auto){
        now_turn = 0;
        for(; now_turn < turn_max; ++now_turn){


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

//            progresdock->addAnswer(*(field_vec.back()));



            setFieldCount(field_vec.size() - 1);
        }

        now_turn = -1;

        // progresdock->show();

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

int GameManager::simulationGenetic(const GeneticAgent &agent_1, const GeneticAgent &agent_2, int algo_number){

    //std::cout << "simulationGenetic" << std::endl;

    if(algo_number == 0){
        team_1 = std::make_shared<GeneticAlgo>(share, agent_1);
        team_2 = std::make_shared<GeneticAlgo>(share, agent_2);
    }
    if(algo_number == 2){
        team_1 = std::make_shared<SimpleAlgorithm>(share, agent_1);
        team_2 = std::make_shared<SimpleAlgorithm>(share, agent_2);
    }

    for(; now_turn < turn_max; ++now_turn){


        std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> team_1_ans;// = team_1->agentAct(0);
        std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> team_2_ans;// = team_2->agentAct(1);

        /* マルチスレッド用
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

    }

    // todo: ここで点数計算を行い勝率を出す
    int point_1 = 0;
    int point_2 = 0;
    for(int x = 0; x < field->getSize().first; ++x)
        for(int y = 0; y < field->getSize().second; ++y){
            if(field->getState(x, y).first == 1)
                point_1 += field->getState(x, y).second;
            else if(field->getState(x, y).first == 2)
                point_2 += field->getState(x, y).second;
        }

    /*
    if(point_1 != point_2)
        std::cout << point_1 << " , " << point_2 << std::endl;
    */

    if(point_1 == point_2)return -1;
    return (point_1 > point_2 ? 1 : 0);
}

procon::Field& GameManager::getField(){
    return *field;
}

unsigned int GameManager::getFieldCount(){
    return now_field;
}
void GameManager::setFieldCount(const unsigned int number){
    if(number >= field_vec.size())return ;
    now_field = number;
    if(vis_show){
        visualizer->setField(*field_vec.at(number), number+1, turn_max);
        visualizer->update();
        visualizer->repaint();
    }
}

unsigned int GameManager::getFinalTurn(){
    return turn_max;
}

bool GameManager::canPut(const unsigned int side, const unsigned int move_1, const unsigned int move_2){

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    auto check_outofrange = [&](int agent){

        std::pair<int,int> agent_pos = field->getAgent(side, agent);

        int move = (agent == 0 ? move_1 : move_2);

        agent_pos.first += x_list.at(move);
        agent_pos.second += y_list.at(move);


        return !(agent_pos.first < 0 || agent_pos.second < 0 || agent_pos.first >= field->getSize().first || agent_pos.second >= field->getSize().second);
    };

    auto check_conflict = [&]{

        std::pair<int,int> agent_pos_1 = field->getAgent(side, 0);

        if(field->getState(agent_pos_1.first + x_list.at(move_1), agent_pos_1.second + y_list.at(move_1) ).first != (side == 0 ? 2 : 1) ){

            agent_pos_1.first += x_list.at(move_1);
            agent_pos_1.second += y_list.at(move_1);
        }

        std::pair<int,int> agent_pos_2 = field->getAgent(side, 1);

        if(field->getState(agent_pos_2.first + x_list.at(move_2), agent_pos_2.second + y_list.at(move_2) ).first != (side == 0 ? 2 : 1) ){

            agent_pos_2.first += x_list.at(move_2);
            agent_pos_2.second += y_list.at(move_2);
        }

        return (agent_pos_1 != agent_pos_2);
    };


    return ( check_outofrange(0) && check_outofrange(1) && check_conflict());
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

    //[(x,y)]:(上書き時の色,(色,エージェント)) わかりづらいね
    std::map<std::pair<int,int>,std::pair<int,std::pair<int,int>>> counts;

    int type, pos_x, pos_y;

    for(int side = 0; side < 2; ++side){
        for(int agent = 0; agent < 2; ++agent){
            std::tie(type, pos_x, pos_y) = act_stack.at(side).at(agent);

            //移動しないなら
            if(type != 1){
                std::pair<int,int> not_move = field->getAgent(side, agent);
                counts[not_move] = std::make_pair(-1,std::make_pair(-1, -1));
            }

            int color = 0;
            if(type != 2)
                color = side + 1;

            //もう既に存在しているなら
            if(counts.count(std::make_pair(pos_x, pos_y) )){
                counts[std::make_pair(pos_x, pos_y)].first = -1;
                continue;
            }

            counts[std::make_pair(pos_x, pos_y)] = std::make_pair(color ,std::make_pair(side,agent));
        }
    }
    for(auto moves : counts){
        if(moves.second.first == -1)
            continue;

        field->setState(moves.first.first, moves.first.second, moves.second.first);

        if(moves.second.first != 0)
            field->setAgent(moves.second.second.first, moves.second.second.second, moves.first.first, moves.first.second);
    }

    /*
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
    */


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

    std::cout << "turn : " << now_turn+1 << std::endl << std::endl;

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

//     progresdock->addAnswer(*(field_vec.back()));



    setFieldCount(field_vec.size() - 1);

    now_turn++;

    visualizer->update();

    if(now_turn == turn_max){

        now_turn = -1;

        emit signalAutoMode(false);
        // progresdock->show();
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
