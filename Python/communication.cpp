#include "communication.h"

namespace np = boost::python::numpy;
namespace p = boost::python;

procon::Communication::Communication() :
    field(grid_x, grid_y)
{
	act_stack.resize(2);
	act_stack[0].resize(2); 
	act_stack[1].resize(2); 

}

p::list procon::Communication::resetField(){
    std::random_device rnd;     // 非決定的な乱数生成器でシード生成機を生成
    std::mt19937 mt(rnd()); //

    std::uniform_int_distribution<> rand_siz(8, 12);
    std::uniform_int_distribution<> rand_turn(60, 120);

    field = procon::Field(rand_siz(mt), rand_siz(mt), 16, -16);
	field.setFinalTurn(rand_turn(mt));
    return exportField(field);
}



p::list procon::Communication::exportField(procon::Field field){
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

boost::python::list procon::Communication::random(int side){

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    std::vector<std::pair<std::pair<int,int>, std::pair<int,int>>> vec;
    for(int a = 0 ; a < 9 ; a++){
        for(int b = 0 ; b < 9 ; b++){
            if(field.canPut(side, a, b, true)){
                int w1 = field.getState(field.getAgent(side,0).first + x_list[a], field.getAgent(side,0).second + y_list[a]).first;
                int w2 = field.getState(field.getAgent(side,1).first + x_list[b], field.getAgent(side,1).second + y_list[b]).first;

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

void procon::Communication::agentAct(const int turn, const int agent, const std::tuple<int, int, int> tuple_val){

    int type, x_inp, y_inp;
    std::tie(type, x_inp, y_inp) = tuple_val;


    std::pair<int,int> agent_pos = field.getAgent(turn, agent);
    std::pair<int,int> grid_size = field.getSize();
    int x_pos = agent_pos.first + x_inp;
    int y_pos = agent_pos.second + y_inp;

    if(
        type == 0 ||
        x_pos < 0 || x_pos >= grid_size.first ||
        y_pos < 0 || y_pos >= grid_size.second ||
        (type == 1 && field.getState(x_pos, y_pos).first == (turn==1 ? 1 : 2)) ||
        (type == 2 && field.getState(x_pos, y_pos).first == 0)
        ){
        act_stack.at(turn).at(agent) = std::make_tuple(1, agent_pos.first, agent_pos.second);
        return ;
    }
    act_stack.at(turn).at(agent) = std::make_tuple(type, x_pos, y_pos);
}
boost::python::list procon::Communication::move(boost::python::list act){
    std::vector<int> Act = py_list_to_std_vector(act);

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    agentAct(0,0,std::make_tuple(Act.at(0) / 9 + 1,x_list[Act.at(0) % 9],y_list[Act.at(0) % 9]));
    agentAct(0,1,std::make_tuple(Act.at(1) / 9 + 1,x_list[Act.at(1) % 9],y_list[Act.at(1) % 9]));
    agentAct(1,0,std::make_tuple(Act.at(2) / 9 + 1,x_list[Act.at(2) % 9],y_list[Act.at(2) % 9]));
    agentAct(1,1,std::make_tuple(Act.at(3) / 9 + 1,x_list[Act.at(3) % 9],y_list[Act.at(3) % 9]));

    changeTurn();

    return exportField(field);

}
void procon::Communication::changeTurn(){

    //[(x,y)]:(上書き時の色,(色,エージェント)) わかりづらいね
    std::map<std::pair<int,int>,std::pair<int,std::pair<int,int>>> counts;

    int type, pos_x, pos_y;


    //移動しようとしたエージェントが失敗した時に呼ばれる
    std::function<void(std::pair<int,int>)> delete_move = [&](std::pair<int,int> agent_data){

        std::pair<int,int> not_move = field.getAgent(agent_data.first, agent_data.second);

        //もう既に埋まっていて、それが移動予定erなら

        if(counts[not_move].first >= 0){

            std::pair<int,int> next_delete_move = counts[not_move].second;

            counts[not_move] = std::make_pair(-1, std::make_pair(-1, -1));
            //循環参照ケースの回避
            if(next_delete_move != agent_data)
                delete_move(next_delete_move);
        }

        counts[not_move] = std::make_pair(-1, std::make_pair(-1, -1));
    };


    for(int side = 0; side < 2; ++side){
        for(int agent = 0; agent < 2; ++agent){
            std::tie(type, pos_x, pos_y) = act_stack.at(side).at(agent);

            //移動しないなら
            if(type != 1){
                std::pair<int,int> not_move = field.getAgent(side, agent);

                if(counts[not_move].first > 0){//移動しようとしているアレのコンフリクト
                    delete_move(counts[not_move].second);
                    delete_move(std::make_pair(side, agent));
                }

                counts[not_move] = std::make_pair(-1,std::make_pair(-1, -1));
            }

            //もう既に存在しているなら
            if(counts.count(std::make_pair(pos_x, pos_y) )){

                if(counts[std::make_pair(pos_x, pos_y)].first > 0){
                    delete_move(counts[std::make_pair(pos_x, pos_y)].second);
                    delete_move(std::make_pair(side, agent));
                }

                counts[std::make_pair(pos_x, pos_y)] = std::make_pair(-1, std::make_pair(-1, -1));
            }else{
                int color = 0;
                if(type != 2)
                    color = side + 1;

                counts[std::make_pair(pos_x, pos_y)] = std::make_pair(color ,std::make_pair(side,agent));
            }
        }
    }

    for(auto moves : counts){
        if(moves.second.first == -1)
            continue;

        field.setState(moves.first.first, moves.first.second, moves.second.first);

        if(moves.second.first != 0)
            field.setAgent(moves.second.second.first, moves.second.second.second, moves.first.first, moves.first.second);
    }

    field.setTurnCount(field.getTurnCount() + 1);

    /*
    std::map<std::pair<int,int>,std::vector<std::pair<int,int>>> dest_map;
    std::map<std::pair<int,int>,std::vector<std::pair<int,int>>> tile_map;
    int type, pos_x, pos_y;
    for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
        for(int agent_num = 0; agent_num < 2; ++agent_num){
            std::tie(type, pos_x, pos_y) = act_stack.at(turn_flag).at(agent_num);
            std::pair<int,int> pos = std::make_pair(pos_x, pos_y);
            if(type == 1){
                dest_map[ pos ].push_back( std::make_pair(turn_flag, agent_num) );
            }else
            if(type == 2){
                tile_map[ pos ].push_back( std::make_pair(turn_flag, agent_num) );
                dest_map[ field->getAgent(turn_flag,agent_num) ].push_back( std::make_pair(turn_flag, agent_num) );
            }
        }
    for(auto elements : dest_map){
        if(elements.second.size() > 1)
            continue;
        if(field->getState(elements.first.first, elements.first.second).first == (elements.second.at(0).first == 0 ? 2 : 1))
            continue;
        field->setAgent(elements.second.at(0).first, elements.second.at(0).second, elements.first.first, elements.first.second);
        field->setState(elements.first.first, elements.first.second, elements.second.at(0).first + 1);
    }
    for(auto elements : tile_map){
        bool state_flag = true;
        if(elements.second.size() > 1)
            continue;
        for(int turn_flag = 0; turn_flag < 2; ++turn_flag)
            for(int agent_num = 0; agent_num < 2; ++agent_num)
                if(field->getAgent(turn_flag, agent_num) == elements.first){
                    state_flag = false;
                    break;
                }
        if(state_flag)
            field->setState(elements.first.first, elements.first.second, 0);
    }
    */

    //得点の更新処理(エージェント側でやるよりこちらの方がよい)
    field.updatePoint();

}

int procon::Communication::winner(){
    int s = field.whichWin();
    return s == 0 ? -1 : 1;
}

bool procon::Communication::isEnable(int side, boost::python::list act){

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};


    std::vector<int> Act = py_list_to_std_vector(act);
    bool result1 = false;
    bool result2 = false;

    if(field.canPut(side, Act.at(1), Act.at(3), true)){
        int a = Act.at(1);
        int b = Act.at(3);
        int w1 = field.getState(field.getAgent(side,1).first + x_list[a],field.getAgent(side,1).second + y_list[a]).first;
        int w2 = field.getState(field.getAgent(side,1).first + x_list[b],field.getAgent(side,1).second + y_list[b]).first;

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



BOOST_PYTHON_MODULE(communication)
{
    using namespace boost::python;

    class_<procon::Communication>("Communication")
        .def("resetField", &procon::Communication::resetField)
        .def("move", &procon::Communication::move)
        .def("random", &procon::Communication::random)
        .def("winner", &procon::Communication::winner)
        .def("isEnable", &procon::Communication::isEnable);
}
