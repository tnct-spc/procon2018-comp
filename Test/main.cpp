#include <iostream>
#include "testagentparam.h"


int main()
{

    std::cout << "select mode" << std::endl;
    std::cout << "0 : TestAgentParam" << std::endl;


    int test_number = -1;

    while(1){

        std::cout << "algo number : ";
        std::cin >> test_number;

        if(test_number == 0)
            break;

        std::cout << "incorrect" << std::endl;
    }

    if(test_number == 0){
        TestAgentParam test;
        test.run();
    }



    return 0;
}
