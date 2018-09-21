#ifndef DEPTHFIRSTSEARCH_H
#define DEPTHFIRSTSEARCH_H

#include <unordered_map>
#include <algorithm>
#include <list>

#include "minimumvisualizer.h"
#include "minimumvisualizerdock.h"

#include "algorithmwrapper.h"

class DepthFirstSearch : public AlgorithmWrapper
{
    // using AlgorithmWrapper::AlgorithmWrapper;
public:
    DepthFirstSearch(const procon::Field& field, int final_turn, bool side);
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int now_turn);

private:
    struct SearchNode;
    std::tuple<std::shared_ptr<SearchNode>, std::list<std::pair<int,int>>, std::vector<std::vector<int>>> depthSearch(int agent, int turn_max);

    const int maxval = 10;

    std::shared_ptr<MinimumVisualizer> minimum;
    std::shared_ptr<MinimumVisualizerDock> dock;

    const bool dock_show = false;
    const bool vis_show = true;

};

struct DepthFirstSearch::SearchNode{

    static const int movecount = 3;
    static const std::vector<int> dx, dy;

    int adv, depth, size;
    int advsum = -1000000007;
    std::unordered_map<int, std::pair<std::shared_ptr<SearchNode>, int>> childs;

    SearchNode(int adv, int depth, int remain, std::pair<int,int> pos, int side, const std::vector<std::vector<int>>& value, std::vector<std::vector<int>>& state);

    void dfsAdd(std::pair<int,int> pos, std::vector<std::vector<int>>& vec);

    int getAdvSum();
    std::pair<int,int> getMaxAdvMove();
};

#endif // DEPTHFIRSTSEARCH_H
