#ifndef CYCLIC_LIST_H
#define CYCLIC_LIST_H
#include "List.h"

namespace MyContainer {

	template<class T>
	class CyclicList : public ListBase<T, ListNode<T>>
	{
		using Node = ListNode<T>;
		using Nodep = Node * ;
	public:
		CyclicList(T &elem);
		~CyclicList() { clear(); }
		virtual void init(T &elem);
		virtual void push_front(T &elem);
		virtual void push_back(T &elem);
		virtual void pop_front();
		virtual void pop_back();
		virtual void clear();
		virtual void show();
	};

	template<class T>
	void MyContainer::CyclicList<T>::clear()
	{
		if (this->_size == 0)
			return;
		if (this->_size == 1) {
			delete this->_front;
			this->_front = nullptr;
			this->_size = 0;
			return;
		}
		Nodep p = this->_front;
		Nodep q = this->_front->_next;
		for (size_t i = 0; i < this->_size-1; ++i) {
			delete p;
			p = q;
			q = q->_next;
		};
		delete q;
		this->_front = nullptr;
		this->_size = 0;
	}

	template<class T>
	void MyContainer::CyclicList<T>::show()
	{
		Nodep p = this->_front;
		for (size_t i = 0; i < this->_size-1; ++i) {
			cout << p->_elem << " ";
			p = p->_next;
		}
		cout << endl;
	}

	template<class T>
	void MyContainer::CyclicList<T>::pop_back()
	{
		if (this->_size == 0)
			return;
		if (this->_size == 1) {
			this->_front = nullptr;
			this->_size = 0;
			return;
		}
		Nodep p = this->_front;
		for (size_t i = 0; i < this->_size-2; ++i)
			p = p->_next;
		delete p->_next;
		p->_next = this->_front;
		--this->_size;
	}

	template<class T>
	void MyContainer::CyclicList<T>::pop_front()
	{
		if (this->_size == 0)
			return;
		if (this->_size == 1) {
			this->_front = nullptr;
			this->_size = 0;
			return;
		}
		Nodep p = this->_front->_next;
		Nodep q = this->_front;
		for (size_t i = 0; i < this->_size - 1; ++i)
			q = q->_next;
		q->_next = p;
		delete this->_front;
		this->_front = p;
		--this->_size;
	}

	template<class T>
	void MyContainer::CyclicList<T>::push_back(T elem)
	{
		if (this->_size == 0) {
			init(elem);
			return;
		}
		Nodep bck = this->_front;
		for (size_t i = 0; i < this->_size - 1; ++i)
			bck = bck->_next;
		bck->_next = new Node(elem, this->_front);
		++this->_size;
	}

	template<class T>
	void MyContainer::CyclicList<T>::push_front(T elem)
	{
		if (this->_size == 0) {
			init(elem);
			return;
		}
		this->_front = new Node(elem, this->_front);
		++this->_size;
		Nodep bck = this->_front;
		for (size_t i = 0; i < this->_size - 1; ++i)
			bck = bck->_next;
		bck->_next = this->_front;
	}

	template<class T>
	void MyContainer::CyclicList<T>::init(T &elem)
	{
		this->_front = new Node(elem, nullptr);
		this->_front->setNext(this->_front);
		this->_size = 1;
	}

	template<class T>
	MyContainer::CyclicList<T>::CyclicList(T &elem)
	{
		init(elem);
	}

}
#endif