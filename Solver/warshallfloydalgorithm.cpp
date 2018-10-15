#include "warshallfloydalgorithm.h"

WarshallFloydAlgorithm::WarshallFloydAlgorithm(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side)
{
    std::tie(size_x, size_y) = field.getSize();
    size_sum = size_x * size_y;

    dock = std::make_shared<ProgresDock>();
    // dock->show();
}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> WarshallFloydAlgorithm::agentAct(int){

    std::pair<int,int> bef_0 = field.getAgent(side, 0);
    std::pair<int,int> bef_1 = field.getAgent(side, 1);
    std::vector<std::pair<int ,std::pair<int,int>>> poses_0 = calcSingleAgent(0);
    std::vector<std::pair<int ,std::pair<int,int>>> poses_1 = calcSingleAgent(1);

    std::pair<std::pair<int,int> , std::pair<int,int>> ans;
    int max_adv = -1e9;

    auto apply_conflict = [&](auto& input_vec){

        for(auto& pos_pair : input_vec){
            std::pair<int,int> next_move_pos = pos_pair.second;

            if(next_move_pos == field.getAgent(side ^ 1, 0) && next_move_pos == field.getAgent(side ^ 1, 1))
                pos_pair.first *= conflict_def_per;

            if(field.getState(next_move_pos.first, next_move_pos.second).first == (side ? 1 : 2) &&
                std::abs(next_move_pos.first - field.getAgent(side ^ 1, 0).first) <= 1 &&
                std::abs(next_move_pos.second - field.getAgent(side ^ 1, 0).second) <= 1 &&
                std::abs(next_move_pos.first - field.getAgent(side ^ 1, 1).first) <= 1 &&
                std::abs(next_move_pos.second - field.getAgent(side ^ 1, 1).second) <= 1)

            pos_pair.first *= conflict_atk_per;
        }
    };

    apply_conflict(poses_0);
    apply_conflict(poses_1);

    for(auto& pos0 : poses_0)
        for(auto& pos1 : poses_1)
            if(pos0.second != pos1.second && pos0.first + pos1.first >= max_adv){
                max_adv = pos0.first + pos1.first;
                ans = std::make_pair(pos0.second, pos1.second);
            }

    std::pair<int,int> pos_0 = ans.first;
    std::pair<int,int> pos_1 = ans.second;


    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> ret_value = std::make_pair(
                std::make_tuple(1 + (field.getState(pos_0.first, pos_0.second).first == (side ? 1 : 2)), pos_0.first - bef_0.first, pos_0.second - bef_0.second),
                std::make_tuple(1 + (field.getState(pos_1.first, pos_1.second).first == (side ? 1 : 2)), pos_1.first - bef_1.first, pos_1.second - bef_1.second)
                          );

    return ret_value;
}

