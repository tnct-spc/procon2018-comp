#include "qrconverterfield.h"

QrConverterField::QrConverterField(){

}

int QrConverterField::ctoi(std::string s, int &i){

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
    i += keta-1;
    return count;
}

procon::Field QrConverterField::ConvertCsvToField(std::string csv_string){
    std::vector<int> agent1_convert;
    std::vector<int> agent2_convert;
    std::vector<std::vector<int>> field_convert;
    int stage = 0;
    std::queue<int> HW;
    std::pair<int, int> field_size;
    int i = 0;
    for(i = 0;stage != 2;i++){
            if(csv_string.at(i) == '-' || csv_string.at(i) == '0' || csv_string.at(i) == '1' || csv_string.at(i) == '2' || csv_string.at(i) == '3' || csv_string.at(i) == '4' || csv_string.at(i) == '5' || csv_string.at(i) == '6' || csv_string.at(i) == '7' || csv_string.at(i) == '8' || csv_string.at(i) == '9'){
                HW.push(ctoi(csv_string,i));
                stage++;
                continue;
            }
    }
    field_size.second = HW.front();
    HW.pop();
    field_size.first = HW.front();
    int brock = 0;
    field_convert = std::vector<std::vector<int>>(field_size.first, std::vector<int>(field_size.second, 0));
    for(stage = 0;stage <= field_size.second;i++){
        if(csv_string.at(i) == ':'){
            stage++;
            brock = 0;
            continue;
        }
        else if(csv_string.at(i) == '-' || csv_string.at(i) == '0' || csv_string.at(i) == '1' || csv_string.at(i) == '2' || csv_string.at(i) == '3' || csv_string.at(i) == '4' || csv_string.at(i) == '5' || csv_string.at(i) == '6' || csv_string.at(i) == '7' || csv_string.at(i) == '8' || csv_string.at(i) == '9'){
                field_convert [brock][stage-1] = ctoi(csv_string,i);
                brock++;
                continue;
            }
    }
    int b = 0;
    for(int a = 0;a < 2;i++){
        if(csv_string.at(i) == ':'){
            a++;
            b = 0;
            continue;
        }
        else if(csv_string.at(i) == '-' || csv_string.at(i) == '0' || csv_string.at(i) == '1' || csv_string.at(i) == '2' || csv_string.at(i) == '3' || csv_string.at(i) == '4' || csv_string.at(i) == '5' || csv_string.at(i) == '6' || csv_string.at(i) == '7' || csv_string.at(i) == '8' || csv_string.at(i) == '9'){
            if(a == 0){
                agent1_convert.push_back(ctoi(csv_string,i));
                b++;
            }
            else if(a == 1){
                agent2_convert.push_back(ctoi(csv_string,i));
                b++;
            }
        }
    }

    procon::Field field(field_size.first,field_size.second);

    // 仮の対応処置です 実際は終了ターンを手打ちします
    field.setFinalTurn(60);
    field.setTurnCount(0);

    field.setStates(std::vector<std::vector<int>>(field_size.first, std::vector<int>(field_size.second, 0)));

    field.setValue(field_convert);
    field.setAgent(0,0,agent1_convert.at(1) - 1,agent1_convert.at(0) - 1);
    field.setAgent(0,1,agent2_convert.at(1) - 1,agent2_convert.at(0) - 1);

    std::vector<std::pair<int,int>> agents = field.guessAgents(1);
    for(int index = 0; index < 2; ++index)
        field.setAgent(1, index, agents.at(index).first, agents.at(index).second);

    for(int side = 0; side < 2; ++side)
        for(int agent = 0; agent < 2; ++agent)
            field.setState(field.getAgent(side, agent).first, field.getAgent(side, agent).second, side + 1);

    return std::move(field);
}
