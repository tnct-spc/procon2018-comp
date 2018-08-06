#include "convertdictfield.h"

boost::python::dict procon::ConvertDictField::convertDictFromField(procon::Field& field)
{
    boost::python::dict dic;

    boost::python::list siz;
    siz.append(field.getSize().first);
    siz.append(field.getSize().second);
    dic["siz"] = siz;

    boost::python::list state;
    boost::python::list value;
    for(int x = 0; x < field.getSize().first; ++x){

        boost::python::list stlist;
        boost::python::list valist;
        for(int y = 0; y < field.getSize().second; ++y){
            int st, va;
            std::tie(st, va) = field.getState(x, y);

            stlist.append(st);
            valist.append(va);
        }
        state.append(std::move(stlist));
        value.append(std::move(valist));
    }
    dic["state"] = std::move(state);
    dic["value"] = std::move(value);

    dic["now_turn"] = field.getTurnCount();
    dic["final_turn"] = field.getFinalTurn();

    boost::python::list agents;
    for(int side = 0; side < 2; ++side){
        boost::python::list agents_side;
        for(int agent = 0; agent < 2; ++agent){

            boost::python::list agents_agent;
            agents_agent.append(field.getAgent(side, agent).first);
            agents_agent.append(field.getAgent(side, agent).second);

            agents_side.append(std::move(agents_agent));
        }
        agents.append(std::move(agents_side));
    }

    boost::python::list points;
    const std::vector<std::pair<int,int>>& points_vec = field.getPoints();

    for(int side = 0; side < 2; ++side){

        boost::python::list point_side;
        point_side.append(points_vec.at(side).first);
        point_side.append(points_vec.at(side).second);

        points.append(std::move(point_side));
    }

    dic["point"] = std::move(points);

    return dic;
}

BOOST_PYTHON_MODULE(basic){

    boost::python::def("convert_dict_from_field", &procon::ConvertDictField::convertDictFromField);
}
