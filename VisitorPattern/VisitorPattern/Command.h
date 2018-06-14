#pragma once
#include "Product.h"
#include "Hireman.h"

class Manager;

template<class T, class Y>
class Command {
public:
	virtual ~Command() {};
	virtual T execute(Y x)=0;
};


class AskPriceCommand : public Command<int, Manager> {
	int execute(Manager seller) {
		return seller.visitAll();
	}
};