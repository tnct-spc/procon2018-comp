#include "depthfirstsearch.h"

DepthFirstSearch::DepthFirstSearch(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side)
{
    if(dock_show){
        dock = std::make_shared<MinimumVisualizerDock>(6);
        dock->show();
    }
}

const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> DepthFirstSearch::agentAct(int now_turn){


    std::shared_ptr<SearchNode> node_1, node_2;
    std::list<std::pair<int,int>> moves_1, moves_2;
    std::vector<std::vector<int>> states_1, states_2;
    std::tie(node_1, moves_1, states_1) = depthSearch(0, std::min(final_turn - now_turn, maxval));
    std::tie(node_2, moves_2, states_2) = depthSearch(1, std::min(final_turn - now_turn, maxval));

    std::vector<std::vector<double>> after_values(field.getSize().first, std::vector<double>(field.getSize().second, 1.0));

    for(int pos_x = 0; pos_x < field.getSize().first; ++pos_x)
        for(int pos_y = 0; pos_y < field.getSize().second; ++pos_y){
            int pos_state = field.getState(pos_x, pos_y).first;
            if(pos_state)
                after_values.at(pos_x).at(pos_y) = 2 * (pos_state != side + 1);

            after_values.at(pos_x).at(pos_y) -= 1.0 * states_1.at(pos_x).at(pos_y) / node_1->size;
            after_values.at(pos_x).at(pos_y) -= 1.0 * states_2.at(pos_x).at(pos_y) / node_2->size;

            after_values.at(pos_x).at(pos_y) = std::max(0.0, after_values.at(pos_x).at(pos_y));
        }

    std::vector<std::vector<std::vector<int>>> colors(3, std::vector<std::vector<int>>(field.getSize().first, std::vector<int>(field.getSize().second, 255)));
    for(int x_pos = 0; x_pos < field.getSize().first; ++x_pos)
        for(int y_pos = 0; y_pos < field.getSize().second; ++y_pos){
            if(after_values.at(x_pos).at(y_pos) > 1){
                colors.at(0).at(x_pos).at(y_pos) -= 255 * (after_values.at(x_pos).at(y_pos) - 1);
                colors.at(1).at(x_pos).at(y_pos) -= 255 * (after_values.at(x_pos).at(y_pos) - 1);

            }else if(after_values.at(x_pos).at(y_pos) < 1){
                colors.at(1).at(x_pos).at(y_pos) -= 255 * (1 - after_values.at(x_pos).at(y_pos));
                colors.at(2).at(x_pos).at(y_pos) -= 255 * (1 - after_values.at(x_pos).at(y_pos));
            }

            for(int index = 0; index < 3; ++index)
                colors.at(index).at(x_pos).at(y_pos) = std::max(0, colors.at(index).at(x_pos).at(y_pos));
        }

    if(dock_show)
        dock->addVisualizer(field.getSize(), std::vector<std::list<std::pair<int,int>>>({moves_1, moves_2}), colors);

    if(vis_show){
        minimum = std::make_shared<MinimumVisualizer>(field.getSize());

        std::vector<std::list<std::pair<int,int>>> use_vec = std::vector<std::list<std::pair<int,int>>>({moves_1, moves_2});
        minimum->setRoute(use_vec);

        for(int index = 0; index < 3; ++index)
            minimum->setValues(colors.at(index), index);
        minimum->show();
    }


    int move_1 = node_1->getMaxAdvMove().second;
    int move_2 = node_2->getMaxAdvMove().second;

    std::pair<std::pair<int,int>,int> ins = getMaxAdvMove(node_1, node_2);

    move_1 = ins.first.first;
    move_2 = ins.first.second;

    std::vector<std::pair<int,int>> agent_pos(2);
    for(int index = 0; index < 2; ++index)
        agent_pos.at(index) = field.getAgent(side, index);


    std::cout<<agent_pos.at(0).first + SearchNode::dx.at(move_1)<<" "<<agent_pos.at(0).second + SearchNode::dy.at(move_1)<<" "<<agent_pos.at(1).first + SearchNode::dx.at(move_2)<<" "<<agent_pos.at(1).second + SearchNode::dy.at(move_2)<<std::endl;
    std::cout<<" "<<std::endl;

    return std::make_pair(std::make_tuple((field.getState(agent_pos.at(0).first + SearchNode::dx.at(move_1), agent_pos.at(0).second + SearchNode::dy.at(move_1)).first != (side ? 1 : 2) ? 1 : 2), SearchNode::dx.at(move_1), SearchNode::dy.at(move_1)),
                          std::make_tuple((field.getState(agent_pos.at(1).first + SearchNode::dx.at(move_2), agent_pos.at(1).second + SearchNode::dy.at(move_2)).first != (side ? 1 : 2) ? 1 : 2), SearchNode::dx.at(move_2), SearchNode::dy.at(move_2)));
}

