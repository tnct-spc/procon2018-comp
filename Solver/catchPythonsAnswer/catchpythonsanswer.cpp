
#include "catchpythonsanswer.h"

const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> catchPythonsAnswer::agentAct(int){
    std::pair<std::tuple<int,int,int>,std::tuple<int,int,int>> ans;
    procon::Communication Expo;

    boost::python::object result;

    Py_Initialize();
    auto main_ns = boost::python::import("__main__").attr("__dict__");

    try {
           std::ifstream ifs(Path);                             //ここPath
           std::string script((std::istreambuf_iterator<char>(ifs)),
                               std::istreambuf_iterator<char>());
           boost::python::exec(script.c_str(), main_ns);
           boost::python::object func = main_ns["Act"];
           result = func(side, Expo.exportField(field));
       }

    catch (boost::python::error_already_set) {
            PyErr_Print();
        }

    std::vector<int> ins = Expo.py_list_to_std_vector(result);

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    ans = std::make_pair(std::make_tuple(ins.at(0) / 9 + 1,x_list[ins.at(0) % 9],y_list[ins.at(0) % 9]),std::make_tuple(ins.at(1) / 9 + 1,x_list[ins.at(1) % 9],y_list[ins.at(1) % 9]));
    return ans;
}
