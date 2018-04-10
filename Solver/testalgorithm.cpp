#include "testalgorithm.h"


std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> TestAlgorithm::agentAct(int side){

    manager->getField();

    //ここに処理を書く
    procon::Field field = manager->getField();
    std::vector<std::vector<int>> value_data = field.getValue();

    std::vector<int> x_list = {0, 0, 1, 1, 1, -1, -1, -1};
    std::vector<int> y_list = {-1, 1, -1, 0, 1, -1, 0, 1};

    std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> return_val;

    for(int agent_num = 0; agent_num < 2; ++agent_num){
        std::pair<int,int> agent = field.getAgent(side, agent_num);

        std::tuple<int,int,int,int> hyouka_max = std::make_tuple(-1, 0, 0, 0);

        for(int rotate = 0; rotate < 8; ++rotate){
            if(agent.first + x_list.at(rotate) < 0 || agent.first + x_list.at(rotate) >= field.getSize().first ||
               agent.second + y_list.at(rotate) < 0 || agent.second + y_list.at(rotate) >= field.getSize().second
                    )continue;
            bool put_flag = (field.getState(agent.first + x_list.at(rotate), agent.second + y_list.at(rotate)).first == (side==0?2:1));

            int value = (field.getState(agent.first + x_list.at(rotate), agent.second + y_list.at(rotate)).first == side + 1 ? 0 : value_data.at(agent.first + x_list.at(rotate)).at(agent.second + y_list.at(rotate)));

            if(std::get<0>(hyouka_max) < value)
                hyouka_max = std::make_tuple(value, (put_flag ? 2 : 1), x_list.at(rotate), y_list.at(rotate));
        }


        (agent_num ? return_val.first : return_val.second) = std::make_tuple(std::get<1>(hyouka_max), std::get<2>(hyouka_max), std::get<3>(hyouka_max));
    }

    return return_val;

}
