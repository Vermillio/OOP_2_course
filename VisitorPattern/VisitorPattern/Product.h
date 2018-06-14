#ifndef PRODUCT_H
#define PRODUCT_H

#include "BaseNode.h"
#include "Hireman.h"

class Manager;

class Product : public BaseNode<int> {
    int price;
public:
    Product(int t_price) : price(t_price) {};
    int accept(Manager * v) {
        return price;
    };
};

#endif