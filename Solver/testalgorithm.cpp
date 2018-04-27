#include "testalgorithm.h"


std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> TestAlgorithm::agentAct(int side){

    manager->getField();

    //ここに処理を書く
    procon::Field field = manager->getField();
    std::vector<std::vector<int>> value_data = field.getValue();

    std::vector<int> x_list = {0, 0, 1, 1, 1, -1, -1, -1};
    std::vector<int> y_list = {-1, 1, -1, 0, 1, -1, 0, 1};

    std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> return_val;

    std::random_device rnd;
    std::mt19937 mt(rnd());

    for(int agent_num = 0; agent_num < 2; ++agent_num){
        std::pair<int,int> agent = field.getAgent(side, agent_num);

        std::vector<int> rotate_value;
        std::vector<std::tuple<int,int,int>> hyouka_max;

        rotate_value.push_back(0);
        hyouka_max.push_back(std::make_tuple(0,0,0));

        for(int rotate = 0; rotate < 8; ++rotate){
            if(agent.first + x_list.at(rotate) < 0 || agent.first + x_list.at(rotate) >= field.getSize().first ||
               agent.second + y_list.at(rotate) < 0 || agent.second + y_list.at(rotate) >= field.getSize().second
                    )continue;
            bool put_flag = (field.getState(agent.first + x_list.at(rotate), agent.second + y_list.at(rotate)).first == (side==0?2:1));

            int value = (field.getState(agent.first + x_list.at(rotate), agent.second + y_list.at(rotate)).first == side + 1 ? -10 : value_data.at(agent.first + x_list.at(rotate)).at(agent.second + y_list.at(rotate)));


            rotate_value.push_back(rotate_value.back() + value + 17);
            hyouka_max.push_back(std::make_tuple( (put_flag ? 2 : 1), x_list.at(rotate), y_list.at(rotate)));
        }

        std::uniform_int_distribution<> random_dist(1, rotate_value.back());

        auto lower_it = std::lower_bound(rotate_value.begin(), rotate_value.end(), random_dist(mt));

        if(lower_it == rotate_value.end())
            (agent_num ? return_val.second : return_val.first) = std::make_tuple(0, 0, 0);

        else{
            std::tuple<int,int,int> return_tuple = hyouka_max.at(std::distance(rotate_value.begin(), lower_it));
            (agent_num ? return_val.second : return_val.first) = return_tuple;
        }
    }

    return return_val;

}
