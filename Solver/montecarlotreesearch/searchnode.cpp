#include "searchnode.h"

int SearchNode::all_pat_try_count = 0;

SearchNode::SearchNode(SearchNode *parent, int side, int try_count) :
    parent(parent),
    side(side),
    try_count(try_count)
{
    can_move_node_list.resize(81);

}

bool SearchNode::trySimulate(GameSimulator *sim, int turn){

    std::random_device rnd;
    std::mt19937 mt(rnd());

    auto random_act = [&](int side){

        std::vector<int> can_move_list;

        for(unsigned int agent_1 = 0; agent_1 < 9; ++agent_1)
            for(unsigned int agent_2 = 0; agent_2 < 9; ++agent_2)

                if( sim->canPut(side, agent_1, agent_2) )
                    can_move_list.push_back( agent_1 * 9 + agent_2 );

        std::uniform_int_distribution<> rand(0, can_move_list.size() - 1);

        int rand_value = can_move_list.at( rand(mt) );

        sim->agentAct(side, 0, rand_value / 9);
        sim->agentAct(side, 1, rand_value % 9);

    };

    if(try_count > threshold){
        expandNode(sim);

        double max_priority = -1;
        int max_priority_move = 0;

        for(unsigned int index = 0; index < can_move_index_list.size(); ++index){

            int value = can_move_index_list.at(index);
            SearchNode *candidate_node = can_move_node_list.at(value);

            int node_try = candidate_node->try_count;
            int node_win = candidate_node->win_count;

            if(node_try == 0){ //試行回数が0なら強制的に
                max_priority_move = value;
                break;
            }

            double priority = 1.0 * node_win / node_try + ( ucb_value * std::sqrt( std::log(all_pat_try_count) / node_try) );

            if(priority > max_priority){
                max_priority = priority;
                max_priority_move = value;
            }

        }

        sim->agentAct(side, 0, can_move_index_list.at(max_priority_move) / 9 );
        sim->agentAct(side, 0, can_move_index_list.at(max_priority_move) % 9 );

        //ここで相手の動き
        random_act((side == 0 ? 1 : 0));

        bool win_flag = can_move_node_list.at(can_move_index_list.at(max_priority_move) )->trySimulate(sim, turn - 1);

        win_count += win_flag;
        return win_flag;
    }

    bool win_flag = sim->randomizedGame(turn - 1, side);
    win_count += win_flag;

    ++try_count;

    return win_flag;
}

void SearchNode::expandNode(GameSimulator *sim){

    for(int count = 0; count < 81; ++count){

        int agent_1_move = count / 9;
        int agent_2_move = count % 9;

        if(sim->canPut(side, agent_1_move, agent_2_move)){

            can_move_index_list.push_back(count);
            can_move_node_list.at(count) = new SearchNode(parent,side) ;
        }

    }

}
