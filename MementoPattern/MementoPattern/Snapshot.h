#ifndef PRODUCT_H
#define PRODUCT_H
#include "common.h"
#include "State.h"
#include "Memento.h"

class Snapshot : Memento<State> {
    
    State state;
    
public:

    Snapshot(const State & t_state) : state(t_state) {};
    State getState() const {return state;}
    void setState(const State & t_state) {state=t_state;}
};

#endif