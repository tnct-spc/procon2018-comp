#ifndef DEPTHFIRSTSEARCH_H
#define DEPTHFIRSTSEARCH_H

#include <unordered_map>

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

    struct moveHash{
        size_t operator()(const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>& key) const ;
    };
    struct moveEqual{
        bool operator()(const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>& left, const std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>>& right) const{
            return left == right;
        }
    };

};

struct DepthFirstSearch::SearchNode{

    int depth;
    std::unordered_map<std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>>, std::shared_ptr<SearchNode>, moveHash, moveEqual> childs;

    SearchNode(int depth, int remain, std::pair<int,int> pos, const procon::Field& field, std::unordered_map<std::pair<int,int>, int, pairHash, pairEqual>& used);
};

#endif // DEPTHFIRSTSEARCH_H