std::tuple<std::shared_ptr<DepthFirstSearch::SearchNode>, std::list<std::pair<int,int>>, std::vector<std::vector<int>>> DepthFirstSearch::depthSearch(int agent, int turn_max){
    int size_x, size_y;
    std::tie(size_x, size_y) = field.getSize();

    std::vector<std::vector<int>> state(size_x, std::vector<int>(size_y, 1));
    const std::vector<std::vector<int>>& field_values = field.getValue();

    for(int pos_x = 0; pos_x < size_x; ++pos_x)
        for(int pos_y = 0; pos_y < size_y; ++pos_y){
            int pos_state = field.getState(pos_x, pos_y).first;
            if(pos_state)
                state.at(pos_x).at(pos_y) = 2 * (pos_state != side + 1);
        }

    std::shared_ptr<SearchNode> node = std::make_shared<SearchNode>(0, 0, turn_max, field.getAgent(side, agent), side, field_values, state);

    std::list<std::pair<int,int>> moves;
    std::pair<int,int> now_pos = field.getAgent(side, agent);
    moves.emplace_back(now_pos);
    std::shared_ptr<SearchNode> now_node = node;
    bool is_move = true;
    std::pair<int,int> value(0, 0);

    while(1){
        value = now_node->getMaxAdvMove();
        if(value.first == -1000000007)
            break;

        is_move  = now_node->childs[value.second].second;
        now_node = now_node->childs[value.second].first;

        if(is_move){
            now_pos.first += SearchNode::dx.at(value.second);
            now_pos.second += SearchNode::dy.at(value.second);
        }
        moves.emplace_back(now_pos);
    }

    std::vector<std::vector<int>> values(field.getSize().first, std::vector<int>(field.getSize().second, 0));
    std::vector<std::vector<double>> after_values(field.getSize().first, std::vector<double>(field.getSize().second));

    node->dfsAdd(field.getAgent(side, agent), values);


    return std::make_tuple(node, moves, values);
}

DepthFirstSearch::SearchNode::SearchNode(int adv, int depth, int remain, std::pair<int,int> pos, int side, const std::vector<std::vector<int>>& value, std::vector<std::vector<int>>& state) :
    adv(adv),
    depth(depth)
{
    size = 1;


    // 末尾ノード
    if(!remain)
        return ;

    // {point,is_move}
    auto state_data = [side, pos, value, state](int move_index){
        int x_pos = pos.first + dx.at(move_index);
        int y_pos = pos.second + dy.at(move_index);

        if(x_pos < 0 || y_pos < 0 || x_pos >= state.size() || y_pos >= state.at(0).size() || !state.at(x_pos).at(y_pos))
            return std::make_pair(-1000000007, false);
        return std::make_pair(value.at(x_pos).at(y_pos), state.at(x_pos).at(y_pos) != 2);
    };

    // {{point, ismove}, dir}
    std::vector<std::pair<std::pair<int, bool>, int>> moves;
    for(int index = 0; index < 8; ++index)
        moves.emplace_back(std::make_pair(state_data(index), index));

    std::sort(moves.begin(), moves.end());

    int bound_val = std::max(moves.at(8 - movecount).first.first, 0);

    moves.erase(moves.begin(), std::lower_bound(moves.begin(), moves.end(), std::make_pair(std::make_pair(bound_val, false), 0)));
    if(moves.empty())
        return ;

    size = 0;

    for(auto move : moves){
        std::pair<int,int> new_pos = std::make_pair(pos.first + dx.at(move.second), pos.second + dy.at(move.second));

        --state.at(new_pos.first).at(new_pos.second);

        childs[move.second] = std::make_pair(std::make_shared<SearchNode>(move.first.first, depth + 1, remain - 1, (move.first.second ? new_pos : pos), side, value, state), move.first.second);

        size += childs[move.second].first->size;

        ++state.at(new_pos.first).at(new_pos.second);
    }
}

