#include <iostream>


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



    return 0;
}
