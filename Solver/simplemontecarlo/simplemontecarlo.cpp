#include "simplemontecarlo.h"

SimpleMonteCalro::SimpleMonteCalro(std::shared_ptr<GameManager> manager_ptr) :
    AlgorithmWrapper(manager_ptr),
    sim(GameSimulator(manager_ptr->getField()))
{

}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> SimpleMonteCalro::agentAct(int side){

    const procon::Field& field = manager->getField();

    std::vector<int> agent_trial(81, 0);

    sim.setFieldData(field.getField(), field.getAgents());


        for(unsigned int agent_1 = 0; agent_1 < 9; ++agent_1)
            for(unsigned int agent_2 = 0; agent_2 < 9; ++agent_2){
                for(unsigned int count = 0; count < trial; ++count){

                    sim.resetField();

                    if(count == 0 && ( sim.canPut(side, agent_1, agent_2) == false) ){

                        agent_trial.at(agent_1 * 9 + agent_2) = -1;
                        break;
                    }

                    agent_trial.at(agent_1 * 9 + agent_2) += sim.startSimulation(side, agent_1, agent_2, manager->getFinalTurn() - field.getTurnCount());

                }
            }

    //これで最高値のindexを取り出している
    int max_val = *std::max_element(agent_trial.begin(), agent_trial.end() );
    std::vector<int> max_val_vector;

    for(int index = 0; index < 81; ++index)
        if( agent_trial.at(index) == max_val)
            max_val_vector.push_back( index );

    std::random_device rnd;
    std::mt19937 mt(rnd());
    std::uniform_int_distribution<> rand_index(0,max_val_vector.size() - 1);

    int max_move = max_val_vector.at( rand_index(mt) );

    std::vector<int> x_list = {1, 1, 1, 0, 0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::pair<int,int> agent_1_move = std::make_pair( x_list.at( max_move / 9), y_list.at( max_move / 9) );
    std::pair<int,int> agent_2_move = std::make_pair( x_list.at( max_move % 9), y_list.at( max_move % 9) );

    std::pair<int,int> agent_1_pos = std::make_pair( agent_1_move.first + field.getAgent(side, 0).first, agent_1_move.second + field.getAgent(side, 0).second );
    std::pair<int,int> agent_2_pos = std::make_pair( agent_2_move.first + field.getAgent(side, 1).first, agent_2_move.second + field.getAgent(side, 1).second );

    std::tuple<int,int,int> agent_1_tuple = std::make_tuple( (field.getState(agent_1_pos.first, agent_1_pos.second).first == (side == 0 ? 2 : 1 ) ? 2 : 1 ),
                                                             agent_1_move.first, agent_1_move.second);

    std::tuple<int,int,int> agent_2_tuple = std::make_tuple( (field.getState(agent_2_pos.first, agent_2_pos.second).first == (side == 0 ? 2 : 1 ) ? 2 : 1 ),
                                                             agent_2_move.first, agent_2_move.second);


    return std::make_pair(agent_1_tuple, agent_2_tuple);

}