std::pair<std::pair<int,int>, int> DepthFirstSearch::getMaxAdvMove(std::shared_ptr<SearchNode> age1, std::shared_ptr<SearchNode> age2){

    int sikiiti = threshold;

    long long rearch = 1;
    for(int a = 0;a < maxval;a++)rearch *= age1->movecount;

    rearch *= ratio;
    //std::cout<<rearch<<std::endl;
    rearch = 3;

    std::vector<RoutesAndNode> routes1,routes2;
    for(auto ch : age1->childs){
        std::cout<<"A"<<std::endl;;
        RoutesAndNode ins;
        std::cout<<"HOGE"<<std::endl;
        ins.CollectIndex(ch.second.first);
        std::cout<<"KS"<<std::endl;
        ins.CollectPos(side, 0, field);
        std::cout<<"HOE"<<std::endl;
        routes1.push_back(ins);
    }
   // std::cout<<routes1.size()<<std::endl;

    for(auto ch :  age2->childs){
        RoutesAndNode ins;
        ins.CollectIndex(ch.second.first);
        ins.CollectPos(side, 1, field);
        routes2.push_back(ins);
    }

    for(int index = 0;index < rearch;index++){
        RoutesAndNode ins;
        ins.CollectIndex(age1);
        ins.CollectPos(side, 0, field);
        routes1.push_back(ins);
    }

    std::cout<<routes1.size()<<" "<<routes2.size()<<std::endl;

    for(int index = 0;index < rearch;index++){
        RoutesAndNode ins;
        ins.CollectIndex(age2);
        ins.CollectPos(side, 1, field);
        routes2.push_back(ins);
    }
    std::cout<<routes1.size()<<" "<<routes2.size()<<std::endl;


    auto check = [&](int index1, int index2){
        RoutesAndNode ro1 = routes1.at(index1);
        RoutesAndNode ro2 = routes2.at(index2);

        int count = 0;
        for(int index_a = 0;index_a < ro1.route_pos.size();index_a++){
            for(int index_b = 0;index_b < ro2.route_pos.size();index_b++){
                if(ro1.route_pos.at(index_a) == ro2.route_pos.at(index_b))
                    count++;
            }
        }
        return sikiiti >= count;
    };
    std::pair<std::pair<int,int>,int> ans = std::make_pair(std::make_pair(8,8),-1e9);


    do{
    std::cout<<routes2.front().indexs.size()<<std::endl;
        for(int a = 0;a < routes1.size();a++){
            for(int b = 0;b < routes2.size();b++){
                if(check(a,b) && routes1.at(a).next_pos != routes2.at(b).next_pos){
                    if(ans.second <= routes1.at(a).adv + routes2.at(b).adv){
                        ans.second = routes1.at(a).adv + routes2.at(b).adv;
                  //      std::cout<<routes1.at(a).indexs.size()<<" "<<routes2.at(b).indexs.size()<<std::endl;
                        ans.first = std::make_pair(routes1.at(a).indexs.front(), routes2.at(b).indexs.front());
                    }
                }
                  std::cout<<"ELE"<<std::endl;
            }

        }
        sikiiti--;
    }while(ans.second == -1e9 && sikiiti != 0);


  //  std::cout<<ans.first.first<<" "<<ans.first.second<<" "<<ans.second<<std::endl;

    return ans;
}

