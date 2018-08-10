#ifndef USEABSTRACTDATA_H
#define USEABSTRACTDATA_H

#include "algorithmwrapper.h"

class UseAbstractData : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int now_turn);

private:
    std::vector<std::vector<int>> getAbstractBasedAgent(bool side, bool agent);

    std::vector<int> x_list = {-1, 0, 1, 1, 1, 0, -1, -1};
    std::vector<int> y_list = {1, 1, 1, 0, -1, -1, -1, 0};
    /*

      12
      43

      111 333
      4 222 444

      */
};

#endif // USEABSTRACTDATA_H
