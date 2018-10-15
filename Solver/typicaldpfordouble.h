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

    int size_x, size_y, size_sum;
    int maxval = 10;

    std::shared_ptr<ProgresDock> dock;

    static const std::vector<int> dx, dy;
    struct Edge;
};

struct TypicalDpForDouble::Edge{

    int pos;
    std::pair<int,int> prev;
    double sum;
    int depth;
    std::bitset<144> bs;

    Edge(int pos, const std::bitset<144>& state, double sum = 0.0, int depth = -1) :
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

    static Edge make(const Edge& x, int pos, double value, int depth, bool is_nocount){
        Edge e(pos, x.bs, x.sum + value, x.depth + depth);

        if(!(e.bs[pos] || is_nocount))
            e.depth = -1;
        e.bs[pos] = 0;

        e.prev = std::make_pair(x.pos, x.depth);
        return e;
    }

    double average() const{
        if(depth == -1)
            return -1e9;

        return depth ? sum / depth : 0;
    }
};


#endif // TYPICALDPFORDOUBLE_H
