#ifndef CYCLIC_LIST_H
#define CYCLIC_LIST_H
#include "List.h"

template<class T>
class CyclicList : virtual public List
{
public:
	CyclicList(T &elem);
	virtual void push_front(T elem);
	virtual void push_back(T elem);
	virtual void pop_front();
	virtual void pop_back();
	virtual T& front();
	virtual T& back();
};

#endif