#ifndef ORIGINATOR_H
#define ORIGINATOR_H
#include "common.h"



template<typename StateClass, class MemClass>
class Originator {
public:
    virtual StateClass getState() const = 0;
    virtual void setState(const StateClass &) = 0;
    virtual shared_ptr<MemClass> makeMemento() const = 0;
    virtual void setMemento(shared_ptr<MemClass>) = 0;
};

#endif