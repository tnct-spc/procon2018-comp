#include "typicaldpfordouble.h"

TypicalDpForDouble::TypicalDpForDouble(const procon::Field& field, int final_turn, bool side) :
    AlgorithmWrapper(field, final_turn, side)
{
    std::tie(size_x, size_y) = field.getSize();
    size_sum = size_x * size_y;

    dock = std::make_shared<ProgresDock>();
    // dock->show();
}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> TypicalDpForDouble::agentAct(int){

    auto int_to_pair = [this](int x){return std::make_pair(x / size_y, x % size_y);};
    auto pair_to_int = [this](std::pair<int,int> x){return x.first * size_y + x.second;};

    /*
    auto tuple_to_int = [this](std::tuple<int,int,int> x){return std::get<0>(x) * size_sum + std::get<1>(x) * size_y + std::get<0>(x);};
    auto int_to_tuple = [this](int x){return std::make_tuple(x / size_sum, (x % size_sum) / size_y, x % size_y);};
    */

    auto swap_pair = [](std::pair<std::pair<int,int>,std::pair<int,int>> x){
        if(x.first > x.second)swap(x.first, x.second);
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
        field_bitset[2 * index] = (field.getState(position.first, position.second).first == (side ? 1 : 2));
        field_bitset[2 * index + 1] = (field.getState(position.first, position.second).first != side + 1);
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

    for(int dep = 0; dep < maxval; ++dep)
        for(int pos = 0; pos < size_sum * size_sum; ++pos){
            if(dp.at(dep).at(pos).depth == -1)
                continue;
            for(int direction = 0; direction < 64; ++direction)
                if(!check_outofrange(pos, direction)){

                    auto before_pos_pairs = int_to_two_pair(pos);
                    auto after_pos_pairs = before_pos_pairs;

                    after_pos_pairs.first.first += dx.at(direction / 8);
                    after_pos_pairs.first.second += dy.at(direction / 8);
                    after_pos_pairs.second.first += dx.at(direction % 8);
                    after_pos_pairs.second.second += dy.at(direction % 8);

                    int after_pos = two_pair_to_int(after_pos_pairs);

                    std::pair<int,int> state, value;
                    std::tie(state.first, value.first) = field.getState(after_pos_pairs.first.first, after_pos_pairs.first.second);
                    std::tie(state.second, value.second) = field.getState(after_pos_pairs.second.first, after_pos_pairs.second.second);

                    int is_update_bitset = 2 * (state.first != side + 1) + state.second != side + 1;

                    int is_move(2 * (is_update_bitset & 2 ? !state.first : 0) + is_update_bitset & 1 ? !state.second : 0);

                    double value_sum = value.first + value.second;

                    Edge e = Edge::make(dp.at(dep).at(pos), two_pair_to_int(std::make_pair(is_move & 2 ? after_pos_pairs.first : before_pos_pairs.first, is_move & 1 ? after_pos_pairs.second : before_pos_pairs.second)), value_sum, is_update_bitset, std::make_pair(pair_to_int(after_pos_pairs.first), pair_to_int(after_pos_pairs.second)));

                    dp.at(dep + 1).at(after_pos) = std::max(dp.at(dep + 1).at(after_pos), e);
                }
        }

    return std::make_pair(std::make_tuple(0,0,0), std::make_tuple(0,0,0));

}

const std::vector<int> TypicalDpForDouble::dx({1, 1, 0, -1, -1, -1, 0, 1});
const std::vector<int> TypicalDpForDouble::dy({0, -1, -1, -1, 0, 1, 1, 1});
