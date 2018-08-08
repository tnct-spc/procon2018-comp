#ifndef CATCHPYTHONSANSWER_H
#define CATCHPYTHONSANSWER_H
#include "field.h"
#include "gamemanager.h"
#include "algorithmwrapper.h"
#include "communication.h"
#include "visualizer.h"
#include <queue>
#include <algorithm>
#include <iostream>

#include <string>
#include <fstream>
#include <streambuf>
#include <string>

#pragma push_macro("slots")
#undef slots
#include <boost/python.hpp>
#pragma pop_macro("slots")


class catchPythonsAnswer : public AlgorithmWrapper
{
    using AlgorithmWrapper::AlgorithmWrapper;
public:
    const std::pair<std::tuple<int,int,int>, std::tuple<int,int,int>> agentAct(int);

    const std::string Path = "";
};

#endif // CATCHPYTHONSANSWER_H
