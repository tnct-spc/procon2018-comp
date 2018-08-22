#include <iostream>
#include "testagentparam.h"
#include "testevaluatespeed.h"
#include "testconvertfield.h"

int main()
{

    std::cout << "select mode" << std::endl;
    std::cout << "0 : TestAgentParam" << std::endl;
    std::cout << "1 : TestEvaluateSpeed" << std::endl;
    std::cout << "2 : TestConvertField" << std::endl;

    int test_number = -1;

    while(1){

        std::cout << "algo number : ";
        std::cin >> test_number;

        if(test_number >= 0 && test_number <= 2)
            break;

        std::cout << "incorrect" << std::endl;
    }

    if(test_number == 0){
        TestAgentParam test;
        int test_number = -1;

        std::cout << "select mode" << std::endl;
        std::cout << "0 : runFix" << std::endl;
        std::cout << "1 : runRand" << std::endl;
        std::cout << "2 : runDoubleRand" << std::endl;

        while(1){

            std::cout << "test number : ";
            std::cin >> test_number;

            if(test_number >= 0 && test_number <= 2)
                break;

            std::cout << "incorrect" << std::endl;
        }

        if(test_number == 0)
            test.runFix();
        if(test_number == 1 || test_number == 2)
            test.runRand( (test_number == 2) );
    }
    else if(test_number == 1){

        TestEvaluateSpeed test;
        test.run();
    }
    else if(test_number == 2){
        TestConvertField test;
        test.run();
    }



    return 0;
}
