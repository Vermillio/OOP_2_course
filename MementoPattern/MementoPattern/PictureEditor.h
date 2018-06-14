#ifndef BUYER_H
#define BUYER_H
#include "common.h"
#include "Picture.h"
#include "Snapshot.h"
#include "State.h"
#include "Caretaker.h"


class PictureEditor : public Caretaker<State, Snapshot, Picture> {
    
public:
	PictureEditor() {};
    PictureEditor(Picturep t_picture) : Caretaker<State, Snapshot, Picture>(nullptr, t_picture) {};

    void change(const State & t_state) {
        memento=originator->makeMemento();
        originator->setState(t_state);
    };
    void cancel() {
        originator->setMemento(memento);
    };
	Picture getPicture() const { return *Caretaker::originator; };
    
};

#endif