std::vector<std::pair<int, std::pair<int,int>>> WarshallFloydAlgorithm::calcSingleAgent(int agent){

    // [0,maxdepth]
    int maxdepth = std::min(maxdepth_max, field.getFinalTurn() - field.getTurnCount());

    std::pair<int,int> agent_pos = field.getAgent(side, agent);

    std::vector<std::vector<Edge>> edges = calcDijkStra(getPosValue(agent_pos), maxdepth);
    std::vector<std::vector<std::list<std::pair<int,int>>>> routes;

    std::map<std::pair<int,int>, MapElement> route_map;

    std::priority_queue<std::pair<double, std::list<std::pair<int,int>>>> que;

    for(int pos = 0; pos < size_sum; ++pos)
        for(int depth = 1; depth <= maxdepth; ++depth){
            double score;
            std::list<std::pair<int,int>> route;

            std::tie(score, route) = getRoute(edges, pos, depth);
            if(route.empty())
                continue;
            que.emplace(std::min(depth_weight_max, std::pow(depth_weight, depth)) * score / depth, std::move(route));

        }

    int bound = que.size() * bound_val;

    for(int index = 0; !que.empty(); ++index){
        double average;
        std::list<std::pair<int,int>> route;

        std::tie(average, route) = que.top();
        que.pop();

        std::pair<int,int> back = *std::next(route.begin());

        MapElement element(back);

        if(route_map.find(back) == route_map.end())
            route_map[back] = MapElement(back);
        route_map[back].addRoute(average, std::move(route));

        if(index >= bound && route_map.size() > 2)
            break;
    }

    std::vector<std::pair<int, std::pair<int,int>>> anses;

    for(auto& map_element : route_map){

        map_element.second.setPutCount(field.getSize());

        const std::pair<int,int>& target_pos = map_element.second.pos;
        const std::vector<std::list<std::pair<int,int>>> routes = map_element.second.routes.second;
        const std::vector<std::vector<int>>& putcounts = map_element.second.put_count;

        std::vector<std::vector<std::vector<int>>> color(3, std::vector<std::vector<int>>(size_x, std::vector<int>(size_y, 255)));

        color.at(0).at(target_pos.first).at(target_pos.second) = 128;
        color.at(1).at(agent_pos.first).at(agent_pos.second) = 128;
        color.at(2).at(agent_pos.first).at(agent_pos.second) = 128;

        int put_count_sum = 0.0;
        std::for_each(putcounts.begin(), putcounts.end(), [&put_count_sum](const std::vector<int>& v){put_count_sum += std::accumulate(v.begin(), v.end(), 0);});

        for(int point = 0; point < size_sum; ++point){
            std::pair<int,int> point_pair = getPosPair(point);

            if(point_pair == target_pos)
                continue;
            color.at(1).at(point_pair.first).at(point_pair.second) -= putcounts.at(point_pair.first).at(point_pair.second) * 512 / put_count_sum;
            color.at(2).at(point_pair.first).at(point_pair.second) -= putcounts.at(point_pair.first).at(point_pair.second) * 512 / put_count_sum;
        }

        anses.emplace_back(routes.size(), target_pos);

        // dock->addMinumuVisu(field.getSize(), routes, color);
    }

    std::sort(anses.begin(), anses.end(), std::greater<std::pair<int, std::pair<int,int>>>());

    return anses;

}

std::pair<double, std::list<std::pair<int,int>>> WarshallFloydAlgorithm::getRoute(std::vector<std::vector<Edge>>& input, int target_pos, int depth){

    auto get_list = [&](int pos, int dep){

        std::list<std::pair<int, int>> pos_list;

        if(input.at(pos).at(dep).depth != dep)
            return pos_list;

        while(dep > 0){
            pos_list.emplace_back(getPosPair(pos));

            int bef_pos = pos;
            pos = input.at(pos).at(dep).prev.first;
            dep = input.at(bef_pos).at(dep).prev.second;
        }

        pos_list.emplace_back(getPosPair(pos));

        pos_list.reverse();

        return pos_list;
    };

    std::list<std::pair<int,int>> routes;
    routes = get_list(target_pos, depth);

    return std::make_pair(input.at(target_pos).at(depth).sum, std::move(routes));
}

// [0,maxval]
std::vector<std::vector<WarshallFloydAlgorithm::Edge>> WarshallFloydAlgorithm::calcDijkStra(int start_pos, int maxval){

    std::bitset<288> field_states;
    for(int x_pos = 0; x_pos < size_x; ++x_pos)
        for(int y_pos = 0; y_pos < size_y; ++y_pos){

            int index = getPosValue(std::make_pair(x_pos, y_pos));

            int before = field.getState(x_pos, y_pos).first;
            int after = (side + 1 == before ? 0 : 1 + static_cast<bool>(before));

            field_states |= (std::bitset<288>(after) << (index * 2));
        }

    std::vector<std::vector<Edge>> dp_vector(size_sum);

    for(int start_index = 0; start_index < size_sum; ++start_index)
        for(int end_index = 0; end_index <= maxval; ++end_index)
            dp_vector.at(start_index).emplace_back(start_index, field_states);

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

                        /*
                        if(check(point, depth, end_index))
                            continue;
                        */

                        std::pair<int, int> end_pos = getPosPair(end_index);
                        std::pair<int, int> end_pos_state = field.getState(end_pos.first, end_pos.second);

                        int length = ((end_pos_state.first != (side ? 1 : 2)) ? 1 : 2);
                        double value = (end_pos_state.first == side + 1 ? 0 : end_pos_state.second * length);

                        if(length == 2 && depth + 1 == maxval){
                            length = 1;
                            value /= 2;
                        }
                        else if(depth + length > maxval)
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
