#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "testalgorithm.h"
#include "simplemontecarlo/simplemontecarlo.h"
#include "montecarlotreesearch/montecarlotreesearch.h"
#include "dummyalgorithm.h"
#include "simplemontecarlo/montecarlowithalgo.h"
#include "BreadthFirstSearch/beamsearch.h"
#include "geneticalgo/simplealgorithm.h"
#include "doubleagent/agentmanager.h"
GameManager::GameManager(const unsigned int x_size, const unsigned int y_size, bool vis_show, const int turn_max, QObject *parent)
    : QObject(parent),
    vis_show(vis_show)
{

   field = std::make_shared<procon::Field>(x_size, y_size, max_val, min_val);
   field->setFinalTurn(turn_max);

    act_stack = std::vector<std::vector<std::tuple<int,int,int>>>(2, std::vector<std::tuple<int,int,int>>(2, std::make_tuple(0, 0, 0) ) );

    if(vis_show){
        visualizer = std::make_shared<Visualizer>(*field);
        visualizer->show();
        connect(visualizer.get(), &Visualizer::nextMove, this, &GameManager::changeMove);
        connect(this, &GameManager::signalAutoMode, visualizer.get(), &Visualizer::slotAutoMode);
        connect(this, &GameManager::setCandidateMove, visualizer.get(), &Visualizer::candidateMove);
        connect(visualizer.get(), &Visualizer::selectChangeGrid, this, &GameManager::getDataToOperator);
        connect(this, &GameManager::sendDataToVisualizer, visualizer.get(), &Visualizer::getData);

    }else{
        is_auto = true;//この場合は自動進行
    }
}

void GameManager::resetManager(const unsigned int x_size, const unsigned int y_size, bool v_show, const int t_max){

    vis_show = v_show;

    field = std::make_shared<procon::Field>(x_size, y_size, max_val, min_val);
    field->setFinalTurn(t_max);

    act_stack = std::vector<std::vector<std::tuple<int,int,int>>>(2, std::vector<std::tuple<int,int,int>>(2, std::make_tuple(0, 0, 0) ) );

    if(vis_show){
        visualizer = std::make_shared<Visualizer>(*field);
        connect(visualizer.get(), &Visualizer::nextMove, this, &GameManager::changeMove);
        connect(this, &GameManager::signalAutoMode, visualizer.get(), &Visualizer::slotAutoMode);
        connect(this, &GameManager::setCandidateMove, visualizer.get(), &Visualizer::candidateMove);
        connect(visualizer.get(), &Visualizer::selectChangeGrid, this, &GameManager::getDataToOperator);
        connect(this, &GameManager::sendDataToVisualizer, visualizer.get(), &Visualizer::getData);

    }else{
        is_auto = true;//この場合は自動進行
    }

    field->updatePoint();

}

void GameManager::setField(const procon::Field &pro, int now_t, int max_t){

    field->resetState(pro);

    field->setTurnCount(now_t);
    field->setFinalTurn(max_t);
}

