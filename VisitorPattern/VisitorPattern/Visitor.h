#ifndef VISITOR_H
#define VISITOR_H

template<class T>
class BaseNode;
class Product;

template<class T>
class Visitor {
    virtual int visit(Product *) = 0;
};

#endif