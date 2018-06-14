#ifndef VISITOR_H
#define VISITOR_H
#include "common.h"


template<class T>
class Memento {
public:
    virtual T getState() const = 0;
    virtual void setState(const T &) = 0;
};

#endif
