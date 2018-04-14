#ifndef GAMESIMULATOR_H
#define GAMESIMULATOR_H

#include "field.h"

class GameSimulator
{

public:
    GameSimulator(const procon::Field& inp_field);

private:
    procon::Field field;
};

#endif // GAMESIMULATOR_H
