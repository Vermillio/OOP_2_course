#pragma once

#include "CyclicList.h"
#include "DoubleLinkedList.h"



template<class T>
class CyclicDoubleSidedList : public DoubleLinkedList, public CyclicList
{
public:
	CyclicDoubleSidedList(T &elem);
	virtual void push_front(T elem);
	virtual void push_back(T elem);
	virtual void pop_front();
	virtual void pop_back();
	virtual T& front();
	virtual T& back();
};