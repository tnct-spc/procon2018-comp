#include "montecarlotreesearch.h"

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> MonteCarloTreeSearch::agentAct(int side){

    SearchNode root_node(nullptr, side, 100000);//try_countには閾値より大きな値を渡す

    const procon::Field& field = manager->getField();

    std::vector<std::vector<int>> field_data = field.getField();
    std::vector<std::vector<std::pair<int,int>>> agent_data = field.getAgents();


    GameSimulator sim(manager->getField());

    for(int count = 0; count < trial; ++count){

        root_node.all_pat_try_count = count;

        sim.setFieldData(field_data, agent_data);
        root_node.trySimulate(&sim, manager->getFinalTurn() - manager->getField().getTurnCount());

    }

    std::vector<int> index_list = root_node.can_move_index_list;

    int max_value = 0;
    int max_index = index_list.front();

    for(unsigned int count = 0; count < index_list.size(); ++count){

        int index = index_list.at(count);

        int value = root_node.can_move_node_list.at(index)->try_count;

        if(max_value < value){
            max_value = value;
            max_index = index;
        }
    }


    std::vector<int> x_list = {1, 1, 1, 0, 0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::pair<int,int> agent_1_move = std::make_pair( x_list.at( max_index / 9), y_list.at( max_index / 9) );
    std::pair<int,int> agent_2_move = std::make_pair( x_list.at( max_index % 9), y_list.at( max_index % 9) );

    std::pair<int,int> agent_1_pos = std::make_pair( agent_1_move.first + field.getAgent(side, 0).first, agent_1_move.second + field.getAgent(side, 0).second );
    std::pair<int,int> agent_2_pos = std::make_pair( agent_2_move.first + field.getAgent(side, 1).first, agent_2_move.second + field.getAgent(side, 1).second );

    std::tuple<int,int,int> agent_1_tuple = std::make_tuple( (field.getState(agent_1_pos.first, agent_1_pos.second).first == (side == 0 ? 2 : 1 ) ? 2 : 1 ),
                                                             agent_1_move.first, agent_1_move.second);

    std::tuple<int,int,int> agent_2_tuple = std::make_tuple( (field.getState(agent_2_pos.first, agent_2_pos.second).first == (side == 0 ? 2 : 1 ) ? 2 : 1 ),
                                                             agent_2_move.first, agent_2_move.second);


    return std::make_pair(agent_1_tuple, agent_2_tuple);

}
