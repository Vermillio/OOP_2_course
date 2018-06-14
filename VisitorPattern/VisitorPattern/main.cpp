//Simple Visitor pattern example

#include "Buyer.h"
#include "Product.h"
#include "Hireman.h"
#include "Command.h"
#include <iostream>
#include <string>
#include <vector>





int main()
{
    std::cout << " ---Visitor pattern demo--- " << std::endl;
    int sum=0;
	Manager seller;
	seller.setFeatProducts(vector<Product>({ 90, 67, 34, 29, 100 }));
	Buyer b;
	int price = b.askPrice(seller);
    std::cout << "Total price = " << price << std::endl;

    return 0;
}



