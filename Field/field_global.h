#ifndef FIELD_GLOBAL_H
#define FIELD_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FIELD_LIBRARY)
#  define FIELDSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FIELDSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FIELD_GLOBAL_H
