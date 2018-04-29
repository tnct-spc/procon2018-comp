#include "montecarlotreesearch.h"

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> MonteCarloTreeSearch::agentAct(int side){

    SearchNode root_node(nullptr, side, 100000);//try_countには閾値より大きな値を渡す

    const procon::Field& field = manager->getField();

    std::vector<std::vector<int>> field_data = field.getField();
    std::vector<std::vector<std::pair<int,int>>> agent_data = field.getAgents();


    GameSimulator sim(manager->getField().getValue(), manager->getTurnCount());

    clock_t start_time = clock();
    int count = 0;
    while(++count){
        clock_t now_time = clock();

        if(try_time < static_cast<double>(now_time - start_time) )
            break;

        sim.resetField(field_data, agent_data);

        root_node.trySimulate(&sim, manager->getFinalTurn() - manager->getTurnCount());

    }

    std::cout << "montecarlotreesearch...try_count : " << count << std::endl;

    std::vector<int> index_list = root_node.can_move_index_list;

    int max_value = 0;
    int max_win = 0;
    int max_index = index_list.front();

    std::vector<int> x_list = {1, 1, 1, 0, 0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    for(unsigned int count = 0; count < index_list.size(); ++count){

        root_node.all_pat_try_count = count + 1;

        int index = index_list.at(count);

        int value = root_node.can_move_node_list.at(index)->try_count;

        int win_count = root_node.can_move_node_list.at(index)->win_count;


        //動かない場合の特別処理(優先度を低めたい)
        if(index / 9 == 8 || index % 9 == 8){
            value = value * 2 / 3;
        }
        //マイナスを踏む場合の特別処理
        std::pair<int,int> state_1 = field.getState(field.getAgent(side, 0).first + x_list.at(max_index / 9), field.getAgent(side, 0).second + y_list.at(max_index / 9));
        std::pair<int,int> state_2 = field.getState(field.getAgent(side, 1).first + x_list.at(max_index % 9), field.getAgent(side, 1).second + y_list.at(max_index % 9));

        if(state_1.first == 0 && state_1.second < 0)
            value = value * 2 / 3;
        if(state_2.first == 0 && state_2.second < 0)
            value = value * 2 / 3;
        if(state_1.first == side + 1)
            value = value * 5 / 6;
        if(state_2.first == side + 1)
            value = value * 5 / 6;


//        std::cout << "index : " << index << "value : " << value << std::endl;

        if(max_value < value){
            max_value = value;
            max_win = win_count;
            max_index = index;

        }else if(max_value == value && max_win < win_count)
            max_index = index;
    }


    std::cout << "max_try : " << max_value << ", win_count : " << max_win << ", move :  " << max_index << std::endl << std::endl;


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
