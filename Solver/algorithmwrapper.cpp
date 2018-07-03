#include "algorithmwrapper.h"
#include "gamemanager.h"

AlgorithmWrapper::AlgorithmWrapper(const procon::Field& field, int final_turn, bool side) :
    field(field),
    final_turn(final_turn),
    side(side)
{

}
