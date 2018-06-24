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
    /*ラベリングを用いています、それが何か気になったらはむへいか会長に聞いてみて
     */
    const int INF = 1e9;
    auto calc = [&](int side){
        int dx[] = {-1, 0};
        int dy[] = {0, -1};
        std::vector<std::vector<bool>> mass = std::vector<std::vector<bool>>(grid_x, std::vector<bool>(grid_y, false)); //最終的にメンバに渡す変数
        std::vector<std::vector<int>> labeling = std::vector<std::vector<int>>(grid_x, std::vector<int>(grid_y, 0)); //ラベリング本体
        std::vector<bool> flag = std::vector<bool>(200, true); //その島が外側の枠と接しているか
        int LookUpTable[200]; //ルックアップテーブル(ラベリングに必要なにか)
        for(int a = 0;a < 200;a++){
            LookUpTable[a]=a;
        }
        int now_index = 1;
        for(int y = 0;y < grid_y;y++){
            for(int x = 0;x < grid_x;x++){
                if(field_data.at(x).at(y)==side)continue;
                std::set<int> _set;
                int ins_min = INF;
                for(int index = 0;index < 2;index++){
                    if(0 <= x + dx[index] && x + dx[index] < grid_x && 0 <= y + dy[index] && y + dy[index] < grid_y){
                        if(labeling.at(x + dx[index]).at(y + dy[index]) != 0 && field_data.at(x).at(y) != side ){
                            ins_min = std::min(labeling.at(x + dx[index]).at(y + dy[index]),ins_min);
                            _set.insert(labeling.at(x + dx[index]).at(y + dy[index]));
                        }
                    }
                }
                if(_set.empty()){
                    labeling.at(x).at(y)=now_index;
                    now_index++;
                }else{
                    labeling.at(x).at(y)=ins_min;
                    for(int s:_set){
                        LookUpTable[s]=ins_min;
                    }
                }
                //std::cout<<ins_min<<std::endl;
            }
        }
        std::vector<int> pos_x = {1,0,-1,0};
        std::vector<int> pos_y = {0,1,0,-1};
        for(int x = 0;x < grid_x;x++){
            for(int y = 0;y < grid_y;y++){
                if(labeling.at(x).at(y)!=0){
                    for(int index = 0;index < 4;index++){
                        if(0 <= x + pos_x[index]&&x + pos_x[index] < grid_x && 0 <= y + pos_y[index] && y + pos_y[index] < grid_y){
                            if(labeling.at(x+pos_x[index]).at(y+pos_y[index])!=0){
                                LookUpTable[labeling.at(x).at(y)]=std::min(LookUpTable[labeling.at(x).at(y)],labeling.at(x+pos_x[index]).at(y+pos_y[index]));
                            }
                        }
                    }
                }
            }
        }
        for(int index = now_index+1; 0 <= index; index--){
            if(LookUpTable[index] == index)continue;

            for(int x = 0;x < grid_x;x++){
                for(int y = 0;y < grid_y;y++){
                    if(labeling.at(x).at(y)==index){
                        labeling.at(x).at(y)=LookUpTable[index];
                    }
                }
            }

        }
        for(int x = 0;x < grid_x;x++){
            flag[labeling.at(x).at(0)]=false;
            flag[labeling.at(x).at(grid_y-1)]=false;
        }
        for(int y = 0;y < grid_y;y++){
            flag[labeling.at(0).at(y)]=false;
            flag[labeling.at(grid_x-1).at(y)]=false;
        }
        for(int x = 0; x < grid_x; x++){
            for(int y = 0; y < grid_y; y++){
                if(labeling.at(x).at(y) != 0 && flag[labeling.at(x).at(y)]){
                    mass.at(x).at(y) = true;
                }
            }
        }
        /*
        std::cout<<std::endl;
        for(int x = 0;x < grid_y;x++){
            for(int y = 0;y < grid_x;y++){
                std::cout<<labeling.at(y).at(x)<<" ";
            }
            std::cout<<std::endl;
        }
        */

       (side == 1 ? region_red = mass: region_blue = mass);
    };

    calc(1);
    calc(2);

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

    /*
    std::cout << region_red_point << std::endl;
    std::cout << region_blue_point << std::endl;
    */

    red_point = std::make_pair(common_red_point, region_red_point);//メンバに代入
    blue_point = std::make_pair(common_blue_point, region_blue_point);//同上
}

std::pair<int,int> procon::Field::getPoints(int side, bool flag){
    if(flag){
        updatePoint();
    }
    return (side == 0 ? red_point : blue_point);
}

std::pair<int,int> procon::Field::getPoints(int side, std::pair<int, std::pair<int,int>> pos, bool flag){
    int dx[8]={1, 1, 1, 0, -1, -1, -1, 0};
    int dy[8]={1, 0, -1, -1, -1, 0, 1, 1};
    bool result = false;
    for(int index = 0;index < 8;index++){
        if(value_data.at(pos.second.first + dx[index]).at(pos.second.second + dy[index]) == pos.first - 1){
            result = true;
        }
    }
    if(result && flag){
        updatePoint();
    }
    return (side == 0 ? red_point : blue_point);
}

std::pair<int,int> procon::Field::getPoints(int side, std::vector<std::pair<int, std::pair<int, int> > > pos_vec, bool flag){
    int dx[8]={1, 1, 1, 0, -1, -1, -1, 0};
    int dy[8]={1, 0, -1, -1, -1, 0, 1, 1};
    bool result = false;
    for(std::pair<int, std::pair<int,int>> pos : pos_vec){
        for(int index = 0;index < 8;index++){
            if(value_data.at(pos.second.first + dx[index]).at(pos.second.second + dy[index]) == pos.first - 1){
                result = true;
            }
        }
    }
    if(result && flag){
        updatePoint();
    }
    return (side == 0 ? red_point :blue_point);
}

void procon::Field::setPoints(int side, std::pair<int, int> value){
    (side == 0 ? red_point : blue_point) = value;
}
std::vector<std::vector<bool>> procon::Field::getRegion(int side){
    return ( side == 0 ? region_red : region_blue);
}
