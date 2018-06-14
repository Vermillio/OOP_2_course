#ifndef BUYER_H
#define BUYER_H

#include "Visitor.h"
#include "Hireman.h"
#include "Command.h"


class Buyer {

protected:
	Command<int, Manager> * command = new AskPriceCommand();

public:
	int askPrice(Manager seller) {
		return command->execute(seller);
	}
};



#endif