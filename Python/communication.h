#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "field.h"
#pragma push_macro("slots")
#undef slots
#include "boost/python.hpp"
#include "boost/python/numpy.hpp"
#pragma pop_macro("slots")


#include <thread>
#include <memory>
#include <tuple>
#include <iostream>

/*関数名は適当でいいので、
boost::numpy:ndarray reset()
 ←盤面初期化してfieldの配列を返すやつ
boost::numpy::ndarray move(boost::python::list)
←行動が渡されてchangeTurnしてfieldの配列を返すやつ
boost::python::list random()
←有効な手のなかで適当に返すやつ
int winner()
←勝ちが決まった後にside=0側から見て{-1,0,1}で{負け,引き分け,勝ち}を返すやつ
 */
namespace procon{

class Communication
{
public:

    Communication();
    boost::python::list resetField();
    boost::python::list exportField(procon::Field field);
    boost::python::list random(int side);
    boost::python::list move(boost::python::list act);

    std::vector<int> py_list_to_std_vector( const boost::python::object& iterable );

    void agentAct(const int turn, const int agent, const std::tuple<int,int,int> tuple_val);
    void changeTurn();
    int  winner();
    bool isEnable(int side, boost::python::list act); //sideと[移動or破壊(1or2),pos,破壊or移動(1or2),pos];


private:

    procon::Field field;


    static const unsigned int grid_x = 12;
    static const unsigned int grid_y = 8;

    std::vector<std::vector<std::tuple<int,int,int>>> act_stack; //ここは絶対座標での入力なので注意！

};
}

#endif // COMMUNICATION_H