void GameManager::startSimulation(QString my_algo, QString opponent_algo,QString InputMethod) {

    if (QString::compare("GenerateField", InputMethod) == 0) {

        int final_turn = field->getFinalTurn();
        field = std::make_shared<procon::Field>(field->getSize().first, field->getSize().second, max_val, min_val);
        field->setFinalTurn(final_turn);
        field_vec.clear();
        field_vec.push_back(std::make_shared<procon::Field>(*field));

     //   std::string path = QFileDialog::getOpenFileName().toStdString();
     //   procon::CsvIo::exportField(*field,path);

    } else if (QString::compare("CSVImport", InputMethod) == 0) {

        std::string path = QFileDialog::getOpenFileName().toStdString();
        field = std::make_shared<procon::Field>(procon::CsvIo::importField(path));

    } else if (QString::compare("QRcode", InputMethod) == 0) {
        QRCode qr;
        QrConverterField qrc;
        std::string f = qr.decodeQRcode();
        procon::Field hoge = qrc.ConvertCsvToField(f);
        field = std::make_shared<procon::Field>(hoge);
        field_vec.clear();
        field_vec.push_back(std::make_shared<procon::Field>(*field));
    }else if (QString::compare("BinaryImport", InputMethod) == 0) {
        std::string path = QFileDialog::getOpenFileName().toStdString();
        field = std::make_shared<procon::Field>(procon::BinaryIo::importField(path));
    }
    //field->guessAgents(1);

    if (QString::compare("DummyAlgorithm", my_algo) == 0) {
        team_1 = std::make_shared<DummyAlgorithm>(*field, field->getFinalTurn(), 0);
    } else if (QString::compare("GeneticAlgo", my_algo) == 0) {
        team_1 = std::make_shared<GeneticAlgo>(*field, field->getFinalTurn(), 0);
    } else if (QString::compare("MontecarloWithAlgo", my_algo) == 0) {
        team_1 = std::make_shared<MontecarloWithAlgo>(*field, field->getFinalTurn(), 0);
    }else if(QString::compare("SimpleAlgorithm", my_algo) == 0){
        team_1 = std::make_shared<SimpleAlgorithm>(*field, field->getFinalTurn(), 0);
    }else if(QString::compare("BeamSearch", my_algo) == 0){
        team_1 = std::make_shared<beamsearch>(*field, field->getFinalTurn(), 0);
    }else if(QString::compare("TestDoubleAgentAlgo", my_algo) == 0){
        team_1 = std::make_shared<AgentManager>(*field, field->getFinalTurn(), 0, 0);
    }else if(QString::compare("DoubleAgentWithSimpleMC", my_algo) == 0){
        team_1 = std::make_shared<AgentManager>(*field, field->getFinalTurn(), 0, 0|(1<<16));
    }else if(QString::compare("DoubleAgentWithUniformMC", my_algo) == 0){
        team_1 = std::make_shared<AgentManager>(*field, field->getFinalTurn(), 0, 0|(2<<16));
    }else if(QString::compare("DoubleAgentWithNash", my_algo) == 0){
        team_1 = std::make_shared<AgentManager>(*field, field->getFinalTurn(), 0, 0|(3<<16));
    }else if(QString::compare("EvaluateParam", my_algo) == 0){
        team_1 = std::make_shared<AgentManager>(*field, field->getFinalTurn(), 0, 1);
    }

    if (QString::compare("DummyAlgorithm", opponent_algo) == 0) {
        team_2 = std::make_shared<DummyAlgorithm>(*field, field->getFinalTurn(), 1);
    } else if (QString::compare("GeneticAlgo", opponent_algo) == 0) {
        team_2 = std::make_shared<GeneticAlgo>(*field, field->getFinalTurn(), 1);
    } else if (QString::compare("MontecarloWithAlgo", opponent_algo) == 0) {
        team_2 = std::make_shared<MontecarloWithAlgo>(*field, field->getFinalTurn(), 1);
    } else if (QString::compare("SimpleAlgorithm", opponent_algo) == 0) {
        team_2 = std::make_shared<SimpleAlgorithm>(*field, field->getFinalTurn(), 1);
    }else if(QString::compare("BeamSearch", opponent_algo)==0){
        team_2 = std::make_shared<beamsearch>(*field, field->getFinalTurn(), 1);
    }else if(QString::compare("TestDoubleAgentAlgo", opponent_algo) == 0){
        team_2 = std::make_shared<AgentManager>(*field, field->getFinalTurn(), 1, 0);
    }else if(QString::compare("DoubleAgentWithSimpleMC", opponent_algo) == 0){
        team_2 = std::make_shared<AgentManager>(*field, field->getFinalTurn(), 1, 0|(1<<16));
    }else if(QString::compare("DoubleAgentWithUniformMC", opponent_algo) == 0){
        team_2 = std::make_shared<AgentManager>(*field, field->getFinalTurn(), 1, 0|(2<<16));
    }else if(QString::compare("DoubleAgentWithNash", opponent_algo) == 0){
        team_2 = std::make_shared<AgentManager>(*field, field->getFinalTurn(), 1, 0|(3<<16));
    }else if(QString::compare("EvaluateParam", opponent_algo) == 0){
        team_2 = std::make_shared<AgentManager>(*field, field->getFinalTurn(), 1, 1);
    }


    // progressdockは一旦表示しない事にします(使う事があまりないため)
    /*
    progresdock = std::make_shared<ProgresDock>();
    field_vec.push_back(std::make_shared<procon::Field>(*field));
    progresdock->addAnswer(*(field_vec.back()));
    */

    if(vis_show){
        visualizer->update();
        visualizer->setField(*field, field->getTurnCount(), field->getFinalTurn());
    }


    //うぇーいｗｗｗｗｗｗｗ
    if(is_auto){
        // field->setTurnCount(0);
        setFieldCount(field_vec.size() - 1);
        while(field->getTurnCount() < field->getFinalTurn()){


            //std::cout << "turn " << now_turn + 1 << " started" << std::endl << std::endl;

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

            std::vector<std::pair<std::pair<int,int>, std::pair<int,int>>> pruning_pos;


            std::pair<int,int> pruning = std::make_pair(std::get<1>(team_1_ans.first) + field->getAgent(0,0).first, std::get<2>(team_1_ans.first) + field->getAgent(0,0).second);

            pruning_pos.push_back(std::make_pair(std::make_pair(0, std::get<0>(team_1_ans.first) - 1), pruning));
            pruning = std::make_pair(std::get<1>(team_1_ans.second) + field->getAgent(0,1).first, std::get<2>(team_1_ans.second) + field->getAgent(0,1).second);
            pruning_pos.push_back(std::make_pair(std::make_pair(0, std::get<0>(team_1_ans.second) - 1), pruning));
            pruning = std::make_pair(std::get<1>(team_2_ans.first) + field->getAgent(1,0).first, std::get<2>(team_2_ans.first) + field->getAgent(1,0).second);
            pruning_pos.push_back(std::make_pair(std::make_pair(1, std::get<0>(team_2_ans.first) - 1), pruning));
            pruning = std::make_pair(std::get<1>(team_2_ans.second) + field->getAgent(1,1).first, std::get<2>(team_2_ans.second) + field->getAgent(1,1).second);
            pruning_pos.push_back(std::make_pair(std::make_pair(1, std::get<0>(team_2_ans.second) - 1), pruning));

            std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> test;
            test = team_1_ans;


            agentAct(0,0,team_1_ans.first);
            agentAct(0,1,team_1_ans.second);
            agentAct(1,0,team_2_ans.first);
            agentAct(1,1,team_2_ans.second);

            changeTurn(false);
            field->getPoints(pruning_pos);

            field_vec.push_back(std::make_shared<procon::Field>(*field));

//            progresdock->addAnswer(*(field_vec.back()));


            setFieldCount(field_vec.size() - 1);
        }

        // procon::CsvIo::exportField(*field, "../../field.csv");

        // progresdock->show();

    }else{

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

int GameManager::simulationGenetic(const GeneticAgent &agent_1, const GeneticAgent &agent_2, int algo_number, const GeneticAgent& agent_3, const GeneticAgent& agent_4){

    //std::cout << "simulationGenetic" << std::endl;

    // changeTurn後の得点計算を行うか
    bool is_update = true;

    if(algo_number == 0){
        team_1 = std::make_shared<GeneticAlgo>(*field, field->getFinalTurn(), 0, agent_1);
        team_2 = std::make_shared<GeneticAlgo>(*field, field->getFinalTurn(), 1, agent_2);
        is_update = false;
    }
    if(algo_number == 2){
        team_1 = std::make_shared<SimpleAlgorithm>(*field, field->getFinalTurn(), 0, agent_1);
        team_2 = std::make_shared<SimpleAlgorithm>(*field, field->getFinalTurn(), 1, agent_2);
    }
    if(algo_number == 3){
        team_1 = std::make_shared<AgentManager>(*field, field->getFinalTurn(), 0, 0, &agent_1, &agent_2);
        team_2 = std::make_shared<AgentManager>(*field, field->getFinalTurn(), 1, 0, &agent_3, &agent_4);
    }


    field->updatePoint();


    while(field->getTurnCount() < field->getFinalTurn()){


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

        changeTurn(is_update);

    }

    if(!is_update)field->updatePoint();

    std::pair<int,int> point_1_pair = field->getPoints(false).at(0);
    std::pair<int,int> point_2_pair = field->getPoints(false).at(1);


    int point_1 = point_1_pair.first + point_1_pair.second;
    int point_2 = point_2_pair.first + point_2_pair.second;


    if(point_1 == point_2)return -1;
    return (point_1 > point_2 ? 1 : 0);
}

procon::Field& GameManager::getField(){
    return (*field);
}

unsigned int GameManager::getFieldCount(){
    return now_field;
}
void GameManager::setFieldCount(const unsigned int number){
    if(number >= field_vec.size())return ;
    now_field = number;
    if(vis_show){
        visualizer->setField(*field_vec.at(number), number, field->getFinalTurn());
        visualizer->update();
        visualizer->repaint();
    }
}

unsigned int GameManager::getFinalTurn(){
    return field->getFinalTurn();
}


void GameManager::agentAct(const int turn, const int agent, const std::tuple<int, int, int> tuple_val){

    int type, x_inp, y_inp;
    std::tie(type, x_inp, y_inp) = tuple_val;

    std::pair<int,int> agent_pos = field->getAgent(turn, agent);
    std::pair<int,int> grid_size = field->getSize();
    int x_pos = agent_pos.first + x_inp;
    int y_pos = agent_pos.second + y_inp;

    if(
        type == 0 ||
        x_pos < 0 || x_pos >= grid_size.first ||
        y_pos < 0 || y_pos >= grid_size.second ||
        (type == 1 && field->getState(x_pos, y_pos).first == (turn==1 ? 1 : 2)) ||
        (type == 2 && field->getState(x_pos, y_pos).first == 0)
        ){
        act_stack.at(turn).at(agent) = std::make_tuple(1, agent_pos.first, agent_pos.second);
        return ;
    }
    act_stack.at(turn).at(agent) = std::make_tuple(type, x_pos, y_pos);
}
void GameManager::changeTurn(bool update){

    //[(x,y)]:(上書き時の色,(色,エージェント)) わかりづらいね
    std::map<std::pair<int,int>,std::pair<int,std::pair<int,int>>> counts;

    int type, pos_x, pos_y;


    //移動しようとしたエージェントが失敗した時に呼ばれる
    std::function<void(std::pair<int,int>)> delete_move = [&](std::pair<int,int> agent_data){

        std::pair<int,int> not_move = field->getAgent(agent_data.first, agent_data.second);

        //もう既に埋まっていて、それが移動予定erなら

        if(counts[not_move].first >= 0){
          
            std::pair<int,int> next_delete_move = counts[not_move].second;

            counts[not_move] = std::make_pair(-1, std::make_pair(-1, -1));
            //循環参照ケースの回避
            if(next_delete_move != agent_data)
                delete_move(next_delete_move);
        }

        counts[not_move] = std::make_pair(-1, std::make_pair(-1, -1));
    };


    for(int side = 0; side < 2; ++side){
        for(int agent = 0; agent < 2; ++agent){
            std::tie(type, pos_x, pos_y) = act_stack.at(side).at(agent);

            //移動しないなら
            if(type != 1){
                std::pair<int,int> not_move = field->getAgent(side, agent);

                if(counts[not_move].first > 0){//移動しようとしているアレのコンフリクト
                    delete_move(counts[not_move].second);
                    delete_move(std::make_pair(side, agent));
                }

                counts[not_move] = std::make_pair(-1,std::make_pair(-1, -1));
            }

            //もう既に存在しているなら
            if(counts.count(std::make_pair(pos_x, pos_y) )){

                if(counts[std::make_pair(pos_x, pos_y)].first > 0){
                    delete_move(counts[std::make_pair(pos_x, pos_y)].second);
                    delete_move(std::make_pair(side, agent));
                }

                counts[std::make_pair(pos_x, pos_y)] = std::make_pair(-1, std::make_pair(-1, -1));
            }else{
                int color = 0;
                if(type != 2)
                    color = side + 1;

                counts[std::make_pair(pos_x, pos_y)] = std::make_pair(color ,std::make_pair(side,agent));
            }
        }
    }

    for(auto moves : counts){
        if(moves.second.first == -1)
            continue;

        field->setState(moves.first.first, moves.first.second, moves.second.first);

        if(moves.second.first != 0)
            field->setAgent(moves.second.second.first, moves.second.second.second, moves.first.first, moves.first.second);
    }

    field->setTurnCount(field->getTurnCount() + 1);

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

    //得点の更新処理(エージェント側でやるよりこちらの方がよい)
    if(update)
        field->updatePoint();


}

void GameManager::setAutoMode(bool value){
    is_auto = value;
}

int GameManager::getTurnCount(){
    return field->getTurnCount();
}

void GameManager::changeMove(const std::vector<std::vector<std::pair<int, int>>>& move, std::vector<std::vector<int>> is_delete){
    //is_deleteは自軍タイル除去時にのみ使う物 基本的に使わなさそう

    std::cout << "turn : " << field->getTurnCount()+1 << std::endl << std::endl;

    for(int side = 0; side < 2; ++side)
        for(int agent = 0; agent < 2; ++agent){

            std::pair<int,int> origin_pos = field->getAgent(side, agent);

            std::pair<int,int> pos = move.at(side).at(agent);

            std::pair<int,int> new_pos = pos;

            new_pos.first -= origin_pos.first;
            new_pos.second -= origin_pos.second;

            //is_deleteなら強制的に削除
            agentAct(side, agent,  std::make_tuple( ( is_delete.at(side).at(agent) || (field->getState(pos.first, pos.second).first == (side == 0 ? 2 : 1)) ? 2 : 1 ), new_pos.first, new_pos.second ) );

        }

    changeTurn();

    field_vec.push_back(std::make_shared<procon::Field>(*field));

//     progresdock->addAnswer(*(field_vec.back()));



    setFieldCount(field_vec.size() - 1);

    visualizer->update();

    if(field->getTurnCount() == field->getFinalTurn()){

        emit signalAutoMode(false);
        // progresdock->show();
    }else
        nextMoveForManualMode();

}

void GameManager::nextMoveForManualMode(){

    visualizer->update();
    visualizer->repaint();

//    std::cout << field->getTurnCount() << "," << field->getFinalTurn() << std::endl;

//    std::pair<int, int> agent = field->getAgent(0,0);
//    std::cout << agent.first << "," << agent.second << std::endl;

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

void GameManager::startupChangeMode()
{
    ope = std::make_shared<Operator>();

    connect(ope.get(), &Operator::pushEnd, this, &GameManager::endChangeMode);
    connect(this, &GameManager::sendDataToOperator, ope.get(), &Operator::changeDataDisplay);
    connect(ope.get(), &Operator::pushChange, this, &GameManager::getChangeOfData);

    // Operatorを表示
    ope->show();
    ope->setTurns(field->getTurnCount(), field->getFinalTurn());

    // VisualizerをChangeModeに変更
    visualizer->setChangeMode(true);
    visualizer->update();
}

void GameManager::endChangeMode(const std::pair<int, int> turns)
{
    // Turnをセット
    visualizer->setTurns(turns);

    // VisualizerのChangeModeを解除
    visualizer->setChangeMode(false);

    // Operatorを閉じる
    ope->close();

    // Fieldの書き換え
    *field = visualizer->getField();
    field->updatePoint();

    // ゲームを続行
    nextMoveForManualMode();
}

void GameManager::getDataToOperator(const std::pair<int,int> grid, const bool agent)
{
    std::pair<int, int> data;

    // 変更するのがエージェントならグリッドの座標をそのまま送る
    if (agent) {
        data.first = grid.first;
        data.second = grid.second;
    } else {

        // グリッドならそのグリッドのステータスを送る
        data = field->getState(grid.first, grid.second);
    }

    emit sendDataToOperator(data, agent);
}

void GameManager::getChangeOfData(const std::pair<int, int> data, const bool agent)
{
    emit sendDataToVisualizer(data, agent);
}
