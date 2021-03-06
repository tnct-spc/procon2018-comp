#include "gamesimulator.h"

GameSimulator::GameSimulator(const std::vector<std::vector<int>>& value_data, int now_turn) :
    value_data(value_data),
    now_turn(now_turn)
{
    /*
    field_data = field.getField();
    agent_data = field.getAgents();
    */
    act_stack.resize(2,std::vector<std::tuple<int,int,int>>(2,std::tuple<int,int,int>(0,0,0)));

}

bool GameSimulator::startSimulation(const unsigned int side, const unsigned int agent_1_move, const unsigned int agent_2_move, const unsigned int final){
    final_turn = final;

    if(canPut(side, agent_1_move, agent_2_move) == false)
        return false;

    return randomizedGame(final_turn, side, agent_1_move, agent_2_move);
}

bool GameSimulator::randomizedGame(const int turn, const unsigned int side, const int agent_1_move, const int agent_2_move){
    if(turn < 0){
        //ここで得点計算処理
        int t = 0;
        for(unsigned int i=0;i<field_data.size();++i)
            for(unsigned int j=0;j<field_data.at(0).size();++j){
                if(field_data.at(i).at(j) == (side == 0 ? 1 : 2))
                    t+=value_data.at(i).at(j);
                if(field_data.at(i).at(j) == (side == 0 ? 2 : 1))
                    t-=value_data.at(i).at(j);
            }
        return t > 0;
        //return true;
    }

    std::random_device rnd;
    std::mt19937 mt(rnd());

    auto random_act = [&](int side){

        std::vector<int> can_move_list;

        for(unsigned int agent_1 = 0; agent_1 < 9; ++agent_1)
            for(unsigned int agent_2 = 0; agent_2 < 9; ++agent_2)

                if( canPut(side, agent_1, agent_2) )
                    can_move_list.push_back( agent_1 * 9 + agent_2 );

        std::uniform_int_distribution<> rand(0, can_move_list.size() - 1);

        int rand_value = can_move_list.at( rand(mt) );

        agentAct(side, 0, x_list.at(rand_value / 9), y_list.at(rand_value / 9) );
        agentAct(side, 1, x_list.at(rand_value % 9), y_list.at(rand_value % 9) );

    };

    if(agent_1_move != -1){

        agentAct(side, 0, x_list.at(agent_1_move), y_list.at(agent_1_move) );
        agentAct(side, 1, x_list.at(agent_2_move), y_list.at(agent_2_move) );
    }else
        random_act(side);

    random_act( ( side == 0 ? 1 : 0 ) );

    changeTurn();

    return randomizedGame(turn - 1, side);
}

bool GameSimulator::canPut(const unsigned int side, const unsigned int move_1, const unsigned int move_2){

    auto check_outofrange = [&](int agent){

        std::pair<int,int> agent_pos = agent_data.at(side).at(agent);
        int move = (agent == 0 ? move_1 : move_2);

        agent_pos.first += x_list.at(move);
        agent_pos.second += y_list.at(move);


        return !(agent_pos.first < 0 || agent_pos.second < 0 || agent_pos.first >= static_cast<int>(field_data.size()) || agent_pos.second >= static_cast<int>(field_data.at(0).size()));
    };

    auto check_conflict = [&]{

        std::pair<int,int> agent_pos_1 = agent_data.at(side).at(0);

        if(field_data.at(agent_pos_1.first + x_list.at(move_1) ).at(agent_pos_1.second + y_list.at(move_1) ) != (side == 0 ? 2 : 1) ){

            agent_pos_1.first += x_list.at(move_1);
            agent_pos_1.second += y_list.at(move_1);
        }

        std::pair<int,int> agent_pos_2 = agent_data.at(side).at(1);

        if(field_data.at(agent_pos_2.first + x_list.at(move_2) ).at( agent_pos_2.second + y_list.at(move_2) ) != (side == 0 ? 2 : 1) ){

            agent_pos_2.first += x_list.at(move_2);
            agent_pos_2.second += y_list.at(move_2);
        }

        return (agent_pos_1 != agent_pos_2);
    };


    return ( check_outofrange(0) && check_outofrange(1) && check_conflict());
}

void GameSimulator::resetField(const std::vector<std::vector<int> > &inp_field, const std::vector<std::vector<std::pair<int, int> > > &inp_agent){
    field_data = inp_field;
    agent_data = inp_agent;
}

void GameSimulator::agentAct(const int turn, const int agent, const int x_inp, const int y_inp){

    std::pair<int,int> agent_pos = agent_data.at(turn).at(agent);
    std::pair<int,int> grid_size = std::make_pair(field_data.size(), field_data.at(0).size());
    //field.getSize();


    int type = (field_data.at(agent_pos.first).at(agent_pos.second) == (turn == 0 ? 2 : 1) ? 2 : 1 );


    int x_pos = agent_pos.first + x_inp;
    int y_pos = agent_pos.second + y_inp;

    if(
        x_pos < 0 || x_pos >= grid_size.first ||
        y_pos < 0 || y_pos >= grid_size.second ||
        (type == 1 && field_data.at(x_pos).at(y_pos) == (turn==1 ? 1 : 2)) ||
        (type == 2 && field_data.at(x_pos).at(y_pos) != (turn==1 ? 1 : 2))
        ){
        act_stack.at(turn).at(agent) = std::make_tuple(0, 0, 0);
        return ;
    }
    act_stack.at(turn).at(agent) = std::make_tuple(type, x_pos, y_pos);

}

void GameSimulator::agentAct(const int turn, const int agent, const int move){
    agentAct(turn, agent, x_list.at(move), y_list.at(move) );
}

void GameSimulator::changeTurn(){

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

        if(field_data.at(elements.first.first).at(elements.first.second) == (elements.second.at(0).first == 0 ? 2 : 1))
            continue;

        agent_data.at(elements.second.at(0).first).at(elements.second.at(0).second) = std::make_pair(elements.first.first, elements.first.second);
        field_data.at(elements.first.first).at(elements.first.second) = elements.second.at(0).first + 1;
    }

    for(auto elements : tile_map){
        bool state_flag = true;
        if(elements.second.size() > 1)
            continue;


        for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
            for(int agent_num = 0; agent_num < 2; ++agent_num)
                if(agent_data.at(turn_flag).at(agent_num) == elements.first){
                    state_flag = false;
                    break;
                }
        if(state_flag)
            field_data.at(elements.first.first).at(elements.first.second) =  0;
    }
}
