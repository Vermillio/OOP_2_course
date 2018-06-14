#ifndef CARETAKER_H
#define CARETAKER_H
#include "common.h"


template<class StateClass, class MemClass, class OrigClass>
class Caretaker {
protected:
    shared_ptr<MemClass> memento;
    shared_ptr<OrigClass> originator;
public:
	Caretaker() {};
    Caretaker(shared_ptr<MemClass> t_memento, shared_ptr<OrigClass> t_originator) : memento(t_memento), originator(t_originator) {};
    virtual void change(const StateClass &) = 0;
    virtual void cancel() = 0;
};

#endif