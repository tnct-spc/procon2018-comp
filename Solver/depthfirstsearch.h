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

class DepthFirstSearch : public AlgorithmWrapper
{
    // using AlgorithmWrapper::AlgorithmWrapper;
public:
    DepthFirstSearch(const procon::Field& field, int final_turn, bool side);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int now_turn);

    struct SearchNode;
    class Treap;
    struct TreapNode;
private:

    // {node, moves, values, depth_size, agent_values};
    std::tuple<std::shared_ptr<SearchNode>, std::list<std::pair<int,int>>, std::vector<std::vector<std::vector<double>>>> depthSearch(bool inp_side, int agent, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict);
    std::tuple<std::shared_ptr<SearchNode>, std::list<std::pair<int,int>>, std::vector<std::vector<std::vector<double>>>> beamSearch(bool inp_side, int agent, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict);

    bool updatePredictData(bool inp_side, bool agent);
    std::vector<std::vector<std::vector<double>>> getMovePer(bool inp_side, bool agent);
    // void searchNodeDfsCreate(bool inp_side, int agent, int turn_max, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict);

    int maxval = 10;

    std::shared_ptr<MinimumVisualizer> minimum;
    std::shared_ptr<MinimumVisualizerDock> dock;

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

    static const bool dock_show = false;
    static const bool vis_show = false;

    static const int loop_count = 4;

    static const int beam_width = 100;

    // 味方の行動にかける倍率(敵の行動にかける倍率を1としている)
    static constexpr double ally_weight = 1.0;

    static const bool do_output = false;

};

struct DepthFirstSearch::SearchNode : public std::enable_shared_from_this<SearchNode>{

    static const int movecount = 3;
    static constexpr double predict_weight = 0.3;
    static const std::vector<int> dx, dy;

    int depth, size, real_size, leaf_size;
    double adv, advsum = -10007.0;
    bool is_back = false;
    std::unordered_map<int, std::pair<std::shared_ptr<SearchNode>, int>> childs;
    std::shared_ptr<SearchNode> parent;

    SearchNode(double adv, int depth, int remain, std::pair<int,int> pos, int side, const std::vector<std::vector<int>>& value, std::vector<std::vector<int>>& state, std::map<std::bitset<296>, std::shared_ptr<SearchNode>, BitSetSorter>& node_map, std::bitset<296>& bs, const std::vector<std::vector<std::vector<double>>>& predict);
    SearchNode(double adv, int depth);

    void dfsAdd(std::pair<int,int> pos, std::vector<std::vector<std::vector<double>>>& vec);

    double getAdvSum();
    std::pair<int,int> getMaxAdvMove();
};

using value_type = std::pair<double, std::shared_ptr<DepthFirstSearch::SearchNode>>;

using np = DepthFirstSearch::TreapNode*;

struct DepthFirstSearch::TreapNode{

    static np nil;

    value_type val;
    uint32_t pri;
    int size;
    np l = nil;
    np r = nil;

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
