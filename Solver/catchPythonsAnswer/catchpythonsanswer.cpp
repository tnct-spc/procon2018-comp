
#include "catchpythonsanswer.h"

std::vector<int> catchPythonsAnswer::py_list_to_std_vector( const boost::python::object& iterable )
{
    return std::vector<int>( boost::python::stl_input_iterator<int>( iterable ),
                             boost::python::stl_input_iterator<int>( ) );
}

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> catchPythonsAnswer::agentAct(int){
    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> ans;

    boost::python::object result;

    Py_Initialize();
    auto main_ns = boost::python::import("__main__").attr("__dict__");

    try {
           std::ifstream ifs(Path);  //ここPath
           std::string script((std::istreambuf_iterator<char>(ifs)),
                               std::istreambuf_iterator<char>());
           boost::python::exec(script.c_str(), main_ns);

           boost::python::object func = main_ns["Act"];
           //boost::python::object calc = main_ns["calc"];
           //boost::python::object s = calc(1);

           //std::cout <<  boost::python::extract<double>(s) << std::endl;

           result = func(side, exportField(field));
       }

    catch (boost::python::error_already_set) {
        std::cout<<"ERROR"<<std::endl;
            PyErr_Print();
        }

    std::vector<int> ins = py_list_to_std_vector(result);

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    ans = std::make_pair(std::make_tuple(ins.at(0) / 9 + 1,x_list[ins.at(0) % 9],y_list[ins.at(0) % 9]),std::make_tuple(ins.at(1) / 9 + 1,x_list[ins.at(1) % 9],y_list[ins.at(1) % 9]));
    return ans;
}
boost::python::list catchPythonsAnswer::exportField(procon::Field field){
    std::vector<int> field_arr(304,0);
    int index = 0;
    field_arr.at(0)=field.getSize().first;
    index++;
    field_arr.at(1)=field.getSize().second;
    index++;
    int ins = index;
    for(int x = 0 ; x < field.getSize().first;x++){
        for(int y = 0; y < field.getSize().second ; y++){
            field_arr.at(index+y*12+x) = field.getState(x,y).first;
            ins++;
        }
    }
    ins = 146;
    index = ins;
    for(int x = 0 ; x < field.getSize().first;x++){
        for(int y = 0; y < field.getSize().second ; y++){
            field_arr.at(index+y*12+x) = field.getState(x,y).second;
            ins++;
        }
    }
    ins = 290;
    index = ins;
    field_arr.at(ins) = field.getTurnCount();
    ins++;
    field_arr.at(ins) = field.getFinalTurn();
    ins++;
    for(int side = 0;side < 2;side++){
        for(int hoge = 0; hoge < 2;hoge++){
            field_arr.at(ins) = field.getAgent(side,hoge).first;
            field_arr.at(ins + 1) = field.getAgent(side,hoge).second;
            ++ins;
        }
    }
    index = ins;
    field_arr.at(index)=field.getPoints().at(0).first;
    field_arr.at(index+1)=field.getPoints(false).at(0).second;
    field_arr.at(index+2)=field.getPoints(false).at(1).first;
    field_arr.at(index+3)=field.getPoints(false).at(1).second;

    typename std::vector<int>::const_iterator it;

    boost::python::list hoge;

    for(it = field_arr.begin(); it != field_arr.end(); ++it)
             hoge.append(*it);
        return hoge;

}
