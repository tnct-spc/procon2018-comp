#include "field.h"

procon::Field::Field(const unsigned int size_x ,const unsigned int size_y){
    grid_x = size_x;
    grid_y = size_y;

    //初期化処理がめっちゃ分かりづらいですが、四隅にagentを配置してます
    agents = { { std::make_pair(0, 0), std::make_pair(size_x - 1, size_y - 1) }, { std::make_pair(size_x - 1, 0), std::make_pair(0, size_y - 1) } };

    field_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));
    value_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));

    region_blue = std::vector<std::vector<bool>>(size_x, std::vector<bool>(size_y,false));
    region_red = std::vector<std::vector<bool>>(size_x, std::vector<bool>(size_y,false));

    for(int side = 0; side < 2; ++side)
        for(int agent = 0; agent < 2; ++agent){

            std::pair<int,int> agent_pos = getAgent(side, agent);

            field_data.at(agent_pos.first).at(agent_pos.second) = side + 1;
        }
}

procon::Field::Field(const unsigned int size_x, const unsigned int size_y, const std::vector<std::vector<int>>& input_val){
    grid_x = size_x;
    grid_y = size_y;


    agents = { { std::make_pair(0, 0), std::make_pair(size_x - 1, size_y - 1) }, { std::make_pair(size_x - 1, 0), std::make_pair(0, size_y - 1) } };

    field_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));
    value_data = input_val;

    region_blue = std::vector<std::vector<bool>>(size_x, std::vector<bool>(size_y,false));
    region_red = std::vector<std::vector<bool>>(size_x, std::vector<bool>(size_y,false));


    for(int side = 0; side < 2; ++side)
        for(int agent = 0; agent < 2; ++agent){

            std::pair<int,int> agent_pos = getAgent(side, agent);

            field_data.at(agent_pos.first).at(agent_pos.second) = side + 1;
        }
}

