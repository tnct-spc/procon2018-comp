#ifndef LASTFORCE_H
#define LASTFORCE_H
#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "field.h"


/*LastForce仕様 LastForceは残りの手を効率的に探索し、勝利が確定しているかどうか調べるアルゴリズムです。
 * 勝利が確定している場合はもちろん勝利するための手を打ちますが、勝利が確定していないときは挙動を切り替えることが出来ます。
 * 勝利が確定していない時の挙動はメンバEdgeCutがTrueかどうかで決まります。EdgeCutがtrueのときは、勝利が確定していないときは何も行動しない((0,0,0),(0,0,0))を返します。
 * EdgeCutがfalseのときは、勝利が確定していないときでも一定の解答を返します。しかし、これらの解答の強さは保証されません。
 * またEdgeCutがTrueのほうが枝きりが働き、高速に動作します。これは有効ターン数にも影響していて、枝切りが働くと2ターン先まで探索することが出来ますが、なしだと1ターンが限度です。
 * 個人的にはEdgeCutをTrueにして、もし詰まなかったらほかのアルゴリズムに変わりに打たせるなどの使い方をおすすめします。
 */
class LastForce : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:


    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int);
    void setParams(std::vector<std::pair<QString, double>> params);
    bool EdgeCut = true;

private:
    bool checkMate = false;
    bool calc(procon::Field field,int depth,int a, int b);
    procon::Field moveAgent(procon::Field field, std::vector<int> act);

    std::pair<int,int> ans;
    std::vector<std::vector<int>> winner;
    int coun = 0;
};

#endif // LASTFORCE_H