void DepthFirstSearch::RoutesAndNode::CollectIndex(std::shared_ptr<SearchNode> now){

    std::shared_ptr<SearchNode> ins;
    int way = 8;
    int mi = -1e9;
    for(auto ch : now->childs){
        if(mi <= ch.second.first->getAdvSum() && ch.second.first->flag){
            ins = ch.second.first;
            mi = ch.second.first->getAdvSum();
            way = ch.first;
        }
    }
    if(mi != -1e9){
       indexs.push_back(way);
       adv = mi;
       CollectIndex(ins);
       ins->adv = -1000000007;
    }else{
        if(indexs.empty())indexs.push_back(8);
        now->flag = false;
    }
}

void DepthFirstSearch::SearchNode::dfsAdd(std::pair<int,int> pos, std::vector<std::vector<int>>& vec){

    for(auto ch : childs){
        std::pair<int,int> new_pos(pos);
        new_pos.first += dx.at(ch.first);
        new_pos.second += dy.at(ch.first);

        vec.at(new_pos.first).at(new_pos.second) += ch.second.first->size;
        ch.second.first->dfsAdd((ch.second.second ? new_pos : pos), vec);
    }
}

int DepthFirstSearch::SearchNode::getAdvSum(){
    if(advsum != -1000000007)
        return advsum;

    int point = 0;
    for(auto ch : childs)
        if(ch.second.first->flag) point = std::max(point, ch.second.first->getAdvSum());

    return advsum = point + adv;
}

std::pair<int, int> DepthFirstSearch::SearchNode::getMaxAdvMove(){
    std::pair<int,int> maxmove(-1000000007, -1);
    for(auto ch : childs)
        if(maxmove.first < ch.second.first->getAdvSum())
            maxmove = std::make_pair(ch.second.first->getAdvSum(), ch.first);

    return maxmove;
}


const std::vector<int> DepthFirstSearch::SearchNode::dx({1, 1, 0, -1, -1, -1, 0, 1, 0});
const std::vector<int> DepthFirstSearch::SearchNode::dy({0, -1, -1, -1, 0, 1, 1, 1, 0});

void DepthFirstSearch::RoutesAndNode::CollectPos(int side, int agent, procon::Field field){
   // for(int a = 0;a < indexs.size();a++)std::cout<<" "<<indexs.at(a);
   // std::cout<<std::endl;
    std::pair<int,int> pos = field.getAgent(side, agent);
    route_pos.push_back(pos);
    std::vector<std::vector<int>> flag(12, std::vector<int>(12, 0));
    for(int x = 0;x < field.getSize().first; x++){
        for(int y = 0;y < field.getSize().second; y++){
            flag.at(x).at(y) = field.getState(x, y).first;
        }
    }

    auto moveAfter = [&](int move){
        std::cout<<SearchNode::dx.at(move) + pos.first<<" "<<SearchNode::dy.at(move) + pos.second<<std::endl;
        if(flag.at(SearchNode::dx.at(move) + pos.first).at(SearchNode::dy.at(move) + pos.second) == side + 1 && flag.at(SearchNode::dx.at(move) + pos.first).at(SearchNode::dy.at(move) + pos.second) == 0){
            flag.at(SearchNode::dx.at(move) + pos.first).at(SearchNode::dy.at(move)) = 0;
            return pos;
        }else{
            return std::make_pair(SearchNode::dx.at(move) + pos.first, SearchNode::dy.at(move) + pos.second);
        }
    };

    for(int a = 0;a < indexs.size();a++){
        pos = moveAfter(indexs.at(a));
        route_pos.push_back(pos);
        if(a == 0){
            next_pos = pos;
        }
    }
}
