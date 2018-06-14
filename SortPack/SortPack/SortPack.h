#pragma once

#define UNIT_TESTING

#include <iostream>

using namespace std;



class Item
{
public:
	virtual bool operator<(const Item&) const = 0;
	virtual bool operator<=(const Item&) const = 0;
	virtual bool operator==(const Item&) const = 0;
	virtual void showItem() = 0;

};

class IntItem : public Item
{
	int value;
public:
	IntItem() {};
	IntItem(int i) :value(i) {};
	~IntItem() {};

	bool equalTo(const IntItem & item) const { return value == item.value; };

	bool operator<(const Item&) const;

	bool operator<(const IntItem&) const;

	bool operator<=(const Item& item) const { return (*this) < item || (*this) == item; };

	bool operator==(const Item& item) const;

	void operator= (const int &_value) {
		value = _value;
	}
	void operator= (const IntItem &item) {
		value = item.value;
	}

	void showItem();
};


class ArrayOfItems
{
public:
	virtual Item& operator[](int) = 0;
	virtual void fill() = 0;
	virtual void show() = 0;
	virtual void swap(int x, int y) = 0;
	virtual bool checkOrder() = 0;
};

class ArrayOfInt :public ArrayOfItems
{
	IntItem* arrayToSort;
	int length;
public:
	ArrayOfInt(int _length) : length(_length) {
		if (_length < 0)
			exception("Negative size is prohibited.");
		arrayToSort = new IntItem[length];
	};
	~ArrayOfInt() {
		arrayToSort = nullptr;
		length = 0;
	};
	Item& operator[](int pos) {
		if (pos >= length || pos < 0)
			exception("Out of range.");
		return arrayToSort[pos];
	};
	void fill() {
		for (int i = 0; i < length; ++i) {
			arrayToSort[i] = (int)rand();
		}
	};

	bool checkOrder() {
		for (int i = 0; i < length - 1; ++i) {
			if (arrayToSort[i + 1] < arrayToSort[i])
				return false;
		}
		return true;
	}

	void swap(int x, int y);
	void show();
};

class SortPack
{
	ArrayOfItems * items;
	int quantity;
	int counter;

	int partition(int first, int last, Item &pivot);
	void split(int first, int last);

public:
	SortPack() :counter(0) {};
	void bubble();
	void select();
	void insert();
	void shell();
	void quick();
	void attach(ArrayOfItems&, int);
	void show();
	void discardCounter();
	bool checkOrder() { 
		if (items)
			return items->checkOrder();
		else return false;
	}
	void increaseConter();

#ifdef UNIT_TESTING
	friend class TestSortPack;
#endif
};