//ここサイズ対応します
procon::Field::Field(const unsigned int size_x, const unsigned int size_y, const int max_val, const int min_val){
    std::random_device rnd;
    std::mt19937 mt (rnd());

    std::uniform_int_distribution<> rndor(0,1);//[0,1]
    std::uniform_int_distribution<> rndminus(0,9);
    std::lognormal_distribution<> dist(3.0,0.25);
    std::chi_squared_distribution<> dist2(3.0);

    grid_x = size_x;
    grid_y = size_y;

    while(grid_x * grid_y < 80){

        if(grid_x==12)++grid_y;
        else if(grid_y==12)++grid_x;
        else ++(rndor(mt)?grid_x:grid_y);
    }


    agents = { { std::make_pair(0, 0), std::make_pair(grid_x - 1, grid_y - 1) }, { std::make_pair(grid_x - 1, 0), std::make_pair(0, grid_y - 1) } };

    field_data = std::vector<std::vector<int>>(grid_x, std::vector<int>(grid_y, 0 ));
    value_data = std::vector<std::vector<int>>(grid_x, std::vector<int>(grid_y, 0 ));

    region_blue = std::vector<std::vector<bool>>(size_x, std::vector<bool>(size_y,false));
    region_red = std::vector<std::vector<bool>>(size_x, std::vector<bool>(size_y,false));

    /*
    std::uniform_int_distribution<> plus_rnd(0,max_val);
    std::uniform_int_distribution<> minus_rnd(min_val,-1);
    //std::uniform_real_distribution<> double_rnd(0.0,1.0);
    */
    std::uniform_int_distribution<> plus_rnd(0,max_val / 3);



    //ここは「x軸かつy軸方向に垂直」で解釈します←するなよ！！！！！運営を許すな

    std::uniform_int_distribution<> rndtri(0,4);

    //面倒なのでコピペでクソコードを書きます 運営を許すな

    int val = rndtri(mt);


    if(!val){
        for(unsigned int x = 0; x < grid_x / 2 + 1; ++x){
            for(unsigned int y = 0; y < grid_y / 2 + 1; ++y){

                int value = std::min(static_cast<int>(dist(mt)) - 16, max_val - plus_rnd(mt));
                value = std::max(min_val, value);

                value = (rndminus(mt) ? std::abs(value) : -1 * std::abs(value) );


                value_data.at(x).at(y) = value;
                value_data.at(grid_x - x - 1).at(grid_y - y - 1) = value;
                value_data.at(x).at(grid_y - y - 1) = value;
                value_data.at(grid_x - x - 1).at(y) = value;
            }
        }

        std::uniform_int_distribution<> random_x(0,(grid_x - 1) / 2);
        std::uniform_int_distribution<> random_y(0,(grid_y - 1) / 2);

        int agent_x = random_x(mt);
        int agent_y = random_y(mt);
        agents.at(0).at(0) = std::make_pair(agent_x, agent_y);
        agents.at(0).at(1) = std::make_pair(grid_x - agent_x - 1, grid_y - agent_y - 1);
        agents.at(1).at(0) = std::make_pair(agent_x, grid_y - agent_y - 1);
        agents.at(1).at(1) = std::make_pair(grid_x - agent_x - 1, agent_y);

    }else if(val==1||val==2){
        for(unsigned int x = 0; x < grid_x; ++x){
            for(unsigned int y = 0; y < grid_y / 2 + 1; ++y){

                int value = std::min(static_cast<int>(dist(mt)) - 16, max_val - plus_rnd(mt));
                value = std::max(min_val, value);

                value_data.at(x).at(y) = value;
                value_data.at(x).at(grid_y - y - 1) = value;
            }
        }

        std::uniform_int_distribution<> random_x(0,grid_x - 1);
        std::uniform_int_distribution<> random_y(0,(grid_y - 1) / 2);

        int agent_x = random_x(mt);
        int agent_y = random_y(mt);
        int agent_x_dif = agent_x;
        agents.at(0).at(0) = std::make_pair(agent_x, agent_y);
        agents.at(1).at(0) = std::make_pair(agent_x, grid_y - agent_y - 1);

        while(agent_x == agent_x_dif)
            agent_x = random_x(mt);

        agent_y = random_y(mt);

        agents.at(0).at(1) = std::make_pair(agent_x, grid_y - agent_y - 1);
        agents.at(1).at(1) = std::make_pair(agent_x, agent_y);

    }else{
        for(unsigned int x = 0; x < grid_x / 2 + 1; ++x){
            for(unsigned int y = 0; y < grid_y; ++y){

                int value = std::min(static_cast<int>(dist(mt)) - 16, max_val - plus_rnd(mt));
                value = std::max(min_val, value);

                value_data.at(x).at(y) = value;
                value_data.at(grid_x - x - 1).at(y) = value;
            }
        }

        std::uniform_int_distribution<> random_x(0,(grid_x - 1) / 2);
        std::uniform_int_distribution<> random_y(0,grid_y - 1);

        int agent_x = random_x(mt);
        int agent_y = random_y(mt);
        int agent_y_dif = agent_y;
        agents.at(0).at(0) = std::make_pair(grid_x - agent_x - 1, agent_y);
        agents.at(1).at(0) = std::make_pair(agent_x, agent_y);

        while(agent_y == agent_y_dif)
            agent_y = random_y(mt);

        agent_x = random_x(mt);

        agents.at(0).at(1) = std::make_pair(agent_x, agent_y);
        agents.at(1).at(1) = std::make_pair(grid_x - agent_x - 1, agent_y);
    }




    for(int side = 0; side < 2; ++side)
        for(int agent = 0; agent < 2; ++agent){

            std::pair<int,int> agent_pos = getAgent(side, agent);

            field_data.at(agent_pos.first).at(agent_pos.second) = side + 1;
        }
}

const std::vector<std::vector<int>>& procon::Field::getField() const{
    return field_data;
}

const std::vector<std::vector<int>>& procon::Field::getValue() const{
    return value_data;
}

std::pair<int,int> procon::Field::getSize() const{
    return std::make_pair(grid_x, grid_y);
}


const std::vector<std::vector<std::pair<int,int>>>& procon::Field::getAgents() const{
    return agents;
}

std::pair<int,int> procon::Field::getAgent(const unsigned int turn, const unsigned int number) const{
    return agents.at(turn).at(number);
}

bool procon::Field::isPlaced(const unsigned int x, const unsigned int y){
    return (field_data.at(x).at(y) != 0);
}

//pair<タイル状況,評価値>を返す
std::pair<int,int> procon::Field::getState(const unsigned int x, const unsigned int y) const{
    return std::make_pair(field_data.at(x).at(y), value_data.at(x).at(y));
}

void procon::Field::setState(const unsigned int x, const unsigned int y, const unsigned int state){
    field_data.at(x).at(y) = state;
}

