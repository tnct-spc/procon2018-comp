#ifndef FIELD_H
#define FIELD_H


#include <vector>
#include <utility>
#include <random>
#include <queue>
#include <complex>
#include <iostream>
#include <set>
#include <bitset>
#include <tuple>
#include <map>
#include <functional>
#include <fstream>
#include <sstream>
#include<math.h>
#include <stdlib.h>


namespace procon {

class Field
{
public:
    Field(const unsigned int size_x, const unsigned int size_y);
    Field(const unsigned int size_x, const unsigned int size_y, const std::vector<std::vector<int>>& input_val);
    Field(const unsigned int size_x, const unsigned int size_y,const int max_val,const int min_val, bool use_fieldtype = false);

    const std::vector<std::vector<std::pair<int,int>>>& getAgents() const;

    std::pair<int,int> getAgent(const unsigned int turn, const unsigned int number) const;
   // const std::vector<std::vector<int>>& getField() const;

    const std::vector<std::vector<int>>& getValue() const;

    std::bitset<288> getRegion();

    std::vector<std::pair<int,int>> getPoints(bool flag = true); //UpdatePointを呼ぶか
    std::vector<std::pair<int,int>> getPoints(std::pair<std::pair<int,int>, std::pair<int, int>> pos, bool flag = true);     //引数にteam番号,移動or破壊(0 or 1),座標を持つ、第二引数には実際に得点を書き換えるか(書き換えるならtrue)
    std::vector<std::pair<int,int>> getPoints(std::vector<std::pair<std::pair<int,int>, std::pair<int, int>>> pos_vec, bool flag = true);//上と大体同じ

    void setPoints(int side, std::pair<int,int> value);

    int getTurnCount();
    int getFinalTurn();
    void setTurnCount(int turn_count);
    void setFinalTurn(int turn_count);

   // bool isPlaced(const unsigned int x, const unsigned int y);
    std::pair<int,int> getState(const int x, const int y) const;//pair<タイル状況,評価値>

    void setState(const unsigned int x, const unsigned int y, const unsigned int state);//タイル状況のみの変更
    void setAgent(const unsigned int side, const unsigned int number, const unsigned int x_pos, const unsigned int y_pos);

    bool canPut(const unsigned int side, const unsigned int move_1, const unsigned int move_2, bool double_move = true) const;

    void setSize(const std::pair<int, int> &grid);
    void setValue(const std::vector<std::vector<int>> &value);
    void setGridValue(const unsigned int x, const unsigned int y, const unsigned int value);
    void setStates(const std::vector<std::vector<int>>& values);
    void setAgents(const std::vector<std::vector<std::pair<int,int>>>& values);

    void resetState(const procon::Field& inp_field);

    std::bitset<288>& getField();
    void setField(std::bitset<288>& input);

    std::pair<int,int> getSize() const;

    void updatePoint();

    void updateOnlyTilePoint();


    std::vector<std::pair<int, int>> guessAgents(int side);  //推測する先のside

    int translateMoveToInt(int side, std::tuple<int, int, int> move);

    void updateFeature();
    const std::vector<double>& getFeatures();
    double getFeature(int i); // 1 -> 対称(1なら縦対称,0なら横対称), 2 -> 傾斜量の平均(0~1) ,3 -> 地上開度(3 ~ 10) ,4 -> 地下開度（-10 ~ 0）,5 ->尾根谷度(２つの開度の平均)(-2 ~ +2),6 -> 平均高度差(-2~2),7 -> 最大得点(-16~16),8 -> 最低得点(-16~16),9 -> 横幅, 10 -> 縦幅

    std::vector<double> calcSituationFeature(std::pair<std::tuple<int, int, int>, std::tuple<int, int, int>> agent_pos,int side_of);
    //まず全agentの行動のposを投げて,第二引数にどっち視点で見るか(下の行に書いてある敵味方の話)

    //1 -> 領域点の差の変化 2 -> マスの得点の差の変化 3 -> 味方agents同士の距離の差 4 -> 敵agent同士の距離の差 5 -> 味方と敵の距離の総和の差 6 -> 味方agentと中央との距離の総和の差(大きければ大きいほど外側) 7 -> 敵のagentと中心の距離の総和の差 8 -> 周辺の自分のタイルの割合 9 -> 周辺の敵のタイルの割合 10 -> 現在のターンの進行割合 11 -> マスの埋まり具合 12 -> 残りのターン数

private:
    std::bitset<288> field_data;
    std::vector<std::vector<int>> value_data;

    int now_turn = 0;
    int final_turn = 0;

    //players.at(turn_flag)みたいな事ができるのでvectorにしてます 変えてもいい
    //めっちゃ分かりづらいですね
    std::vector<std::vector<std::pair<int,int>>> agents;

    int grid_x;
    int grid_y;

    int max_val = 16;
    int min_val = -16;

    std::bitset<288> regions;

    std::vector<std::pair<int,int>> points; //それぞれのチームの<踏んだマスの得点,領域得点>を格納

    std::vector<double> feature;

    std::vector<std::vector<int>> createField(int x_size, int y_size, int field_type);
};
}

#endif // FIELD_H
