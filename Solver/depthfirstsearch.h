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

private:
    struct SearchNode;
    std::tuple<std::shared_ptr<SearchNode>, std::list<std::pair<int,int>>, std::vector<std::vector<std::vector<double>>>, std::vector<int>, std::vector<std::vector<std::vector<int>>>> depthSearch(bool inp_side, int agent, int turn_max, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict);
    bool updatePredictData(bool inp_side, bool agent);
    std::vector<std::vector<std::vector<double>>> getMovePer(bool inp_side, bool agent);
    void searchNodeDfsCreate(bool inp_side, int agent, int turn_max, std::vector<std::vector<int>>& state, const std::vector<std::vector<std::vector<double>>>& predict);

    // bool randPer(double bound);

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

    // 味方の行動にかける倍率(敵の行動にかける倍率を1としている)
    static constexpr double ally_weight = 1.0;

    static const bool do_output = false;

    std::pair<std::pair<int,int>, int> getMaxAdvMove(std::shared_ptr<SearchNode> age1, std::shared_ptr<SearchNode> sge2);

    const double ratio = 0.03;

    struct RoutesAndNode;

    int threshold = 1;


};

struct DepthFirstSearch::RoutesAndNode{

    std::vector<int> indexs;  //深さごとのchildsのindexs
    void CollectIndex(std::shared_ptr<SearchNode> ins);
    std::vector<std::pair<int,int>> route_pos;
    void CollectPos(int side, int agent, procon::Field field);

    int adv = -1e9;

    std::pair<int,int> next_pos;

};

struct DepthFirstSearch::SearchNode{
    static const int advinit = -10007.0;
    static const int movecount = 3;
    static constexpr double predict_weight = 0.3;
    static const std::vector<int> dx, dy;

    bool flag = true;    //最後の探索用
    int depth, size, real_size, leaf_size;
    double adv, advsum = advinit;
    bool is_back = false;
    std::unordered_map<int, std::pair<std::shared_ptr<SearchNode>, int>> childs;

    SearchNode(double adv, int depth, int remain, std::pair<int,int> pos, int side, const std::vector<std::vector<int>>& value, std::vector<std::vector<int>>& state, std::map<std::bitset<296>, std::shared_ptr<SearchNode>, BitSetSorter>& node_map, std::bitset<296>& bs, const std::vector<std::vector<std::vector<double>>>& predict);

    void dfsAdd(std::pair<int,int> pos, std::vector<std::vector<std::vector<double>>>& vec, std::vector<std::vector<std::vector<int>>>& agent_vec);

    double getAdvSum();
    std::pair<int,int> getMaxAdvMove();
};

#endif // DEPTHFIRSTSEARCH_H
