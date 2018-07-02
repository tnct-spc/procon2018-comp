#include "qrconverterfield.h"

// vector<vector<int>> vec の二次元配列に点数
// vector<int> A (2) , vector<int> B (2) それぞれにエージェントの初期位置

int ctoi(std::string s,int& i) {
    int keta = 0;
    for(int j = 0;true;j++){
        if(s.at(i+j) == '-' || s.at(i+j) == '0' || s.at(i+j) == '1' || s.at(i+j) == '2' || s.at(i+j) == '3' || s.at(i+j) == '4' || s.at(i+j) == '5' || s.at(i+j) == '6' || s.at(i+j) == '7' || s.at(i+j) == '8' || s.at(i+j) == '9'){
            keta++;
        }
        else{
            break;
        }
    }
    int count = 0;
    bool minus = false;
    for(int j = 0;j < keta;j++){
        count *= 10;
        switch (s.at(i+j)) {
            case '-': minus = true;break;
            case '0': count += 0;break;
            case '1': count += 1;break;
            case '2': count += 2;break;
            case '3': count += 3;break;
            case '4': count += 4;break;
            case '5': count += 5;break;
            case '6': count += 6;break;
            case '7': count += 7;break;
            case '8': count += 8;break;
            case '9': count += 9;break;
            default: return 0;
        }
    }
    if(minus){
        count *= -1;
    }
    i += keta;
    return count;
}

QrConverterField::QrConverterField()
{
    std::string s = read_only_data;
    int stage = 0;
    std::queue<int> HW;
    int H, W;
    int i = 0;
    std::vector<std::vector<int>> vec;
    for(i = 0;stage != 2;i++){
            if(s.at(i) == '-' || s.at(i) == '0' || s.at(i) == '1' || s.at(i) == '2' || s.at(i) == '3' || s.at(i) == '4' || s.at(i) == '5' || s.at(i) == '6' || s.at(i) == '7' || s.at(i) == '8' || s.at(i) == '9'){
                HW.push(ctoi(s,i));
                stage++;
                continue;
            }
    }
    H = HW.front();
    HW.pop();
    W = HW.front();
    int brock = 0;
    vec = std::vector<std::vector<int>>(H, std::vector<int>(W, 0));
    for(stage = 0;stage <= H;i++){
        if(s.at(i) == ':'){
            stage++;
            brock = 0;
            continue;
        }
        else if(s.at(i) == '-' || s.at(i) == '0' || s.at(i) == '1' || s.at(i) == '2' || s.at(i) == '3' || s.at(i) == '4' || s.at(i) == '5' || s.at(i) == '6' || s.at(i) == '7' || s.at(i) == '8' || s.at(i) == '9'){
                vec [stage-1][brock] = ctoi(s,i);
                brock++;
                continue;
            }
    }
    std::vector<int> A (2);
    std::vector<int> B (2);
    int b = 0;
    for(int a = 0;a < 2;i++){
        if(s.at(i) == ':'){
            a++;
            b = 0;
            continue;
        }
        else if(s.at(i) == '-' || s.at(i) == '0' || s.at(i) == '1' || s.at(i) == '2' || s.at(i) == '3' || s.at(i) == '4' || s.at(i) == '5' || s.at(i) == '6' || s.at(i) == '7' || s.at(i) == '8' || s.at(i) == '9'){
                if(a == 0){
                    A.at(b) = ctoi(s,i);
                    b++;
                }
                else if(a == 1){
                    B.at(b) = ctoi(s,i);
                    b++;
                }
            }
    }
}
