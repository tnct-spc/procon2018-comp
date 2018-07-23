#include "testconvertfield.h"

TestConvertField::TestConvertField()
{

}

void TestConvertField::run(){
    std::string input;
    /*
    std::cout << "please input : ";
    std::cin >> input;
    */
    input = "8 11:-2 1 0 1 2 0 2 1 0 1 -2:1 3 2 -2 0 1 0 -2 2 3 1:1 3 2 1 0 -2 0 1 2 3 1:2 1 1 2 2 3 2 2 1 1 2: 2 1 1 2 2 3 2 2 1 1 2: 1 3 2 1 0 -2 0 1 2 3 1: 1 3 2 -2 0 1 0 -2 2 3 1: -2 1 0 1 2 0 2 1 0 1 -2:2 2:7 10: ";

    QrConverterField StrToField;
    procon::Field field = StrToField.ConvertCsvToField(input);
    procon::CsvIo::exportField(field, "../../convertedcsvdata.csv");

    std::cout << "finished" << std::endl;
}
