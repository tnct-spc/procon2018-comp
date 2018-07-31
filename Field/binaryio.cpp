#include "binaryio.h"
#define bit char,std::char_traits<char>,std::allocator<char>

void procon::BinaryIo::exportField(procon::Field& field,std::string Path){
    /* 全ての表したい数を数を適正な桁数のbitで表現して結合します。
     * それを書き込みます
     *
     * まず横幅のサイズと縦幅のサイズ(それぞれ4桁のbitで表現)
     * 次にそれぞれのマスの状態(2bit)と得点(6bit)(0,0)(0,1)...(0,grid_y-1)(1,0)(1,1)...(1,grid_y-1)(2,0).......(grid_x-1,grid_y-1)の順番(伝われ)
     * 次に現在のターン数と最終ターン数(それぞれ8桁のbitで表現)
     * 最後にagentの位置を格納 team0のagent0x座標,y座標 同様にteam0のagent1,team1のagent0,team1のagent1も(それぞれ4桁)
     *
     */
    std::pair<int,int> large = field.getSize();
    std::string ans;
    ans += std::bitset<4>(large.first).to_string<bit>();
    ans += std::bitset<4>(large.second).to_string<bit>();
    for(int x = 0;x < large.first; x++){
        for(int y = 0;y < large.second; y++){
            ans += std::bitset<2>(field.getState(x,y).first).to_string<bit>() + std::bitset<6>(field.getState(x,y).second+16).to_string<bit>();
        }
    }
    ans += std::bitset<8>(field.getTurnCount()).to_string<bit>();
    ans += std::bitset<8>(field.getFinalTurn()).to_string<bit>();

    for(int turn = 0; turn < 2 ;turn++){
        for(int index = 0; index < 2; index++){
            ans += std::bitset<4>(field.getAgent(turn, index).first).to_string<bit>();
            ans += std::bitset<4>(field.getAgent(turn, index).second).to_string<bit>();
        }
    }
    ans += std::bitset<12>(field.getPoints(false).at(0).first).to_string<bit>();
    ans += std::bitset<12>(field.getPoints(false).at(0).second).to_string<bit>();
    ans += std::bitset<12>(field.getPoints(false).at(1).first).to_string<bit>();
    ans += std::bitset<12>(field.getPoints(false).at(1).second).to_string<bit>();

    std::string str;
    std::ofstream ofs;
    ofs.open(Path, std::ios::binary);

    if (!ofs) {
        std::cout << Path + "に書き込めません"<<std::endl;
        return;
    }
    for(long unsigned int i = 0; i < ans.size() ; i++){
        str.push_back(ans.at(i));
        if(str.size() == 8){
             char w = std::stoi(str, nullptr, 2);
        //    std::cout<<w<<" ";
            ofs.write((char * ) &w, sizeof (char));
            str.clear();
        }
    }
    //if(str.empty())return;
    //std::string y;
    //for(int i = str.size(); i < 4;i++){
      //  y += '0';
    //}
    //str = y + str;
    // std::cout<<"SIZE"<<str<<std::endl;
    //int w = std::stoi( y + str, nullptr, 2);
    //ofs.write((char * )&w, sizeof (int));
    std::cout<<ans.size()<<std::endl;

   // std::cout<<"binary_size:"<<ans.size()<<std::endl;
}

