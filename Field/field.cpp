#include "field.h"

procon::Field::Field(const unsigned int size_x ,const unsigned int size_y){
    grid_x = size_x;
    grid_y = size_y;

    //初期化処理がめっちゃ分かりづらいですが、四隅にagentを配置してます
    agents = { { std::make_pair(0, 0), std::make_pair(size_x - 1, size_y - 1) }, { std::make_pair(size_x - 1, 0), std::make_pair(0, size_y - 1) } };

    field_data = std::bitset<288>(0uL);
    value_data = std::vector<std::vector<int>>(size_x, std::vector<int>(size_y,0));

    regions = std::bitset<288>(0uL);

    points = std::vector<std::pair<int,int>>(2, std::make_pair(0,0));

    for(int side = 0; side < 2; ++side)
        for(int agent = 0; agent < 2; ++agent){

            std::pair<int,int> agent_pos = getAgent(side, agent);

            std::bitset<288> w = side + 1;

            field_data |= (w << (2*(12*agent_pos.second + agent_pos.first)));
        }
    feature = std::vector<double>(10);
    updateFeature();
}

procon::Field::Field(const unsigned int size_x, const unsigned int size_y, const std::vector<std::vector<int>>& input_val){
    grid_x = size_x;
    grid_y = size_y;


    agents = { { std::make_pair(0, 0), std::make_pair(size_x - 1, size_y - 1) }, { std::make_pair(size_x - 1, 0), std::make_pair(0, size_y - 1) } };


    field_data = std::bitset<288>(0uL);
    value_data = input_val;

    regions = std::bitset<288>(0uL);
    points = std::vector<std::pair<int,int>>(2, std::make_pair(0,0));

    for(int side = 0; side < 2; ++side)
        for(int agent = 0; agent < 2; ++agent){

            std::pair<int,int> agent_pos = getAgent(side, agent);

            std::bitset<288> w = side + 1;

            field_data |= (w << ((2*(12*agent_pos.second+agent_pos.first))));
        }
    feature = std::vector<double>(10);
    updateFeature();
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

    field_data = std::bitset<288>(0uL);
    value_data = std::vector<std::vector<int>>(grid_x, std::vector<int>(grid_y, 0 ));



    regions = std::bitset<288>(0uL);
    points = std::vector<std::pair<int,int>>(2, std::make_pair(0,0));

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

            std::bitset<288> w = side+1;

            field_data |= (w << (2*(12*agent_pos.second+agent_pos.first)));
        }
    feature = std::vector<double>(10);
    updateFeature();
}

int procon::Field::getTurnCount(){
    return now_turn;
}

int procon::Field::getFinalTurn(){
    return final_turn;
}

void procon::Field::setTurnCount(int turn_count){
    now_turn = turn_count;
}

void procon::Field::setFinalTurn(int turn_count){
    final_turn = turn_count;
}


const std::vector<std::vector<int>>& procon::Field::getValue() const{
    return value_data;
}

std::pair<int,int> procon::Field::getSize() const{
    return std::make_pair(grid_x, grid_y);
}

std::bitset<288>& procon::Field::getField(){
    return field_data;
}

void procon::Field::setField(std::bitset<288>& input){
    field_data = input;
}

const std::vector<std::vector<std::pair<int,int>>>& procon::Field::getAgents() const{
    return agents;
}

std::pair<int,int> procon::Field::getAgent(const unsigned int side, const unsigned int number) const{
    return agents.at(side).at(number);
}


//pair<タイル状況,評価値>を返す
std::pair<int,int> procon::Field::getState(const unsigned int x, const unsigned int y) const{
    std::bitset<288> w(0uL);
    w |= field_data >> (2*(12*y+x));
    w &= 3;
    return std::make_pair(w.to_ulong(),value_data.at(x).at(y));
}

void procon::Field::setState(const unsigned int x, const unsigned int y, const unsigned int state){
    std::bitset<288> w = 3;
    field_data &= ~( w << (2*(12*y+x)));
    w = state;
    field_data |= ( w << (2*(12*y+x)));
}

void procon::Field::resetState(const procon::Field& inp_field){
    field_data = inp_field.field_data;
    agents = inp_field.agents;
    points = inp_field.points;
    regions = inp_field.regions;
}

void procon::Field::setAgent(const unsigned int side, const unsigned int number, const unsigned int x_pos, const unsigned int y_pos){
    agents.at(side).at(number) = std::make_pair(x_pos, y_pos);
}


