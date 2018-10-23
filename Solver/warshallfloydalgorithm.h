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

    struct Edge;
    struct MapElement;

    std::map<std::pair<int,int>, MapElement> route_map_agent0;
    std::map<std::pair<int,int>, MapElement> route_map_agent1;
    struct Parameters{

        int maxdepth_max = 10;

        double conflict_def_per = 1.3;
        double conflict_atk_per = 0.3;
        double conflict_ally_per = 0.3;

        double bound_val = 0.19;

        double pena_ratio = 0.03;

        bool fix_conflict = false;  //falseにするとconflict処理なし

        std::vector<double> point_depth_weight = {
            1.0, 0.9995497206321, 0.9965839655084, 0.9863446263349, 0.9614730743376, 0.9120101602625, 0.8253962143756, 0.6864710464629, 0.47747394583040004, 0.17804368130409998
        };
        std::vector<double> route_length_weight = {
           1.0, 0.997528331, 0.988562982, 0.9682778049999999, 0.9318466519999999, 0.8744433749999999, 0.791241826, 0.677415857, 0.52813932, 0.33858606700000005, 0.10392995000000002
        };
    };

    void setParams(Parameters& param);


private:

    Parameters params;

    std::vector<std::pair<int, std::pair<int,int>>> calcSingleAgent(int agent);

    std::vector<std::vector<Edge>> calcDijkStra(int start_pos, int maxval, bool agent);
    std::pair<double, std::list<std::pair<int, int>>> getRoute(std::vector<std::vector<Edge>>& input, int target_pos, int depth);

    static const std::vector<int> dx, dy;

    std::pair<int, int> getPosPair(int x);
    int getPosValue(std::pair<int,int> x);
    int getRotatePos(int pos, int rotate);
    bool outOfRange(int pos, int rotate);

    int size_x, size_y, size_sum;

    std::shared_ptr<ProgresDock> dock;

    bool FixConflict = false;  //falseにするとconflict処理なし
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

