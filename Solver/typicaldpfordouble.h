#ifndef TYPICALDPFORDOUBLE_H
#define TYPICALDPFORDOUBLE_H

#include "algorithmwrapper.h"
#include "progresdock.h"

class TypicalDpForDouble : public AlgorithmWrapper
{
public:
    TypicalDpForDouble(const procon::Field& field, int final_turn, bool side);

    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int);
    void setParams(std::vector<std::pair<QString, double>>){}

private:

    int size_sum;
    const int max_maxval = 30;
    double bound_per = 0.1;

    double conflict_def_per = 1.3;
    double conflict_atk_per = 0.3;

    std::shared_ptr<ProgresDock> dock;

    const bool dock_show = false;

    static const std::vector<int> dx, dy;
    struct Edge;
    struct MapElement;
};

struct TypicalDpForDouble::Edge{

    int pos;
    std::pair<int,int> prev;
    double sum;
    int depth;
    std::bitset<288> bs;

    Edge(int pos, const std::bitset<288>& state, double sum = 0.0, int depth = -1) :
        pos(pos),
        prev(pos, depth),
        sum(sum),
        depth(depth),
        bs(state)
    {
    }


    friend bool operator<(const Edge& a, const Edge& b){
        return a.average() < b.average();
    }

    static Edge make(const Edge& x, int pos, double value, int is_update_bitset, std::pair<int,int> target_pos_pair){
        Edge e(pos, x.bs, x.sum + value, x.depth + 1);

        auto set_bitset = [&e](bool bs_flag, int pos){
            if(!bs_flag)
                return ;

            if(!((e.bs >> (pos * 2)) & std::bitset<288>(3)).any())
                e.depth = -1;
            e.bs[pos * 2] = e.bs[pos * 2 + 1];
            e.bs[pos * 2 + 1] = 0;

        };

        set_bitset(is_update_bitset & 2, target_pos_pair.first);
        set_bitset(is_update_bitset & 1, target_pos_pair.second);

        e.prev = std::make_pair(x.pos, x.depth);
        return e;
    }

    double average() const{
        if(depth == -1)
            return -1e9;

        return depth ? sum / depth : 0;
    }
};

struct TypicalDpForDouble::MapElement{

    std::vector<double> points;
    std::vector<std::pair<std::list<std::pair<int,int>>, std::list<std::pair<int,int>>>> routes;

    void addRoute(double point, std::pair<std::list<std::pair<int,int>>, std::list<std::pair<int,int>>> route){
        points.push_back(point);
        routes.push_back(route);
    }
};


#endif // TYPICALDPFORDOUBLE_H
