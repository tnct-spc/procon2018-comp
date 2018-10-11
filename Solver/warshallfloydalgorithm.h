#ifndef WARSHALLFLOYDALGORITHM_H
#define WARSHALLFLOYDALGORITHM_H

#include "algorithmwrapper.h"
#include "minimumvisualizerdock.h"

class WarshallFloydAlgorithm : public AlgorithmWrapper
{
public:
    WarshallFloydAlgorithm(const procon::Field& field, int final_turn, bool side);

    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int);
    void setParams(std::vector<std::pair<QString, double>>){}

private:
    struct Edge;

    std::vector<std::vector<Edge>> calcDijkStra(int start_pos, int maxval);
    std::list<std::pair<int, int>> getRoute(std::vector<std::vector<Edge>>& input, int target_pos, int depth);

    static const std::vector<int> dx, dy;

    std::pair<int, int> getPosPair(int x);
    int getPosValue(std::pair<int,int> x);
    int getRotatePos(int pos, int rotate);
    bool outOfRange(int pos, int rotate);

    int size_x, size_y, size_sum;

    std::shared_ptr<MinimumVisualizerDock> dock;

};

struct WarshallFloydAlgorithm::Edge{

    int pos;
    std::pair<int,int> prev;
    double sum;
    int depth;

    Edge(int pos, double sum = 0.0, int depth = -1) :
        pos(pos),
        prev(pos, depth),
        sum(sum),
        depth(depth)
    {
    }


    friend Edge operator+(const Edge& a, const Edge& b){
        Edge e(b.pos, a.sum + b.sum, a.depth + b.depth);
        e.prev = a.prev;

        return e;
    }

    friend bool operator<(const Edge& a, const Edge& b){
        return a.average() < b.average();
    }

    static Edge make(const Edge& x, int pos, double value, int depth){
        Edge e(pos, x.sum + value, x.depth + depth);
        e.prev = std::make_pair(x.pos, x.depth);
        return e;
    }

    double average() const{
        if(depth == -1)
            return -1e9;

        return depth ? sum / depth : 0;
    }
};


#endif // WARSHALLFLOYDALGORITHM_H
