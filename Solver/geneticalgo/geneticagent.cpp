#include "geneticagent.h"

GeneticAgent::GeneticAgent()
{

    std::random_device rnd;
    std::mt19937 mt(rnd());

    std::uniform_real_distribution<> rand_double(0.0, 1.0);

    rand = rand_double(mt);
    minus = rand_double(mt);
    next_p = rand_double(mt);
    region = rand_double(mt);
    removal = rand_double(mt);
    nomove = rand_double(mt);
    backmove = rand_double(mt);
}
bool GeneticAgent::operator<(const GeneticAgent &other) const {
    return 1.0 * win_count / try_count < 1.0 * other.win_count / other.try_count;
}
