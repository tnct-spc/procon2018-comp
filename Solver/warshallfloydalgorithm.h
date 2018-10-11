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
    static const std::vector<int> dx, dy;

    std::pair<int, int> getPosPair(int x);
    int getPosValue(std::pair<int,int> x);
    int getRotatePos(int pos, int rotate);
    bool outOfRange(int pos, int rotate);

    int size_x, size_y, size_sum;

    const int maxval = 30;

    std::shared_ptr<MinimumVisualizerDock> dock;

};

struct WarshallFloydAlgorithm::Edge{

    int pos;
    std::pair<int,int> prev;
    double sum;
    int length;

    Edge(int pos, double sum = 0.0, int length = -1) :
        pos(pos),
        prev(pos, length),
        sum(sum),
        length(length)
    {
    }


    friend Edge operator+(const Edge& a, const Edge& b){
        Edge e(b.pos, a.sum + b.sum, a.length + b.length);
        e.prev = a.prev;

        return e;
    }

    friend bool operator<(const Edge& a, const Edge& b){
        return a.average() < b.average();
    }

    static Edge make(const Edge& x, int pos, double value, int length){
        Edge e(pos, x.sum + value, x.length + length);
        e.prev = std::make_pair(x.pos, x.length);
        return e;
    }

    double average() const{
        if(length == -1)
            return -1e9;

        return length ? sum / length : 0;
    }
};


#endif // WARSHALLFLOYDALGORITHM_H
