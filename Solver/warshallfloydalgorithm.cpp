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
        for(int end_index = 0; end_index < maxval; ++end_index)
            wf_vector.at(start_index).emplace_back(start_index);

    wf_vector.at(getPosValue(field.getAgent(side, 0))).at(0).length = 0;

    auto check = [&](int start_pos, int depth, int end_pos){
        while(depth > 0){
            if(start_pos == end_pos)
                return true;

            int bef_pos = start_pos;
            start_pos = wf_vector.at(start_pos).at(depth).prev.first;
            depth = wf_vector.at(bef_pos).at(depth).prev.second;
        }

        return start_pos == end_pos;
    };

    for(int depth = 0; depth < maxval; ++depth)
        for(int point = 0; point < size_sum; ++point)
            if(wf_vector.at(point).at(depth).length == depth)
                for(int direction = 0; direction < 8; ++direction)
                    if(!outOfRange(point, direction)){

                        int end_index = getRotatePos(point, direction);

                        if(check(point, depth, end_index))
                            continue;

                        std::pair<int, int> end_pos = getPosPair(end_index);
                        std::pair<int, int> end_pos_state = field.getState(end_pos.first, end_pos.second);

                        int length = 1 + (end_pos_state.first == side ? 1 : 2);
                        double value = (end_pos_state.first == side + 1 ? 0 : end_pos_state.second * length);

                        if(depth + length >= maxval)
                            continue;

                        Edge e = Edge::make(wf_vector.at(point).at(depth), end_index, value, length);

                        wf_vector.at(end_index).at(depth + length) = std::max(wf_vector.at(end_index).at(depth + length), e);
                    }

    auto get_list = [&](int pos, int depth){
        std::list<std::pair<int, int>> pos_list;

        while(depth > 0){
            pos_list.emplace_back(getPosPair(pos));

            int bef_pos = pos;
            pos = wf_vector.at(pos).at(depth).prev.first;
            depth = wf_vector.at(bef_pos).at(depth).prev.second;
        }

        pos_list.emplace_back(getPosPair(pos));

        pos_list.reverse();

        return pos_list;
    };

    std::vector<std::list<std::pair<int,int>>> routes;
    routes.push_back(get_list(0, 25));

    dock->addVisualizer(field.getSize(), routes, std::vector<std::vector<std::vector<int>>>(3, std::vector<std::vector<int>>(size_x, std::vector<int>(size_y, 255))));

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
