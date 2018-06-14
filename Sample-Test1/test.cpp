#include "pch.h"
#include "../SortPack/SortPack.h"
#include <ctime>

class TestSortPack : public ::testing::Test {
protected:
	SortPack testee;

public:

	virtual void SetUp() {
		int length = 50000;
		ArrayOfInt arr(length);
		arr.fill();
		testee.attach(arr, length);
	};

	virtual void TearDown() {

	};


	TestSortPack() {};
	~TestSortPack() {};
	bool checkOrder() { return testee.checkOrder(); }
	void bubble() { testee.bubble(); };
	void select() { testee.select(); };
	void insert() { testee.insert(); };
	void shell() { testee.shell(); };
	void quick() { testee.quick(); };
	void attach(ArrayOfItems& arr, int l) { testee.attach(arr, l); };
	void show() { testee.show(); };
	void discardCounter() { testee.discardCounter(); };
	void increaseConter() { testee.increaseConter(); };
	int partition(int first, int last, Item &pivot) { return testee.partition(first, last, pivot); };
	void split(int first, int last) { testee.split(first, last); };
};


TEST_F(TestSortPack, Bubble) {
	time_t timer = clock();
	bubble();
	timer = clock() - timer;
	EXPECT_EQ(checkOrder(), true);
}

TEST(er, er) {
	EXPECT_EQ(true, true);
}