#include "qrconverterfield.h"
#include <bits/stdc++.h>
using namespace std;

int ctoi(string s,int& i) {
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
        cout << s.at(i) << endl;
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
    cout << "final number is " << count << endl;
    return count;
}

QrConverterField::QrConverterField()
{
    string s = read_only_data;
    int stage = 0;
    queue<int> HW;
    for(int i = 0;i < s.size();i++){
        cout << "check" << " [" << s.at(i) << "]" << endl;
        if(s.at(i) == ':'){
            cout << "stage up" << endl;
            stage++;
            continue;
        }
        else if(stage == 0){
            cout << "stage1" << endl;
            if(s.at(i) == '-' || s.at(i) == '0' || s.at(i) == '1' || s.at(i) == '2' || s.at(i) == '3' || s.at(i) == '4' || s.at(i) == '5' || s.at(i) == '6' || s.at(i) == '7' || s.at(i) == '8' || s.at(i) == '9'){
                HW.push(ctoi(s,i));
                continue;
            }
            else{
                continue;
            }
        }
        else if(stage == 1){
            break;
        }
        else{
            cout << "stage is unknown" << endl;
            continue;
        }
    }
    cout << HW.front() << endl;
    HW.pop();
    cout << HW.front() << endl;
}
