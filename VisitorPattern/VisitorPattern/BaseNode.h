#ifndef BASE_NODE_H
#define BASE_NODE_H


template<class T>
class Visitor;

class Manager;

template<class T>
class BaseNode {
    virtual int accept(Manager *) = 0;
};

#endif