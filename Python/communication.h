#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "field.h"

#include "boost/python.hpp"
#include "boost/python/numpy.hpp"
#include "gamemanager.h"
#include "visualizer.h"

#include <thread>
#include <memory>
#include <tuple>

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
    boost::python::numpy::ndarray reset();
    boost::python::numpy::ndarray exportField(procon::Field& field);
    boost::python::list random(int side);
    boost::python::numpy::ndarray move(boost::python::list act);

    std::vector<int> py_list_to_std_vector( const boost::python::object& iterable );
    boost::python::list std_vector_to_py_list(std::vector<int> vector);

    void agentAct(const int turn, const int agent, const std::tuple<int,int,int> tuple_val);
    void changeTrun();
    int  winner();
    bool isEnable(int side, boost::python::list act); //sideと[移動or破壊(1or2),pos,破壊or移動(1or2),pos];

private:


    std::shared_ptr<GameManager> manager;

    std::vector<std::vector<std::tuple<int,int,int>>> act_stack; //ここは絶対座標での入力なので注意！

    const int grid_x = 12;
    const int grid_y = 8;

};
}

#endif // COMMUNICATION_H
