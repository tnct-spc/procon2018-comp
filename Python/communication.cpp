#include "communication.h"

namespace np = boost::python::numpy;


procon::Communication::Communication()
{
    manager = std::make_shared<GameManager>(grid_x, grid_y,false);

}

np::ndarray procon::Communication::reset(){
    manager->resetManager(grid_x, grid_y, false);
    return exportField(manager->getField());
}



np::ndarray procon::Communication::exportField(procon::Field& field){
    std::vector<int> field_arr(308,0);
    field_arr.at(0)=field.getSize().first;
    field_arr.at(1)=field.getSize().second;
    int index = 2;
    for(int x = 0 ; x < field.getSize().first;x++){
        for(int y = 0; y < field.getSize().second ; y++){
            field_arr.at(index+y*12+x) = field.getState(x,y).second;
        }
    }
    index+=144;
    for(int x = 0 ; x < field.getSize().first;x++){
        for(int y = 0; y < field.getSize().second ; y++){
            field_arr.at(index+y*12+x) = field.getState(x,y).first;
        }
    }
    index += 144;
    field_arr.at(index + 1) = field.getTurnCount();
    field_arr.at(index + 2) = field.getFinalTurn();
    index += 2;
    for(int side = 0;side < 2;side++){
        for(int hoge = 0; hoge < 2;hoge++){
            field_arr.at(index + 1) = field.getAgent(side,hoge).first;
            field_arr.at(index + 2) = field.getAgent(side,hoge).second;
            index+=2;
        }
    }
    for(int side = 0; side < 2;side++){
        field_arr.at(index+1)=field.getPoints().at(side).first;
        field_arr.at(index+2)=field.getPoints(false).at(side).second;
        index+=2;
    }
    Py_intptr_t shape[1] = {308};
    np::ndarray result = np::zeros(1, shape, np::dtype::get_builtin<int>());
    std::copy(field_arr.begin(), field_arr.end(), reinterpret_cast<double*>(result.get_data()));
    return result;
}

boost::python::list procon::Communication::random(int side){

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::vector<std::pair<std::pair<int,int>, std::pair<int,int>>> vec;
    for(int a = 0 ; a < 9 ; a++){
        for(int b = 0 ; b < 9 ; b++){
            if(manager->getField().canPut(side, a, b, true)){
                int w1 = manager->getField().getState(manager->getField().getAgent(side,1).first + x_list[a],manager->getField().getAgent(side,1).second + y_list[a]).first;
                int w2 = manager->getField().getState(manager->getField().getAgent(side,1).first + x_list[b],manager->getField().getAgent(side,1).second + y_list[b]).first;

                if((w1 == side +1 || w1 == 0)&&(w2 == side + 1|| w2 == 0)){
                    vec.push_back(std::make_pair(std::make_pair(1,1), std::make_pair(a,b)));
                }

                if(w1 != side + 1&&w1 != 0 && w2 != 0&&w2 != side+1){
                    vec.push_back(std::make_pair(std::make_pair(1,1), std::make_pair(a,b)));
                }

                if((w1 == side +1 || w1 == 0)&& w2 != 0&&w2 != side + 1){
                    vec.push_back(std::make_pair(std::make_pair(1,2), std::make_pair(a,b)));
                }

                if(w1 != side+1 && w1 != 0 && (w2 == side+1 || w2 == 0)){
                    vec.push_back(std::make_pair(std::make_pair(2,1), std::make_pair(a,b)));
                }
            }


        }
    }

    std::random_device rnd;     // 非決定的な乱数生成器でシード生成機を生成
    std::mt19937 mt(rnd()); //

    std::uniform_int_distribution<> rand_move(0, vec.size()-1);
    int s = rand_move(mt);
    std::vector<int> hoge;
    hoge.push_back(vec.at(s).first.first);
    hoge.push_back(vec.at(s).first.second);
    hoge.push_back(vec.at(s).second.first);
    hoge.push_back(vec.at(s).second.second);
    typename std::vector<int>::const_iterator it;

    boost::python::list py_list;

    for(it = hoge.begin(); it != hoge.end(); ++it)
             py_list.append(*it);
        return py_list;
}


std::vector<int> procon::Communication::py_list_to_std_vector( const boost::python::object& iterable )
{
    return std::vector<int>( boost::python::stl_input_iterator<int>( iterable ),
                             boost::python::stl_input_iterator<int>( ) );
}



boost::python::numpy::ndarray procon::Communication::move(boost::python::list act){
    std::vector<int> Act = py_list_to_std_vector(act);

    manager->agentAct(0,0,std::make_tuple(Act.at(0),Act.at(1),Act.at(2)));
    manager->agentAct(0,1,std::make_tuple(Act.at(3),Act.at(4),Act.at(5)));
    manager->agentAct(1,0,std::make_tuple(Act.at(6),Act.at(7),Act.at(8)));
    manager->agentAct(1,1,std::make_tuple(Act.at(9),Act.at(10),Act.at(11)));

    manager->changeTurn(true);

    return exportField(manager->getField());

}

int procon::Communication::winner(){
    int s = manager->getField().whichWin();
    return s == 0 ? -1 : 1;
}

bool procon::Communication::isEnable(int side, boost::python::list act){

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};


    std::vector<int> Act = py_list_to_std_vector(act);
    bool result1 = false;
    bool result2 = false;

    if(manager->getField().canPut(side, Act.at(1), Act.at(3), true)){
        int a = Act.at(1);
        int b = Act.at(3);
        int w1 = manager->getField().getState(manager->getField().getAgent(side,1).first + x_list[a],manager->getField().getAgent(side,1).second + y_list[a]).first;
        int w2 = manager->getField().getState(manager->getField().getAgent(side,1).first + x_list[b],manager->getField().getAgent(side,1).second + y_list[b]).first;

        if(Act.at(0) == 1 && (w1 == side + 1 || w1 == 0)){
            result1 = true;
        }
        if(Act.at(0) == 2 && w1 != 0){
            result1 = true;
        }
        if(Act.at(2) == 1 && (w2 == side + 1 || w2 == 0)){
            result2 = true;
        }
        if(Act.at(2) == 2 && w2 != 0){
            result2 = true;
        }

    }
    return result1 && result2;
}

BOOST_PYTHON_MODULE(Communication)
{
    using namespace boost::python;

    class_<procon::Communication>("Communication")
        .def("reset", &procon::Communication::reset)
        .def("move", &procon::Communication::move)
        .def("random", &procon::Communication::winner)
        .def("isEnable", &procon::Communication::isEnable);

}
