#ifndef SEARCHNODE_H
#define SEARCHNODE_H

#include <vector>
#include <random>
#include <cmath>
#include "field.h"
#include "gamesimulator.h"

class SearchNode
{
public:

    SearchNode(SearchNode *parent, int side, int try_count = 0);

    bool trySimulate(GameSimulator* sim);

    //展開
    void expandNode(GameSimulator* sim);

    //UCB定数 sqrt(2)が基本
    static constexpr double ucb_value = 1.41421356;

    //閾値 これ以上試行した場合は葉を展開する
    static constexpr int threshold = 5;

    static int all_pat_try_count;

    SearchNode *parent;

    int side;

    unsigned int win_count = 0;
    unsigned int try_count;

    //indexlistの方は動ける値のindexを、nodelistの方はnode本体を格納している(node_list[index_list[i]]はnullにならない)

    std::vector<int> can_move_index_list;
    std::vector<SearchNode*> can_move_node_list;

    /*

      まずはmontecarlotreesearchからrootに対して(simulatorのポインタ渡して)関数実行
      関数trySimulate(*simulator)はboolで成否を返す

      std::setで移動の可否を調べる 移動可否はO(1)なので大丈夫なはず

      閾値を設定し、それ以上の場合は葉を展開する
      葉が展開できなくなった場合はそこからプレイアウト

      実装としては bool flag = child[rand]->trySimulate; win_count+=flag; try_count++;return flag;みたいに

    */

};

#endif // SEARCHNODE_H
