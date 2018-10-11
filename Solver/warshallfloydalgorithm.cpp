#include "warshallfloydalgorithm.h"

WarshallFloydAlgorithm::WarshallFloydAlgorithm(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side)
{
    std::tie(size_x, size_y) = field.getSize();
    size_sum = size_x * size_y;

    dock = std::make_shared<MinimumVisualizerDock>(4);
    dock->show();
}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> WarshallFloydAlgorithm::agentAct(int){

    std::vector<std::vector<Edge>> edges = calcDijkStra(getPosValue(field.getAgent(side, 0)), 30);
    for(int index = 0; index < 30; ++index)
        getRoute(edges, 0, index);

    return std::make_pair(std::make_tuple(0,0,0), std::make_tuple(0,0,0));
}

std::list<std::pair<int,int>> WarshallFloydAlgorithm::getRoute(std::vector<std::vector<Edge>>& input, int target_pos, int depth){

    auto get_list = [&](int pos, int depth){

        if(input.at(pos).at(depth).depth != depth){
            std::cerr << "error case" << std::endl;
        }

        std::list<std::pair<int, int>> pos_list;

        while(depth > 0){
            pos_list.emplace_back(getPosPair(pos));

            int bef_pos = pos;
            pos = input.at(pos).at(depth).prev.first;
            depth = input.at(bef_pos).at(depth).prev.second;
        }

        pos_list.emplace_back(getPosPair(pos));

        pos_list.reverse();

        return pos_list;
    };

    std::list<std::pair<int,int>> routes;
    routes = get_list(target_pos, depth);

    dock->addVisualizer(field.getSize(), std::vector<std::list<std::pair<int,int>>>({routes}), std::vector<std::vector<std::vector<int>>>(3, std::vector<std::vector<int>>(size_x, std::vector<int>(size_y, 255))));

    return routes;
}

std::vector<std::vector<WarshallFloydAlgorithm::Edge>> WarshallFloydAlgorithm::calcDijkStra(int start_pos, int maxval){

    std::vector<std::vector<Edge>> dp_vector(size_sum);

    for(int start_index = 0; start_index < size_sum; ++start_index)
        for(int end_index = 0; end_index < maxval; ++end_index)
            dp_vector.at(start_index).emplace_back(start_index);

    dp_vector.at(start_pos).at(0).depth = 0;

    auto check = [&](int start_pos, int depth, int end_pos){
        while(depth > 0){
            if(start_pos == end_pos)
                return true;

            int bef_pos = start_pos;
            start_pos = dp_vector.at(start_pos).at(depth).prev.first;
            depth = dp_vector.at(bef_pos).at(depth).prev.second;
        }

        return start_pos == end_pos;
    };

    for(int depth = 0; depth < maxval; ++depth)
        for(int point = 0; point < size_sum; ++point)
            if(dp_vector.at(point).at(depth).depth == depth)
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

                        Edge e = Edge::make(dp_vector.at(point).at(depth), end_index, value, length);

                        dp_vector.at(end_index).at(depth + length) = std::max(dp_vector.at(end_index).at(depth + length), e);
                    }

    return dp_vector;
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
