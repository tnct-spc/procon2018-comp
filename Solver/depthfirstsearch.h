#ifndef DEPTHFIRSTSEARCH_H
#define DEPTHFIRSTSEARCH_H

#include <unordered_map>
#include <algorithm>
#include <list>
#include <queue>
#include <stack>
#include <future>

#include "minimumvisualizer.h"
#include "minimumvisualizerdock.h"

#include "algorithmwrapper.h"

#include "csvio.h"
#include "progresdock.h"

class DepthFirstSearch : public AlgorithmWrapper
{
    // using AlgorithmWrapper::AlgorithmWrapper;
public:
    DepthFirstSearch(const procon::Field& field, int final_turn, bool side);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int now_turn);
    void setParams(std::vector<std::pair<QString, double>> params);

    class  Treap;
    struct TreapNode;
    struct SearchNode;
private:

    // {node, moves, values, depth_size, agent_values};
    std::tuple<std::shared_ptr<SearchNode>, std::list<std::pair<int,int>>, std::vector<std::vector<std::vector<double>>>> calcMove(bool inp_side, int agent, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict);

    std::shared_ptr<SearchNode> createNodeWithDepthSearch(bool inp_side, bool agent, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict);
    std::shared_ptr<SearchNode> createNodeWithBeamSearch (bool inp_side, bool agent, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict);

    void updatePredictData(bool inp_side, bool agent, bool is_adddock);
    std::vector<std::vector<std::vector<double>>> getMovePer(bool inp_side, bool agent, bool is_adddock);

    void addVisualizerToDock(const std::pair<int,int>& size, const std::vector<std::list<std::pair<int,int>>>& route, const std::vector<std::vector<std::vector<int>>>& color, const std::vector<std::vector<double>>& values = std::vector<std::vector<double>>(0));

    std::pair<std::pair<int,int>, int> getMaxAdvMove(std::shared_ptr<SearchNode> age1, std::shared_ptr<SearchNode> age2);


    int maxval = 10;

    std::shared_ptr<MinimumVisualizer> minimum;
    std::shared_ptr<ProgresDock> dock;

    std::stack<std::tuple<std::pair<int,int>, std::vector<std::list<std::pair<int,int>>>, std::vector<std::vector<std::vector<int>>>, std::vector<std::vector<double>>>> dock_stack;

    std::vector<std::vector<std::vector<std::vector<double>>>> predict_per;

    struct BitSetSorter{
        bool operator()(const std::bitset<296>& x, const std::bitset<296>& y) const{
            for(int count = 0; count < 5; ++count){
                unsigned long long value_x = ((x >> (count * 64)) & std::bitset<296>(~0uLL)).to_ullong();
                unsigned long long value_y = ((y >> (count * 64)) & std::bitset<296>(~0uLL)).to_ullong();
                if(value_x != value_y)
                    return value_x > value_y;
            }
            return false;
        }
    };

    bool dock_show = false;
    bool vis_show;

    int loop_count;

    bool use_beamsearch;
    int beam_width;

    // 味方の行動にかける倍率(敵の行動にかける倍率を1としている)
    double ally_weight;

    double ratio;

    int movecount;

    double predict_weight;


    static const bool do_output = false;

    struct RoutesAndNode;

};

struct DepthFirstSearch::RoutesAndNode{

    std::vector<int> indexs;  //深さごとのchildsのindexs
    void CollectIndex(std::shared_ptr<SearchNode> ins);
    std::vector<std::pair<int,int>> route_pos;
    void CollectPos(int side, int agent, procon::Field field);

    int adv = -1e9;

    std::pair<int,int> next_pos;

};

struct DepthFirstSearch::SearchNode : public std::enable_shared_from_this<SearchNode>{
    static const int advinit = -10007.0;
//    static const int movecount = 3;
//    static constexpr double predict_weight = 0.3;
    static const std::vector<int> dx, dy;

    bool flag = true;    //最後の探索用
    int depth, size, real_size, leaf_size;
    double adv, advsum = advinit;

    double predict_weight;
    int movecount;

    bool is_back = false;
    std::unordered_map<int, std::pair<std::shared_ptr<SearchNode>, int>> childs;
    std::pair<SearchNode*, int> parent;

    SearchNode(double adv, int depth, int remain, std::pair<int,int> pos, int side, const std::vector<std::vector<int>>& value, std::vector<std::vector<int>>& state, std::map<std::bitset<296>, std::shared_ptr<SearchNode>, BitSetSorter>& node_map, std::bitset<296>& bs, const std::vector<std::vector<std::vector<double>>>& predict, double predict_weight, int movecount);
    SearchNode(double adv, int depth, double predict_weight, int movecount);

    void dfsAdd(std::pair<int,int> pos, std::vector<std::vector<std::vector<double>>>& vec);

    double getAdvSum();
    std::pair<int,int> getMaxAdvMove();
};

using value_type = std::pair<double, std::shared_ptr<DepthFirstSearch::SearchNode>>;

using np = std::shared_ptr<DepthFirstSearch::TreapNode>;

struct DepthFirstSearch::TreapNode{

    static np nil;

    value_type val;
    uint32_t pri;
    int size;
    np l = nil;
    np r = nil;

    static int k;

    TreapNode() : val(), pri(rndpri()), size(1), l(nil), r(nil){}
    TreapNode(value_type v) : val(v), pri(rndpri()), size(1), l(nil), r(nil){}
    TreapNode(value_type v, uint32_t p) : val(v), pri(p), size(1), l(nil), r(nil){}

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

class DepthFirstSearch::Treap{
public:
    np root;
    Treap();
    Treap(value_type val);
    Treap(std::vector<value_type>::iterator st, std::vector<value_type>::iterator en);
    Treap(std::vector<value_type> v);

protected:
    int _size(np x);
    np _update(np x);
    np _merge(np l, np r);
    std::pair<np,np> _split(np x, int k);
    np _insert(np x, int k, value_type val);
    np _erase(np x, int k);
    np _erase_back(np x, int k);
    void _set(np x, int k, value_type val);
    value_type _get(np x, int k);
    int _lowerbound(np x, value_type val);
    np _insert(np x, value_type val);

public:
    void push_front(value_type val);
    void push_back(value_type val);
    void pop_front();
    void pop_back();

    // rootを含めたサイズの出力
    int size();
    // k番目への代入
    void set(int k, value_type val);
    // k番目の取得
    value_type get(int k);
    // k番目への挿入
    void insert(int k, value_type val);
    // 適切な位置への挿入
    void insert(value_type val);
    // val <= get(k) となるような最小のk
    int lowerbound(value_type val);
    // k番目の要素削除
    void erase(int k);
    // k番目以降の要素削除
    void erase_back(int k);
};

#endif // DEPTHFIRSTSEARCH_H
