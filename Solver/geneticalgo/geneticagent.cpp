#include "geneticagent.h"

GeneticAgent::GeneticAgent(int size, int flag) :
    size(size)
{

    std::random_device rnd;
    std::mt19937 mt(rnd());

    std::uniform_real_distribution<> rand_double(0.0, 1.0);

    status.resize(size, 0.0);

    if(flag == 1)
        for(int index = 0; index < size; ++index)
            status.at(index) = rand_double(mt);

    if(flag == 2){
        std::vector<double> rand_status(size + 1);
        for(int count = 0; count < size - 1; ++count)
            rand_status.at(count) = rand_double(mt);

        rand_status.at(size - 1) = 0;
        rand_status.at(size) = 1;

        std::sort(rand_status.begin(), rand_status.end());

        for(int index = 0; index < size; ++index)
            status.at(index) = rand_status.at(index + 1) - rand_status.at(index);
    }

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
