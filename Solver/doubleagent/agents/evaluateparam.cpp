#include "evaluateparam.h"

double EvaluateParam::evaluateMove(int move, bool is_delete){

    const std::vector<int> x_list = {1, 1, 1, 0,  0, -1, -1, -1, 0};
    const std::vector<int> y_list = {-1, 0, 1, -1, 1, -1, 0, 1, 0};

    procon::Field& field = manager->getField();

    if(!manager->canPut(side, agent, move, false))
        return -300000;

}
