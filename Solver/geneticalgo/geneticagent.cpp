#include "geneticagent.h"

GeneticAgent::GeneticAgent(bool flag)
{

    std::random_device rnd;
    std::mt19937 mt(rnd());

    std::uniform_real_distribution<> rand_double(0.0, 1.0);


    if(flag){
        rand = rand_double(mt);
        minus = rand_double(mt);
        next_p = rand_double(mt);
        region = rand_double(mt);
        removal = rand_double(mt);
        nomove = rand_double(mt);
        backmove = rand_double(mt);

        /* GAで生成された手 コメントアウトすればこれで戦える
        std::vector<double> val = {0.0873751, 0.302949, 0.508008, 0.305717, 0.301959, 0.759892, 0.717588 };
        setData(val);
        */

    }
}
bool GeneticAgent::operator<(const GeneticAgent &other) const {
    return 1.0 * win_count / try_count < 1.0 * other.win_count / other.try_count;
}

void GeneticAgent::setData(const std::vector<double>& inp){

    rand = inp.at(0);
    minus = inp.at(1);
    next_p = inp.at(2);
    region = inp.at(3);
    removal = inp.at(4);
    nomove = inp.at(5);
    backmove = inp.at(6);

}

std::vector<double> GeneticAgent::getData(){
    std::vector<double> ret;
    ret = {rand,minus,next_p,region,removal,nomove,backmove};

    return std::move(ret);
}
