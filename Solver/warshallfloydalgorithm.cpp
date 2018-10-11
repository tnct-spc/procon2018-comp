#include "warshallfloydalgorithm.h"

WarshallFloydAlgorithm::WarshallFloydAlgorithm(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side)
{
    dock = std::make_shared<MinimumVisualizerDock>(4);
    dock->show();
}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> WarshallFloydAlgorithm::agentAct(int){

    std::tie(size_x, size_y) = field.getSize();
    size_sum = size_x * size_y;


    std::vector<std::vector<Edge>> wf_vector(size_sum);

    for(int start_index = 0; start_index < size_sum; ++start_index)
        for(int end_index = 0; end_index < size_sum; ++end_index)
            wf_vector.at(start_index).emplace_back(start_index, end_index);

    for(int start_index = 0; start_index < size_sum; ++start_index){
        for(int rotate_index = 0; rotate_index < 8; ++rotate_index)
            if(!outOfRange(start_index, rotate_index)){

                int end_index = getRotatePos(start_index, rotate_index);
                std::pair<int, int> end_pos = getPosPair(end_index);
                std::pair<int, int> end_pos_state = field.getState(end_pos.first, end_pos.second);

                int length = 1 + (end_pos_state.first == side ? 1 : 2);

                wf_vector.at(start_index).at(end_index).length = length;

                wf_vector.at(start_index).at(end_index).sum = (end_pos_state.first == side + 1 ? 0 : end_pos_state.second * length);
            }
        wf_vector.at(start_index).at(start_index).length = 0;
    }

    auto conflict_check = [&](int st, int en, int center){

        if(st == en)
            return true;

        // [st,center)
        std::set<int> front_set;
        while(st != center){
            front_set.insert(st);
            st = wf_vector.at(st).at(center).next;
            front_set.insert(st);
        }

        // [center,en)
        while(center != en){
            center = wf_vector.at(center).at(en).next;
            if(front_set.count(center))
                return true;
            std::cout << st << " " << center << " " << en << std::endl;
        }
        return false;
    };

    for(int center = 0; center < size_sum; ++center)
        for(int begin = 0; begin < size_sum; ++begin)
            for(int end = 0; end < size_sum; ++end)
                // if(wf_vector.at(begin).at(center).length != -1 && wf_vector.at(center).at(end).length != -1)
                if(wf_vector.at(begin).at(center).length != -1 && wf_vector.at(center).at(end).length != -1 && !conflict_check(begin, end, center))
                    wf_vector.at(begin).at(end) = std::max(wf_vector.at(begin).at(end), wf_vector.at(begin).at(center) + wf_vector.at(center).at(end));

    auto get_list = [&](int st, int en){
        std::list<std::pair<int, int>> pos_list;

        // while(st != en){
            for(int i = 0; i < 20; ++i){
                std::cout << st << std::endl;
            pos_list.emplace_back(getPosPair(st));
            st = wf_vector.at(st).at(en).next;
        }
        // pos_list.emplace_back(getPosPair(en));

        return pos_list;
    };

    std::vector<std::list<std::pair<int,int>>> routes;
    routes.push_back(get_list(getPosValue(field.getAgent(side, 0)), 0));

    std::vector<std::list<std::pair<int,int>>> routes2;
    routes2.push_back(get_list(getPosValue(field.getAgent(side, 1)), 0));

    dock->addVisualizer(field.getSize(), routes, std::vector<std::vector<std::vector<int>>>(3, std::vector<std::vector<int>>(size_x, std::vector<int>(size_y, 255))));
    dock->addVisualizer(field.getSize(), routes2, std::vector<std::vector<std::vector<int>>>(3, std::vector<std::vector<int>>(size_x, std::vector<int>(size_y, 255))));

    return std::make_pair(std::make_tuple(0,0,0), std::make_tuple(0,0,0));
}

std::pair<int, int> WarshallFloydAlgorithm::getPosPair(int x){
    return std::make_pair(x / size_y, x % size_y);
}

int WarshallFloydAlgorithm::getPosValue(std::pair<int, int> pos){
    return size_y * pos.first + pos.second;
}

int WarshallFloydAlgorithm::getRotatePos(int pos, int rotate){
    return pos + size_y * dx.at(rotate) + dy.at(rotate);
}

bool WarshallFloydAlgorithm::outOfRange(int pos, int rotate){

    std::pair<int, int> field_size = field.getSize();
    std::pair<int,int> pos_pair = getPosPair(pos);
    pos_pair.first += dx.at(rotate);
    pos_pair.second += dy.at(rotate);

    return pos_pair.first < 0 || pos_pair.second < 0 || pos_pair.first >= field_size.first || pos_pair.second >= field_size.second;
}

const std::vector<int> WarshallFloydAlgorithm::dx({1, 1, 0, -1, -1, -1, 0, 1, 0});
const std::vector<int> WarshallFloydAlgorithm::dy({0, -1, -1, -1, 0, 1, 1, 1, 0});
