#ifndef PROCONTHREAD_H
#define PROCONTHREAD_H

#include "proconthread_global.h"
#include <pthread.h>

namespace procon {

class PROCONTHREADSHARED_EXPORT thread
{

public:
    thread();
    virtual ~thread();
    void stop();

};

}

#endif // PROCONTHREAD_H