void procon::Field::setAgent(const unsigned int turn, const unsigned int number, const unsigned int x_pos, const unsigned int y_pos){
    agents.at(turn).at(number) = std::make_pair(x_pos, y_pos);
}


void procon::Field::setSize(const std::pair<int, int> &grid){
    grid_x = grid.first; grid_y = grid.second;
}

void procon::Field::setValue(const std::vector<std::vector<int>> &value){
    value_data = value;
}

void procon::Field::setAgents(const std::vector<std::vector<std::pair<int,int>>>& values){
    agents = values;
}

void procon::Field::setStates(const std::vector<std::vector<int>>& values){
    field_data = values;
}

void procon::Field::updatePoint(){
    int LookUpTable[400],flag[400];
    for(int a = 1;a < 400;a++){
        LookUpTable[a]=a;
        flag[a]=0;
    }
    int ins = 1;
    int dx[] = {-1, 0};
    int dy[] = {0, 1};
    std::vector<std::vector<bool>> mass = std::vector<std::vector<bool>>(grid_x, std::vector<bool>(grid_y, false));
    std::vector<std::vector<int>> labeling = std::vector<std::vector<int>>(grid_x, std::vector<int>(grid_y, 0));
    auto calc =[&](int x,int y,int side){
        if(field_data.at(x).at(y)==side)return;
        std::set<int> se;
        int inst = 1000000;
        for(int index = 0;index < 2;index++){
            if(0 <= x + dx[index] && x + dx[index] < grid_x && 0 <= y + dy[index] && y + dy[index] < grid_y){
                if(field_data.at(x + dx[index]).at(y + dy[index]) != side){
                    if(labeling.at(x + dx[index]).at(y + dy[index]) != 0){
                        inst = std::min(inst,labeling.at(x + dx[index]).at(y + dy[index]));
                        se.insert(labeling.at(x + dx[index]).at(y + dy[index]));
                    }
                }
            }
        }
        if(se.empty()){
            labeling.at(x).at(y)=ins;
            //std::cout<<ins<<std::endl;
            ins++;
        }else{
            labeling.at(x).at(y)=inst;
            //std::cout<<inst<<std::endl;
            for(int s:se){
                LookUpTable[s]=inst;
            }
        }
    };
    for(int a = 0;a < grid_x;a++){
        for(int b = 0;b < grid_y;b++){
            calc(a,b,1);
        }
    }
    for(int a = 0;a < grid_x;a++){
        for(int b = 0;b < grid_y;b++){
            labeling.at(a).at(b)=LookUpTable[labeling.at(a).at(b)];
        }
    }
    for(int a = 0;a < grid_x;a++){
        for(int b = 0;b < grid_y;b++){
            if(a == 0 || a == grid_x-1 || b == 0 || b == grid_y-1){
                if(labeling.at(a).at(b)!=0){
              //  flag[labeling.at(a).at(b)]=1;
                }
            }
        }
    }
    std::cout<<std::endl;
    for(int a = 0;a < grid_x;a++){
        for(int b = 0;b < grid_y;b++){
            std::cout<<labeling.at(a).at(b)<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    for(int a = 0;a < grid_x;a++){
        for(int b = 0;b < grid_y;b++){
            if(labeling.at(a).at(b)!=0){
                if(flag[labeling.at(a).at(b)]==0){
                    mass.at(a).at(b)=true;
                }
            }
        }
    }
    region_red = mass;

    int region_red_point = 0;//赤領域
    int region_blue_point = 0;//青領域


    int common_red_point = 0;//赤マスポイント
    int common_blue_point = 0;//青マスポイント

    //それぞれのマスにゴニョゴニョしてます(読めばわかる)
    for(int a = 0;a < grid_x; a++){
        for(int b = 0;b < grid_y; b++){

            if(field_data.at(a).at(b) == 1)
                common_red_point += value_data.at(a).at(b);
            if(field_data.at(a).at(b) == 2)
                common_blue_point += value_data.at(a).at(b);

            if(region_red.at(a).at(b))
                region_red_point += std::abs(value_data.at(a).at(b));

            if(region_blue.at(a).at(b))
                 region_blue_point += std::abs(value_data.at(a).at(b));

        }
    }

    red_point = std::make_pair(common_red_point, region_red_point);//メンバに代入
    blue_point = std::make_pair(common_blue_point, region_blue_point);//同上
}

std::pair<int,int> procon::Field::getPoints(int side, bool update_flag){

    if(update_flag)
        updatePoint();

    return (side == 0 ? red_point : blue_point);
}
