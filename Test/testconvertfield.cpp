#include "testconvertfield.h"

TestConvertField::TestConvertField()
{

}

void TestConvertField::run(){
    std::string input;
    std::cout << "please input" << std::endl;
    std::cin >> input;

    QrConverterField StrToField;
    procon::Field field = StrToField.ConvertCsvToField(input);
    procon::CsvIo::exportField(field, "../../convertedcsvdata.csv");

    std::cout << "finished" << std::endl;
}
