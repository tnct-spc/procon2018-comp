#include "geneticagent.h"

GeneticAgent::GeneticAgent(int size, bool flag) :
    size(size)
{

    std::random_device rnd;
    std::mt19937 mt(rnd());

    std::uniform_real_distribution<> rand_double(0.0, 1.0);

    status.resize(size, 0.0);

    if(flag)
        for(int index = 0; index < size; ++index)
            status.at(index) = rand_double(mt);
}
bool GeneticAgent::operator<(const GeneticAgent &other) const {
    return 1.0 * win_count / try_count < 1.0 * other.win_count / other.try_count;
}

void GeneticAgent::setData(const std::vector<double>& inp){

    status = inp;
}

const std::vector<double>& GeneticAgent::getData() const {

    return status;
}
