#ifndef DEPTHFIRSTSEARCH_H
#define DEPTHFIRSTSEARCH_H

#include <unordered_map>
#include <algorithm>

#include "algorithmwrapper.h"

class DepthFirstSearch : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int now_turn);

private:
    struct SearchNode;
    std::shared_ptr<SearchNode> depthSearch(int agent, int turn_max);

    const int maxval = 10;

    struct pairHash{
        size_t operator()(const std::pair<int,int>& key) const;
    };
    struct pairEqual{
        bool operator()(const std::pair<int,int>& left, const std::pair<int,int>& right) const{
            return left == right;
        }
    };

};

struct DepthFirstSearch::SearchNode{

    static const int movecount = 3;
    static const std::vector<int> dx, dy;

    int adv, depth;
    std::unordered_map<int, std::shared_ptr<SearchNode>> childs;

    SearchNode(int adv, int depth, int remain, std::pair<int,int> pos, int side, const procon::Field& field, std::unordered_map<std::pair<int,int>, int, pairHash, pairEqual>& used);
};

const std::vector<int> DepthFirstSearch::SearchNode::dx({1, 1, 0, -1, -1, -1, 0, 1});
const std::vector<int> DepthFirstSearch::SearchNode::dy({0, -1, -1, -1, 0, 1, 1, 1});

#endif // DEPTHFIRSTSEARCH_H
