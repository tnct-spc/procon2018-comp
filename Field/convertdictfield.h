#ifndef CONVERTDICTFIELD_H
#define CONVERTDICTFIELD_H

#include "field.h"

#pragma push_macro("slots")
#undef slots
#include <boost/python.hpp>
#pragma pop_macro("slots")

namespace procon
{
    namespace ConvertDictField{
        boost::python::dict convertDictFromField(procon::Field& field);
    }
};

#endif // CONVERTDICTFIELD_H
