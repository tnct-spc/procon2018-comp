#ifndef WARSHALLFLOYDALGORITHM_H
#define WARSHALLFLOYDALGORITHM_H

#include "algorithmwrapper.h"
#include "progresdock.h"

class WarshallFloydAlgorithm : public AlgorithmWrapper
{
public:
    WarshallFloydAlgorithm(const procon::Field& field, int final_turn, bool side);

    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int);
    void setParams(std::vector<std::pair<QString, double>>){}

private:
    struct Edge;
    struct MapElement;

    std::vector<std::pair<int, std::pair<int,int>>> calcSingleAgent(int agent);

    std::vector<std::vector<Edge>> calcDijkStra(int start_pos, int maxval);
    std::pair<double, std::list<std::pair<int, int>>> getRoute(std::vector<std::vector<Edge>>& input, int target_pos, int depth);

    static const std::vector<int> dx, dy;

    std::pair<int, int> getPosPair(int x);
    int getPosValue(std::pair<int,int> x);
    int getRotatePos(int pos, int rotate);
    bool outOfRange(int pos, int rotate);

    int size_x, size_y, size_sum;

    int maxdepth_max = 30;

    std::shared_ptr<ProgresDock> dock;

    double conflict_def_per = 1.3;
    double conflict_atk_per = 0.3;

    double bound_val = 0.05;

    double depth_weight = 1.02;
    double depth_weight_max = 1.50;

};

struct WarshallFloydAlgorithm::MapElement{
    std::pair<int,int> pos;
    std::pair<std::vector<double>, std::vector<std::list<std::pair<int,int>>>> routes;
    std::vector<std::vector<int>> put_count;

    MapElement(){

    }

    MapElement(std::pair<int,int> pos) : pos(pos){

    }

    void addRoute(double score, std::list<std::pair<int,int>> route){
        routes.first.push_back(score);
        routes.second.push_back(route);
    }

    void setPutCount(std::pair<int,int> size){
        int size_x, size_y;
        std::tie(size_x, size_y) = size;
        put_count.resize(size_x, std::vector<int>(size_y, 0));

        for(int index = 0; index < routes.first.size(); ++index){
            auto score = routes.first.at(index);
            const auto& route = routes.second.at(index);

            for(auto it = std::next(route.begin(), 1); it != route.end(); ++it)
                ++put_count.at((*it).first).at((*it).second);
        }
    }
};

struct WarshallFloydAlgorithm::Edge{

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


#endif // WARSHALLFLOYDALGORITHM_H

