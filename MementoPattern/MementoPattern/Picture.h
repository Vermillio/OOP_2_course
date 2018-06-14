#ifndef BASE_NODE_H
#define BASE_NODE_H
#include "common.h"
#include "Snapshot.h"
#include "State.h"
#include "Originator.h"

class Picture : Originator<State, Snapshot> {
    
    string title;
    State state;
    
public:

    Picture(const string & t_title, const State & t_state) : 
    title(t_title), state(t_state) {};
    void setState(const State & t_state) {
        state=t_state;
    }
    string getTitle() const {return title;}
    State getState() const {return state; }
    Snapshotp makeMemento() const {
        return Snapshotp(new Snapshot(state));
       // return p; //make_shared<Snapshot>(brightness, contrast, saturation);
    }
    void setMemento(Snapshotp t_snapshot) {
        if (!t_snapshot)
            return;
        state=t_snapshot->getState();
    }
    
    string to_json() const {
        return state.to_json();
    }
    
};

#endif