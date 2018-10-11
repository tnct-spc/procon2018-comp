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

    std::shared_ptr<MinimumVisualizerDock> dock;

};

struct WarshallFloydAlgorithm::Edge{

    int begin, end, next;
    double sum;
    int length;

    Edge(int begin, int end, double sum = 0.0, int length = -1) :
        begin(begin),
        end(end),
        next(end),
        sum(sum),
        length(length)
    {
    }

    friend Edge operator+(const Edge& a, const Edge& b){
        Edge e(a.begin, b.end, a.sum + b.sum, a.length + b.length);
        e.next = b.begin;
        return e;
    }

    friend bool operator<(const Edge& a, const Edge& b){
        return a.average() < b.average();
    }

    double average() const{
        return length <= 0 ? -1e9 : sum / length;
    }
};


#endif // WARSHALLFLOYDALGORITHM_H