procon::Field procon::BinaryIo::importField(std::string Path){
    std::ifstream fin(Path, std::ios::binary);

    if (!fin){
        std::cout << "ファイル " + Path + " が開けません"<<std::endl;
    }
    char ins = 0;

    std::string ans, str;

    while(!fin.eof()){
        fin.read( (char * ) &ins, sizeof (char));
        ans += std::bitset<8>(ins).to_string<bit>();
     //   std::cout<<ins<<" ";
    }
    std::cout<<ans.size()<<std::endl;
   // std::cout<<std::endl;
   // std::cout<<"binary_size:"<<ans.size()<<std::endl;


    std::queue<char> que;

    for(long unsigned int i = 0 ; i < ans.size() ; i++){
        que.push(ans.at(i));
    }

    for(int i = 0;i < 4 ; i++){
        str.push_back(que.front());
        que.pop();
    }
    std::pair<int,int> grid_size;
    grid_size.first = std::bitset<4>(str).to_ulong();
    str.clear();

    for(int i = 0 ; i < 4 ; i++){
        str.push_back(que.front());
        que.pop();
    }

    grid_size.second = std::bitset<4>(str).to_ulong();
    str.clear();
    Field field(grid_size.first, grid_size.second);

    std::vector<std::vector<int>> values;

    for(int x = 0 ; x < grid_size.first ; x++){
        std::vector<int> vec;
        for(int y = 0 ; y < grid_size.second ; y++){
            for(int i = 0;i < 2;i++){
                str.push_back(que.front());
                que.pop();
            }
            field.setState(x, y, std::bitset<2>(str).to_ulong());
            str.clear();
            for(int i = 0; i < 6 ; i++){
                str.push_back(que.front());
                que.pop();
            }
            vec.push_back(std::bitset<6>(str).to_ulong()-16);
            str.clear();
        }
        values.push_back(vec);
    }
    for(int i = 0 ; i < 8 ; i++){
        str.push_back(que.front());
        que.pop();
    }
    field.setTurnCount(std::bitset<8>(str).to_ulong());
    str.clear();
    for(int i = 0 ; i < 8 ; i++){
        str.push_back(que.front());
        que.pop();
    }

    field.setFinalTurn(std::bitset<8>(str).to_ulong());
    str.clear();
    for(int turn = 0; turn < 2; turn++){
    for(int index = 0 ; index < 2 ; index++){
       // std::cout<<que.size()<<std::endl;
        for(int i = 0; i < 4; i++){
            str.push_back(que.front());
            que.pop();
        }
        int a_x = std::bitset<4>(str).to_ulong();
        str.clear();
        for(int i = 0 ; i < 4 ; i++){
            str.push_back(que.front());
            que.pop();
        }
        int a_y = std::bitset<4>(str).to_ulong();
        str.clear();
        field.setAgent(turn, index, a_x, a_y);
        }
    }
    for(int i = 0; i < 12;i++){
        str.push_back(que.front());
        que.pop();
    }
    int hoge = std::bitset<12>(str).to_ulong();
    str.clear();
    for(int i = 0;i < 12;i++){
        str.push_back(que.front());
        que.pop();
    }
    int huge = std::bitset<12>(str).to_ulong();
    str.clear();
    field.setPoints(0, std::make_pair(hoge,huge));

    for(int i = 0; i < 12;i++){
        str.push_back(que.front());
        que.pop();
    }
    hoge = std::bitset<12>(str).to_ulong();
    str.clear();
    for(int i = 0;i < 12;i++){
        str.push_back(que.front());
        que.pop();
    }
    huge = std::bitset<12>(str).to_ulong();
    str.clear();
    field.setPoints(1, std::make_pair(hoge,huge));
    fin.close();
    return field;
}
std::string procon::BinaryIo::exportToString(procon::Field& field){
    std::pair<int,int> large = field.getSize();
    std::string ans;
    ans += std::bitset<4>(large.first).to_string<bit>();
    ans += std::bitset<4>(large.second).to_string<bit>();
    for(int x = 0;x < large.first; x++){
        for(int y = 0;y < large.second; y++){
            ans += std::bitset<2>(field.getState(x,y).first).to_string<bit>() + std::bitset<6>(field.getState(x,y).second+16).to_string<bit>();
        }
    }
    ans += std::bitset<8>(field.getTurnCount()).to_string<bit>();
    ans += std::bitset<8>(field.getFinalTurn()).to_string<bit>();

    for(int turn = 0; turn < 2 ;turn++){
        for(int index = 0; index < 2; index++){
            ans += std::bitset<4>(field.getAgent(turn, index).first).to_string<bit>();
            ans += std::bitset<4>(field.getAgent(turn, index).second).to_string<bit>();
        }
    }

    const std::vector<std::pair<int,int>>& points = field.getPoints(false);
    for(int side = 0; side < 2; ++side){
        ans += std::bitset<12>(points.at(side).first).to_string<bit>();
        ans += std::bitset<12>(points.at(side).second).to_string<bit>();
    }


    return ans;
}
