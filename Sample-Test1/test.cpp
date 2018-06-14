#include "pch.h"
#include "../VisitorPattern/Hireman.h"
#include "../VisitorPattern/Buyer.h"
#include "../VisitorPattern/Product.h"
#include <vector>

using std::vector;

TEST(TestCaseName, VisitorCommandTest) {
	vector<int> prices = { 90, 67, 34, 29, 100 };
	vector<Product> products;
	for (size_t i = 0; i < prices.size(); ++i) {
		products.push_back(Product(prices[i]));
	}
	Manager seller;
	seller.setFeatProducts(products);
	Buyer b;
	int price = b.askPrice(seller);
	std::cout << "Total price = " << price << std::endl;
	int actualPrice=0;
	for (size_t i = 0; i < products.size(); ++i) {
		actualPrice += prices[i];
	}
	EXPECT_EQ(price, actualPrice);
}