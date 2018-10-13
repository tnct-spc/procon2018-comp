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

    class  Treap;
    struct TreapNode;

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

    double conflict_def_per = 0.3;
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


using tr_type = std::pair<double, std::list<std::pair<int,int>>>;

using np_2 = std::shared_ptr<WarshallFloydAlgorithm::TreapNode>;

struct WarshallFloydAlgorithm::TreapNode{

    static np_2 nil;

    tr_type val;
    uint32_t pri;
    int size;
    np_2 l = nil;
    np_2 r = nil;

    static int k;

    TreapNode() : val(), pri(rndpri()), size(1), l(nil), r(nil){}
    TreapNode(tr_type v) : val(v), pri(rndpri()), size(1), l(nil), r(nil){}
    TreapNode(tr_type v, uint32_t p) : val(v), pri(p), size(1), l(nil), r(nil){}

    static uint32_t rndpri() {
        static uint32_t x = 123456789, y = 362436069, z = 521288629, w = time(0);
        uint32_t t = x ^ (x << 11);
        x = y;
        y = z;
        z = w;
        w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
        return std::max<uint32_t>(1, w & 0x3FFFFFFF);
    }
};

class WarshallFloydAlgorithm::Treap{
public:
    np_2 root;
    Treap();
    Treap(tr_type val);
    Treap(std::vector<tr_type>::iterator st, std::vector<tr_type>::iterator en);
    Treap(std::vector<tr_type> v);

protected:
    int _size(np_2 x);
    np_2 _update(np_2 x);
    np_2 _merge(np_2 l, np_2 r);
    std::pair<np_2,np_2> _split(np_2 x, int k);
    np_2 _insert(np_2 x, int k, tr_type val);
    np_2 _erase(np_2 x, int k);
    np_2 _erase_back(np_2 x, int k);
    void _set(np_2 x, int k, tr_type val);
    tr_type _get(np_2 x, int k);
    int _lowerbound(np_2 x, tr_type val);
    np_2 _insert(np_2 x, tr_type val);

public:
    void push_front(tr_type val);
    void push_back(tr_type val);
    void pop_front();
    void pop_back();

    // rootを含めたサイズの出力
    int size();
    // k番目への代入
    void set(int k, tr_type val);
    // k番目の取得
    tr_type get(int k);
    // k番目への挿入
    void insert(int k, tr_type val);
    // 適切な位置への挿入
    void insert(tr_type val);
    // val <= get(k) となるような最小のk
    int lowerbound(tr_type val);
    // k番目の要素削除
    void erase(int k);
    // k番目以降の要素削除
    void erase_back(int k);
};

#endif // WARSHALLFLOYDALGORITHM_H

