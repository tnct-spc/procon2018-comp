#include "searchnode.h"

#include "montecarlotreesearch.h"

SearchNode::SearchNode(MonteCarloTreeSearch *tree, SearchNode *parent, std::vector<std::pair<int,int>> agents, int side, int x_inp, int y_inp) :
    tree(tree),
    parent(parent),
    agents(agents),
    field(tree->getManagerPtr()->getField())
{
    if(size_x != -1){
        size_x = x_inp;
        size_y = y_inp;
    }

}

bool SearchNode::canMove(int move_1, int move_2){

    auto check_outofrange = [&](int agent){

        std::pair<int,int> agent_pos = agents.at(agent);
        int move = (agent == 0 ? move_1 : move_2);

        agent_pos.first += x_list.at(move);
        agent_pos.second += y_list.at(move);


        return !(agent_pos.first < 0 || agent_pos.second < 0 || agent_pos.first >= field.getSize().first || agent_pos.second >= field.getSize().second);
    };

    auto check_conflict = [&]{

        std::pair<int,int> agent_pos_1 = field.getAgent(side, 0);

        if(field.getState(agent_pos_1.first + x_list.at(move_1) , agent_pos_1.second + y_list.at(move_1) ).first != (side == 0 ? 2 : 1) ){

            agent_pos_1.first += x_list.at(move_1);
            agent_pos_1.second += y_list.at(move_1);
        }

        std::pair<int,int> agent_pos_2 = field.getAgent(side, 1);

        if(field.getState(agent_pos_2.first + x_list.at(move_2) , agent_pos_2.second + y_list.at(move_2) ).first != (side == 0 ? 2 : 1) ){

            agent_pos_2.first += x_list.at(move_2);
            agent_pos_2.second += y_list.at(move_2);
        }

        return (agent_pos_1 != agent_pos_2);
    };


    return ( check_outofrange(0) && check_outofrange(1) && check_conflict());
}