bool procon::Field::canPut(const unsigned int side, const unsigned int move_1, const unsigned int move_2, bool double_move) const{

    std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    auto check_outofrange = [&](int agent){

        std::pair<int,int> agent_pos = getAgent(side, agent);

        int move = (agent == 0 ? move_1 : move_2);

        agent_pos.first += x_list.at(move);
        agent_pos.second += y_list.at(move);


        return !(agent_pos.first < 0 || agent_pos.second < 0 || agent_pos.first >= getSize().first || agent_pos.second >= getSize().second);
    };

    auto check_conflict = [&]{

        std::pair<int,int> agent_pos_1 = getAgent(side, 0);

        if(getState(agent_pos_1.first + x_list.at(move_1), agent_pos_1.second + y_list.at(move_1) ).first != (side == 0 ? 2 : 1) ){

            agent_pos_1.first += x_list.at(move_1);
            agent_pos_1.second += y_list.at(move_1);
        }

        std::pair<int,int> agent_pos_2 = getAgent(side, 1);

        if(getState(agent_pos_2.first + x_list.at(move_2), agent_pos_2.second + y_list.at(move_2) ).first != (side == 0 ? 2 : 1) ){

            agent_pos_2.first += x_list.at(move_2);
            agent_pos_2.second += y_list.at(move_2);
        }

        return (agent_pos_1 != agent_pos_2);
    };

    //クソ実装を許せ
    if(double_move == false){

        std::pair<int,int> agent_pos = getAgent(side, move_1);

        agent_pos.first += x_list.at(move_2);
        agent_pos.second += y_list.at(move_2);


        return !(agent_pos.first < 0 || agent_pos.second < 0 || agent_pos.first >= getSize().first || agent_pos.second >= getSize().second);

    }

    return ( check_outofrange(0) && check_outofrange(1) && check_conflict());
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
    field_data = std::bitset<288>(0uL);
    std::bitset<288> w = std::bitset<288>(0uL);
    for(int a = 0;a < grid_x;a++){
        for(int b = 0;b < grid_y;b++){
            w = values.at(a).at(b);
            field_data |= ( w << (2*(12*b+a)));
        }
    }
}
namespace procon {


class UnionFind{
private:
    std::vector<int> par;
    std::vector<int> rank;
public:
    void init(int N){
        par.resize(N);
        rank.resize(N);
        for(int a = 0;a < N;a++)
        {
            par[a]=a;
            rank[a]=0;
        }
    }
    int root(int a)
    {
        return par[a] == a ? a : par[a] = root(par[a]);
    }
    void connect(int a,int b)
    {
        a = root(a);
        b = root(b);
        if(a==b)return;

        if(rank[a] < rank[b]){
            par[a] = b;
        }else{
            par[b] = a;
            if(rank[a] == rank[b])rank[a]++;
        }
    }
    bool check(int a,int b){
        a = root(a);
        b = root(b);
        if(a == b)return true;
        else return false;
    }
};
}
void procon::Field::updatePoint(){
    /*ラベリングを用いています、それが何か気になったらはむへいか会長に聞いてみて
     */
    const int INF = 1e9;
    regions = std::bitset<288>(0uL);
    auto calc = [&](int side){
        int dx[] = {-1, 0};
        int dy[] = {0, -1};
        std::bitset<288> mass = std::bitset<288>(0uL); //最終的にメンバに渡す変数
        std::vector<std::vector<int>> labeling = std::vector<std::vector<int>>(grid_x, std::vector<int>(grid_y, 0)); //ラベリング本体
        std::vector<bool> flag = std::vector<bool>(200, true); //その島が外側の枠と接しているか
        procon::UnionFind LookUpTable;
        LookUpTable.init(50);
        int now_index = 1;
        for(int y = 0;y < grid_y;y++){
            for(int x = 0;x < grid_x;x++){
                if(getState(x, y).first==side)continue;
                std::set<int> _set;
                int ins_min = INF;
                for(int index = 0;index < 2;index++){
                    if(0 <= x + dx[index] && x + dx[index] < grid_x && 0 <= y + dy[index] && y + dy[index] < grid_y){
                        if(labeling.at(x + dx[index]).at(y + dy[index]) != 0 && getState(x, y).first != side ){
                            ins_min = std::min(labeling.at(x + dx[index]).at(y + dy[index]), ins_min);
                            _set.insert(labeling.at(x + dx[index]).at(y + dy[index]));
                        }
                    }
                }
                if(_set.empty()){
                    labeling.at(x).at( y ) = now_index;
                    now_index++;
                }else{
                    labeling.at(x).at( y ) = ins_min;
                    for(int s:_set){
                        LookUpTable.connect(s, ins_min);
                    }
                }
                //std::cout<<ins_min<<std::endl;
            }
        }
        /*
        std::vector<int> pos_x = {1,0,-1,0};
        std::vector<int> pos_y = {0,1,0,-1};
        for(int x = 0;x < grid_x;x++){
            for(int y = 0;y < grid_y;y++){
                if(labeling.at(x).at(y)!=0){
                    for(int index = 0;index < 4;index++){
                        if(0 <= x + pos_x[index]&&x + pos_x[index] < grid_x && 0 <= y + pos_y[index] && y + pos_y[index] < grid_y){
                            if(labeling.at(x+pos_x[index]).at(y+pos_y[index]) != 0){
                                LookUpTable[labeling.at(x).at(y)]=std::min(LookUpTable[labeling.at(x).at(y)],labeling.at(x+pos_x[index]).at(y+pos_y[index]));
                            }
                        }
                    }
                }
            }
        }
        */
        for(int x = 0;x < grid_x;x++){
            for(int y = 0;y < grid_y;y++){
                labeling.at(x).at(y)=LookUpTable.root(labeling.at(x).at(y));
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
                    mass[(y*12+x)] = true;
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

       if(side == 1)regions |= mass;
       if(side == 2)regions |= mass << 144;
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

            if(getState(a, b).first == 1)
                common_red_point += value_data.at(a).at(b);
            if(getState(a, b).first == 2)
                common_blue_point += value_data.at(a).at(b);

            if(regions[b*12 + a])
                region_red_point += std::abs(value_data.at(a).at(b));

            if(regions[b * 12 + a + 144])
                 region_blue_point += std::abs(value_data.at(a).at(b));
        }
    }

    /*
    std::cout << region_red_point << std::endl;
    std::cout << region_blue_point << std::endl;
    */

    points.at(0) = std::make_pair(common_red_point, region_red_point);//メンバに代入
    points.at(1) = std::make_pair(common_blue_point, region_blue_point);//同上
}

std::vector<std::pair<int,int>> procon::Field::getPoints(bool flag){
    if(flag){
        updatePoint();
    }
    return points;
}

std::vector<std::pair<int,int>> procon::Field::getPoints(std::pair<std::pair<int,int>, std::pair<int,int>> pos, bool flag){
    int dx[8]={1, 1, 1, 0, -1, -1, -1, 0};
    int dy[8]={1, 0, -1, -1, -1, 0, 1, 1};
    bool result = false;
    for(int index = 0;index < 8;index++){
        if(!(pos.second.first + dx[index] >= 0 && pos.second.first + dx[index] <= grid_x - 1 && pos.second.second + dy[index] >= 0 && pos.second.second + dy[index] <= grid_y - 1))continue;
        if(getState(pos.second.first + dx[index], pos.second.second + dy[index]).first == pos.first.first + 1){
            if(pos.first.second == 0){
                result = true;
            }
        }else if(getState(pos.second.first + dx[index], pos.second.second + dy[index]).first != 0){
            if(pos.first.second == 1){
                result = true;
            }
        }
    }
    if(!flag && result){
        std::bitset<288> stash_regions = regions;
        std::vector<std::pair<int, int>> stash_points = points;
        updatePoint();
        regions = stash_regions;
        std::vector<std::pair<int, int>> return_points = points;
        points = stash_points;
        return return_points;
    }
    if(flag && result){
        updatePoint();
    }
    return points;
}

std::vector<std::pair<int,int>> procon::Field::getPoints(std::vector<std::pair<std::pair<int,int>, std::pair<int, int> > > pos_vec, bool flag){
    int dx[8] = {1, 1, 1, 0, -1, -1, -1};
    int dy[8] = {1, 0, -1, -1, -1, 0, 1};
    bool result = false;
    for(std::pair<std::pair<int,int>, std::pair<int,int>> pos : pos_vec){
        for(int index = 0;index < 8;index++){
            if(!(pos.second.first + dx[index] >= 0 && pos.second.first + dx[index] <= grid_x - 1 && pos.second.second + dy[index] >= 0 && pos.second.second + dy[index] <= grid_y - 1))continue;
            if(getState(pos.second.first + dx[index], pos.second.second + dy[index]).first == pos.first.first + 1){
                if(pos.first.second == 0){
                    result = true;
                }
            }else if(getState(pos.second.first + dx[index], pos.second.second + dy[index]).first != 0){
                if(pos.first.second == 1){
                    result = true;
                }
            }
        }
    }
    if(!flag && result){
        std::bitset<288> stash_regions = regions;
        std::vector<std::pair<int, int>> stash_points = points;
        updatePoint();
        regions = stash_regions;
        std::vector<std::pair<int, int>> return_points = points;
        points = stash_points;
        return return_points;
    }
    if(flag && result){
        updatePoint();
    }
    return points;
}

void procon::Field::setPoints(int side, std::pair<int, int> value){
    points.at(side) = value;
}
std::bitset<288> procon::Field::getRegion(){
    return regions;
}

void procon::Field::updateFeature(){

    bool result = true; //trueなら縦に対称、falseなら横対称

    for(int x = 0 ; x < grid_x ; x++) {
        for(int y = 0 ; y < grid_y / 2; y++) {
            if( getState(x, y).second != getState(x, grid_y - y - 1).second ) {

                result = false;
                break;

            }
        }
    }
    double sym = result;

    double x_d = 0;
    double y_d = 0;
    for(int x = 1;x < grid_x - 1 ;x++){
        for(int y = 1 ;y < grid_y - 1 ;y++){
            x_d += ((getState(x-1,y-1).second + 2 * getState(x-1,y).second+getState(x-1,y+1).second)-(getState(x+1,y-1).second+2*getState(x+1,y).second+getState(x+1,y+1).second))/5;
            y_d += ((getState(x-1,y-1).second + 2 * getState(x,y-1).second+getState(x+1,y-1).second)-(getState(x-1,y+1).second+2*getState(x,y+1).second+getState(x+1,y-1).second))/5;

        }
    }
    int MA = -100;
    int MI = 100;
    for(int x = 0;x < grid_x;x++){
        for(int y = 0;y < grid_y;y++){
            MA = std::max(MA,getState(x,y).second);
            MI = std::min(MI,getState(x,y).second);
        }
    }
    double UnderGroundOpening = 0;//地下開度
    double AboveGroundOpening = 0;//地上開度っぽく

    for(int x = 0 ; x < grid_x ; x++){
        for(int y = 0 ; y < grid_y ; y++){
            double d_max = -1e9;
            double d_min = +1e9;
            for(int x_ins = 0 ; x_ins < grid_x ; x_ins++){
                for(int y_ins = 0 ; y_ins < grid_y ; y_ins++){
                    if(x == x_ins && y == y_ins)continue;
                    double dis = abs(x-x_ins)*abs(x-x_ins)+abs(y_ins-y)*abs(y_ins-y);
                    dis = std::sqrt(dis);
                    d_max = std::max(d_max, 1.00000*value_data.at(x).at(y) - value_data.at(x_ins).at(y_ins) / dis);
                    d_min = std::min(d_min, 1.00000*value_data.at(x).at(y) - value_data.at(x_ins).at(y_ins) / dis);
                }
            }
            UnderGroundOpening += d_min;
            AboveGroundOpening += d_max;
        }
    }
    AboveGroundOpening /= 1.00000*(grid_x*grid_y);
    UnderGroundOpening /= 1.00000*(grid_x*grid_y);

    std::vector<std::pair<int,int>> age1;

    age1.push_back(std::make_pair(0,1));
    age1.push_back(std::make_pair(0,-1));
    age1.push_back(std::make_pair(1,0));
    age1.push_back(std::make_pair(1,1));
    age1.push_back(std::make_pair(1,-1));
    age1.push_back(std::make_pair(-1,1));
    age1.push_back(std::make_pair(-1,0));
    age1.push_back(std::make_pair(-1,-1));

    double  AverageAltitudeDifference = 0;  //平均高度差
    for(int x = 0;x < grid_x;x++){
        for(int y = 0;y < grid_y;y++){
            int ins = 0;
            double w = 0;
            for(int index = 0;index < 8;index++){
                if(x + age1.at(index).first >= 0&& x + age1.at(index).first <= grid_x-1 && age1.at(index).second+y >= 0 && age1.at(index).second + y <= grid_y-1){
                    ins++;
                    w += getState(x + age1.at(index).first, y + age1.at(index).second).second - getState(x,y).second;
                }
            }
            w /= 1.000000 * ins;
            AverageAltitudeDifference += w;
        }
    }
    AverageAltitudeDifference /= 1.0000*(grid_x*grid_y);

    feature.at(0)  = sym; // 1なら縦に,0なら横に対称
    feature.at(1) = std::sqrt((x_d*x_d)+(y_d*y_d))/(grid_x*grid_y);//傾斜度
    feature.at(2) = AboveGroundOpening; //地上開度(っぽいもの)
    feature.at(3) = UnderGroundOpening; //地下開度(っぽいもの)(負の値を取る)
    feature.at(4) = (AboveGroundOpening+UnderGroundOpening)/2; //尾根谷度
    feature.at(5) = AverageAltitudeDifference;
    feature.at(6) = MA; //最大値
    feature.at(7) = MI; //最小値
    feature.at(8) = grid_x;//xの幅
    feature.at(9) = grid_y;//yの幅
    // std::cout<<feature.at(0)<<" "<<feature.at(1)<<" "<<feature.at(2)<<" "<<feature.at(3)<<" "<<feature.at(4)<<" "<<feature.at(5)<<" "<<feature.at(6)<<" "<<feature.at(7)<<" "<<feature.at(8)<<" "<<feature.at(9)<<std::endl;
}

std::vector<std::pair<int,int>> procon::Field::guessAgents(int side){

   // updateFeature();

    std::vector<std::pair<int,int>> ans_pos;

    if( feature.at(0) ) {

        for(int index = 0 ; index < 2; index++ ){

            ans_pos.push_back(std::make_pair( agents.at( !side ).at(index).first, grid_y - agents.at( !side ).at( index ).second - 1));
        }

    } else {

        for(int index = 0 ; index < 2; index++ ){
            ans_pos.push_back(std::make_pair( grid_x - agents.at( !side ).at(index).first - 1, agents.at( !side ).at( index ).second));
        }

    }
    return ans_pos;
}


double procon::Field::getFeature(int i){
    return feature.at(i-1);
}
const std::vector<double>& procon::Field::getFeatures(){
    return feature;
}

std::vector<double> procon::Field::calcSituationFeature(std::vector<std::pair<std::tuple<int, int, int>,std::tuple<int, int, int>>> agent_pos,int side_of){
    std::vector<double> ans;
    std::vector<std::pair<int,int>> before_point = getPoints();
    std::vector<std::vector<std::tuple<int,int,int>>> act_stack = std::vector<std::vector<std::tuple<int,int,int>>>(2, std::vector<std::tuple<int,int,int>>(2));

    std::bitset<288> ins_field = field_data;
    std::vector<std::vector<std::pair<int, int>>> before_agents = agents;

    auto agent_act  = [&](const int turn, const int agent, const std::tuple<int, int, int> tuple_val){

        int type, x_inp, y_inp;
        std::tie(type, x_inp, y_inp) = tuple_val;


        std::pair<int,int> agent_pos = getAgent(turn, agent);
        std::pair<int,int> grid_size = getSize();
        int x_pos = agent_pos.first + x_inp;
        int y_pos = agent_pos.second + y_inp;

        if(
            type == 0 ||
            x_pos < 0 || x_pos >= grid_size.first ||
            y_pos < 0 || y_pos >= grid_size.second ||
            (type == 1 && getState(x_pos, y_pos).first == (turn==1 ? 1 : 2)) ||
            (type == 2 && getState(x_pos, y_pos).first == 0)
            ){
            act_stack.at(turn).at(agent) = std::make_tuple(1, agent_pos.first, agent_pos.second);
            return ;
        }
        act_stack.at(turn).at(agent) = std::make_tuple(type, x_pos, y_pos);
    };

    auto changeturn = [&](){

        //[(x,y)]:(上書き時の色,(色,エージェント)) わかりづらいね
        std::map<std::pair<int,int>,std::pair<int,std::pair<int,int>>> counts;

        int type, pos_x, pos_y;



        //移動しようとしたエージェントが失敗した時に呼ばれる
        std::function<void(std::pair<int,int>)> delete_move = [&](std::pair<int,int> agent_data){

            std::pair<int,int> not_move = getAgent(agent_data.first, agent_data.second);

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
                    std::pair<int,int> not_move = getAgent(side, agent);

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

            setState(moves.first.first, moves.first.second, moves.second.first);

            if(moves.second.first != 0)
                setAgent(moves.second.second.first, moves.second.second.second, moves.first.first, moves.first.second);
        }
    };
    agent_act(0, 0, agent_pos.at(0).first);
    agent_act(0, 1, agent_pos.at(0).second);
    agent_act(1, 0, agent_pos.at(1).first);
    agent_act(1, 1, agent_pos.at(1).second);
    changeturn();
    updatePoint();
    std::vector<std::pair<int,int>> after_points = getPoints();
    std::vector<std::vector<std::pair<int, int>>> after_agent = agents;
    agents = before_agents;
    field_data = ins_field;
    updatePoint();

    int different_region_point = (after_points.at(side_of).second - after_points.at(!side_of).second)-(before_point.at(side_of).second- before_point.at(!side_of).second);
    ans.push_back(different_region_point);

    int different_simple_point = (after_points.at(side_of).first - after_points.at(!side_of).first)-(before_point.at(side_of).first- before_point.at(!side_of).first);
    ans.push_back(different_simple_point);

    auto calc_distance = [](std::pair<int,int> A, std::pair<int,int> B){
        return std::sqrt(std::abs(A.first - B.first) * std::abs(A.first - B.first) + std::abs(A.second - B.second) * std::abs(A.second - B.second));
    };


    double my_agent_distance_diffrence = calc_distance(after_agent.at(side_of).at(0), after_agent.at(side_of).at(1)) - calc_distance(before_agents.at(side_of).at(0), before_agents.at(side_of).at(1));
    ans.push_back(my_agent_distance_diffrence);

    double opposite_agent_distance_diffrence = calc_distance(after_agent.at(!side_of).at(0), after_agent.at(!side_of).at(1)) - calc_distance(before_agents.at(!side_of).at(0), before_agents.at(!side_of).at(1));
    ans.push_back(opposite_agent_distance_diffrence);

    double opposite_and_my_agent_distance_diffrence = calc_distance(after_agent.at(side_of).at(0), after_agent.at(!side_of).at(0)) + calc_distance(after_agent.at(side_of).at(0), after_agent.at(!side_of).at(1)) + calc_distance(after_agent.at(side_of).at(1), after_agent.at(!side_of).at(0)) + calc_distance(after_agent.at(side_of).at(1), after_agent.at(!side_of).at(1));
    opposite_and_my_agent_distance_diffrence -= calc_distance(before_agents.at(side_of).at(0), before_agents.at(!side_of).at(0)) + calc_distance(before_agents.at(side_of).at(0), before_agents.at(!side_of).at(1)) + calc_distance(before_agents.at(side_of).at(1), before_agents.at(!side_of).at(0)) + calc_distance(before_agents.at(side_of).at(1), before_agents.at(!side_of).at(1));
    ans.push_back(opposite_and_my_agent_distance_diffrence);

    double my_agent_center_distance_diffrence = calc_distance(after_agent.at(side_of).at(0), std::make_pair(grid_x/2,grid_y/2)) + calc_distance(after_agent.at(side_of).at(1), std::make_pair(grid_x/2, grid_y/2));
    my_agent_center_distance_diffrence -= calc_distance(before_agents.at(side_of).at(0), std::make_pair(grid_x/2, grid_y/2)) + calc_distance(before_agents.at(side_of).at(1), std::make_pair(grid_x/2, grid_y/2));
    ans.push_back(my_agent_center_distance_diffrence);


    double opposite_agent_center_disrance_diffrence = calc_distance(after_agent.at(!side_of).at(0), std::make_pair(grid_x/2, grid_y/2)) + calc_distance(after_agent.at(!side_of).at(1), std::make_pair(grid_x / 2, grid_y / 2));
    opposite_agent_center_disrance_diffrence -= calc_distance(before_agents.at(!side_of).at(0), std::make_pair(grid_x / 2, grid_y / 2)) + calc_distance(before_agents.at(!side_of).at(1), std::make_pair(grid_x / 2, grid_y / 2));
    ans.push_back(opposite_agent_center_disrance_diffrence);
    //std::cout<<ans.at(0)<<" "<<ans.at(1)<<" "<<ans.at(2)<<" "<<ans.at(3)<<" "<<ans.at(4)<<" "<<ans.at(5)<<" "<<ans.at(6)<<std::endl;
    //std::cout<<std::endl;
    return ans;
}

