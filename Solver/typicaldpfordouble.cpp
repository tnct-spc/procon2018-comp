#include "typicaldpfordouble.h"

TypicalDpForDouble::TypicalDpForDouble(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side)
{
    std::tie(size_x, size_y) = field.getSize();
    size_sum = size_x * size_y;

    dock = std::make_shared<ProgresDock>();
    if(dock_show)
        dock->show();
}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> TypicalDpForDouble::agentAct(int){

    int maxval = std::min(max_maxval, field.getFinalTurn() - field.getTurnCount() + 1);

    auto int_to_pair = [this](int x){return std::make_pair(x / size_y, x % size_y);};
    auto pair_to_int = [this](std::pair<int,int> x){return x.first * size_y + x.second;};

    auto swap_pair = [](std::pair<std::pair<int,int>,std::pair<int,int>> x){
        if(x.first > x.second)std::swap(x.first, x.second);
        return x;
    };

    auto two_pair_to_int = [this, pair_to_int, swap_pair](std::pair<std::pair<int,int>,std::pair<int,int>> x){
        x = swap_pair(x);
        return pair_to_int(x.first) * size_sum + pair_to_int(x.second);
    };
    auto int_to_two_pair = [this, int_to_pair, swap_pair](int x){
        return swap_pair(std::make_pair(int_to_pair(x / size_sum), int_to_pair(x % size_sum)));
    };

    std::bitset<288> field_bitset;
    for(int index = 0; index < size_sum; ++index){

        std::pair<int,int> position = int_to_pair(index);
        field_bitset[2 * index + 1] = (field.getState(position.first, position.second).first == (side ? 1 : 2));
        field_bitset[2 * index] = !field_bitset[2 * index + 1] && !field.getState(position.first, position.second).first;
    }

    std::vector<std::vector<Edge>> dp(maxval + 1);
    for(int dep = 0; dep < maxval + 1; ++dep)
        for(int count = 0; count < size_sum * size_sum; ++count)
            dp.at(dep).emplace_back(count, field_bitset);

    dp.at(0).at(two_pair_to_int(std::make_pair(field.getAgent(side, 0), field.getAgent(side, 1)))).depth = 0;

    auto check_outofrange = [this, int_to_two_pair](int pos, int dir){

        std::pair<int,int> pos_0, pos_1;
        std::tie(pos_0, pos_1) = int_to_two_pair(pos);

        pos_0.first += dx.at(dir / 8);
        pos_0.second += dy.at(dir / 8);
        pos_1.first += dx.at(dir % 8);
        pos_1.second += dy.at(dir % 8);

        return pos_0.first < 0 || pos_0.second < 0 || pos_0.first >= size_x || pos_0.second >= size_y ||
            pos_1.first < 0 || pos_1.second < 0 || pos_1.first >= size_x || pos_1.second >= size_y;
    };

    std::vector<int> first_move_pos(size_sum * size_sum, 0);

    procon::Field _field = field;

    for(int dep = 0; dep < maxval; ++dep)
        for(int pos_0 = 0; pos_0 < size_sum; ++pos_0)
            for(int pos_1 = pos_0 + 1; pos_1 < size_sum; ++pos_1){

                int pos = pos_0 * size_sum + pos_1;

                if(dp.at(dep).at(pos).depth == -1)
                    continue;

                for(int direction = 0; direction < 64; ++direction){
                    if(check_outofrange(pos, direction))
                        continue;

                    auto before_pos_pairs = int_to_two_pair(pos);
                    auto after_pos_pairs = before_pos_pairs;

                    after_pos_pairs.first.first += dx.at(direction / 8);
                    after_pos_pairs.first.second += dy.at(direction / 8);
                    after_pos_pairs.second.first += dx.at(direction % 8);
                    after_pos_pairs.second.second += dy.at(direction % 8);

                    std::pair<int,int> state, value;
                    std::tie(state.first, value.first) = field.getState(after_pos_pairs.first.first, after_pos_pairs.first.second);
                    std::tie(state.second, value.second) = field.getState(after_pos_pairs.second.first, after_pos_pairs.second.second);

                    int is_update_bitset = 2 * (state.first != side + 1) + (state.second != side + 1);

                    int is_move = 2 * (is_update_bitset & 2 ? !state.first : 0) + (is_update_bitset & 1 ? !state.second : 0);


                    double value_sum = (is_update_bitset & 2) * value.first + (is_update_bitset & 1) * value.second;

                    std::vector<std::pair<int,int>> points = {std::make_pair(0,0), std::make_pair(0,0)};

                    if(dep + 1 == maxval){
                        _field.setFieldData(is_update_bitset);
                        std::vector<std::pair<int,int>> points = _field.getPoints();
                    }


                    int next_pos = two_pair_to_int(std::make_pair(is_move & 2 ? after_pos_pairs.first : before_pos_pairs.first, is_move & 1 ? after_pos_pairs.second : before_pos_pairs.second));

                    value_sum *= point_depth_weight.at(dep);

                    Edge e = Edge::make(dp.at(dep).at(pos), next_pos, value_sum + points.at(side).second, is_update_bitset, std::make_pair(pair_to_int(after_pos_pairs.first), pair_to_int(after_pos_pairs.second)));

                    if(!dep && dp.at(dep + 1).at(next_pos) < e)
                        first_move_pos.at(next_pos) = two_pair_to_int(after_pos_pairs);

                    dp.at(dep + 1).at(next_pos) = std::max(dp.at(dep + 1).at(next_pos), e);
                }
        }

    auto match_at_first = [&](std::pair<std::pair<int,int>, std::pair<int,int>> before, std::pair<std::pair<int,int>, std::pair<int,int>> after){

        return std::abs(after.first.first - before.first.first) <= 1 && std::abs(after.first.second - before.first.second) <= 1 &&
            std::abs(after.second.first - before.second.first) <= 1 && std::abs(after.second.second - before.second.second) <= 1;
    };


    auto getRoute = [&](int pos, int depth){
        // std::list<std::pair<int,int>> lis;
        std::pair<std::list<std::pair<int,int>>, std::list<std::pair<int,int>>> lis;

        if(dp.at(depth).at(pos).depth == -1)
            return lis;

        while(dp.at(depth).at(pos).depth > 0){
            auto pos_pair = int_to_two_pair(pos);

            if(!match_at_first(std::make_pair(lis.first.back(), lis.second.back()), pos_pair))
                std::swap(pos_pair.first, pos_pair.second);

            lis.first.push_back(pos_pair.first);
            lis.second.push_back(pos_pair.second);

            std::tie(pos, depth) = dp.at(depth).at(pos).prev;
        }

        auto pos_pair = int_to_two_pair(pos);

        if(!match_at_first(std::make_pair(lis.first.back(), lis.second.back()), pos_pair))
            std::swap(pos_pair.first, pos_pair.second);

        lis.first.push_back(pos_pair.first);
        lis.second.push_back(pos_pair.second);

        lis.first.reverse();
        lis.second.reverse();

        return lis;
    };

    std::priority_queue<std::pair<double, std::pair<std::list<std::pair<int,int>>, std::list<std::pair<int,int>>>>> routes_que;
    for(int dep = 1; dep <= maxval; ++dep)
        for(int pos = 0; pos < size_sum * size_sum; ++pos){
            auto route = getRoute(pos, dep);
            if(!route.first.empty() || !route.second.empty()){
                if(dock_show)
                    dock->addMinumuVisu(field.getSize(), std::vector<std::list<std::pair<int,int>>>({route.first, route.second}), std::vector<std::vector<std::vector<int>>>(3, std::vector<std::vector<int>>(size_x, std::vector<int>(size_y, 255))));
                routes_que.emplace(dp.at(dep).at(pos).average() * route_length_weight.at(dep - 1), route);
            }
        }

    std::map<int, MapElement> move_map;

    int bound = bound_per * routes_que.size();

    for(int count = 0; count < bound; ++count){
        auto top_element = routes_que.top();
        routes_que.pop();

        int ori_pos = first_move_pos.at(two_pair_to_int(std::make_pair(*std::next(top_element.second.first.begin()), *std::next(top_element.second.second.begin()))));

        move_map[ori_pos].addRoute(top_element.first, top_element.second);
    }

    double max_adv = -1e9;

    std::pair<int,int> pos_first(field.getAgent(side, 0));
    std::pair<int,int> pos_second(field.getAgent(side, 1));

    auto agent_conflict = [this](double& score, std::pair<int,int> next_move_pos){

        if(next_move_pos == field.getAgent(side ^ 1, 0) && next_move_pos == field.getAgent(side ^ 1, 1))
            score *= conflict_def_per;

        if(field.getState(next_move_pos.first, next_move_pos.second).first == (side ? 1 : 2) &&
        std::abs(next_move_pos.first - field.getAgent(side ^ 1, 0).first) <= 1 &&
        std::abs(next_move_pos.second - field.getAgent(side ^ 1, 0).second) <= 1 &&
        std::abs(next_move_pos.first - field.getAgent(side ^ 1, 1).first) <= 1 &&
        std::abs(next_move_pos.second - field.getAgent(side ^ 1, 1).second) <= 1)
            score *= conflict_atk_per;
    };

    for(auto element : move_map){

        double adv = element.second.routes.size();

        auto after_pair = int_to_two_pair(element.first);

        agent_conflict(adv, after_pair.first);
        agent_conflict(adv, after_pair.second);

        if(!(max_adv < adv))
            continue;

        max_adv = adv;

        if(match_at_first(std::make_pair(field.getAgent(side, 0), field.getAgent(side, 1)), after_pair))
            std::tie(pos_first, pos_second) = after_pair;
        else
            std::tie(pos_second, pos_first) = after_pair;
    }

    /*
    std::cout << field.getAgent(side, 0).first << "," << field.getAgent(side, 0).second << " " << field.getAgent(side, 1).first << "," << field.getAgent(side, 1).second << std::endl;
    std::cout << pos_first.first << "," << pos_first.second << " " << pos_second.first << "," << pos_second.second << std::endl;
    */

    return std::make_pair(std::make_tuple((field.getState(pos_first.first, pos_first.second).first != (side ? 1 : 2) ? 1 : 2), pos_first.first - field.getAgent(side, 0).first, pos_first.second - field.getAgent(side, 0).second),
                          std::make_tuple((field.getState(pos_second.first, pos_second.second).first != (side ? 1 : 2) ? 1 : 2), pos_second.first - field.getAgent(side, 1).first, pos_second.second - field.getAgent(side, 1).second));

}

const std::vector<int> TypicalDpForDouble::dx({1, 1, 0, -1, -1, -1, 0, 1});
const std::vector<int> TypicalDpForDouble::dy({0, -1, -1, -1, 0, 1, 1, 1});
