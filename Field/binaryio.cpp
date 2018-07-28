#include "binaryio.h"
#define bit char,std::char_traits<char>,std::allocator<char>

void procon::BinaryIo::exportField(procon::Field& field,std::string Path){
    /* 全ての表したい数を数を適正な桁数のbitで表現して結合します。
     * 最後に32個ずつ前から取ってint型の数字に直して、それを書き込みます。
     *
     * まず横幅のサイズと縦幅のサイズ(それぞれ4桁のbitで表現)
     * 次にそれぞれのマスの得点 (0,0)(0,1)...(0,grid_y-1)(1,0)(1,1)...(1,grid_y-1)(2,0).......(grid_x-1,grid_y-1)の順番(伝われ)(それぞれ6桁のbitで表現予定)
     * 次にそれぞれのマスの状態 (マスの順番は上に同じ)(それぞれ2桁のbitで表現)
     * 次に現在のターン数と最終ターン数(それぞれ7桁のbitで表現)
     *
     */
    std::pair<int,int> large = field.getSize();
    std::string ans;
    ans += std::bitset<4>(large.first).to_string<bit>();
    ans += std::bitset<4>(large.second).to_string<bit>();
    for(int x = 0;x < large.first; x++){
        for(int y = 0;y < large.second; y++){
            ans += std::bitset<6>(field.getState(x,y).second+16).to_string<bit>();
        }
    }
    for(int x = 0; x < large.first ; x++){
        for(int y = 0;y < large.second ; y++){
            ans += std::bitset<2>(field.getState(x,y).first).to_string<bit>();
        }
    }
    ans += std::bitset<7>(field.getTurnCount()).to_string<bit>();
    ans += std::bitset<7>(field.getTurnCount()).to_string<bit>();
    std::string str;
    std::ofstream ofs;
    ofs.open(Path, std::ios::binary);

    if (!ofs) {
            std::cout << Path + "に書き込めません"<<std::endl;;
            return;
        }

    for(int i = 0;i < ans.size() ; i++){
        str.push_back(ans.at(i));
        if(str.size() == 32){
            std::bitset<32> ins(str);
            unsigned int hoge = ins.to_ulong();
            ofs.write((char * )&hoge, sizeof (unsigned int));
            str.clear();
        }
    }
    if(!str.empty()){
        std::bitset<32> ins(str);
        unsigned int hoge = ins.to_ulong();
        ofs.write((char * )&hoge, sizeof (unsigned int));
        str.clear();
    }
    ofs.close();
}

procon::Field procon::BinaryIo::importField(std::string Path){
    std::ifstream fin(Path, std::ios::binary);

    if (!fin){
           std::cout << "ファイル " + Path + " が開けません"<<std::endl;
       }

}
