#pragma once

#include "Product.h"
#include "Visitor.h"
#include "vector"

using std::vector;

class Manager : public Visitor<int> {

	vector<Product> featuredProducts;

public:

	void setFeatProducts(vector<Product> products) { featuredProducts = products; };

	int visit(Product * pr) {
		return pr->accept(this);
	};

	int visitAll() {
		int sum = 0;
			for (size_t i = 0; i < featuredProducts.size(); ++i) {
				sum += visit(&featuredProducts[i]);
			}
		return sum;
	}
};