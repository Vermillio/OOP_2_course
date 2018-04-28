#ifndef CYCLIC_DOUBLE_LINKED_LIST_H
#define CYCLIC_DOUBLE_LINKED_LIST_H

#include "DoubleLinkedList.h"

namespace MyContainer {

	template<class T>
	class CyclicDoubleLinkedList : public DoubleLinkedList<T>
	{
		using Node = DoubleLinkedNode<T>;
		using Nodep = Node * ;
	protected:
		virtual void init(T &elem);
	public:
		CyclicDoubleLinkedList(T &elem);
		virtual void push_front(T &elem);
		virtual void push_back(T &elem);
		virtual void pop_front();
		virtual void pop_back();
		virtual void show();
	};

	template<class T>
	void CyclicDoubleLinkedList<T>::show()
	{
		if (this->_size == 0) {
			cout << "[empty list]" << endl;
			return;
		}
		Nodep p = this->_front;
		for (size_t i = 0; i < this->_size - 1; ++i) {
			cout << p->_elem << " ";
			p = p->_next;
		}
		cout << endl;
	}

	template<class T>
	void CyclicDoubleLinkedList<T>::pop_back()
	{
		if (this->_size == 0)
			return;
		if (this->_size == 1) {
			delete this->_front;
			this->_front = this->_back = nullptr;
			this->_size = 0;
			return;
		}
		Nodep p = this->_back;
		this->_back = this->_back->_prev;
		this->_back->_next = this->_front;
		this->_front->_prev = this->_back;
		delete p;
		--this->_size;
	}

	template<class T>
	void CyclicDoubleLinkedList<T>::pop_front()
	{
		if (this->_size == 0)
			return;
		if (this->_size == 1) {
			delete this->_front;
			this->_front = this->_back = nullptr;
			this->_size = 0;
			return;
		}
		Nodep p = this->_front;
		this->_front = this->_front->_next;
		this->_front->_prev = this->_back;
		this->_back->_next = this->_front;
		delete p;
		--this->_size;
	}

	template<class T>
	void CyclicDoubleLinkedList<T>::push_back(T &elem)
	{
		if (this->_size == 0) {
			init(elem);
			return;
		}
		this->_back->_next = new Node(elem, this->_back, this->_front);
		this->_back = this->_back->_next;
		this->_front->_prev = this->_back;
		++this->_size;
	}

	template<class T>
	void CyclicDoubleLinkedList<T>::push_front(T &elem)
	{
		if (this->_size == 0) {
			init(elem);
			return;
		}
		this->_front = new Node(elem, nullptr, this->_front);
		++this->_size;
	}

	template<class T>
	void CyclicDoubleLinkedList<T>::init(T &elem)
	{
		this->_back = this->_front = new Node(elem, nullptr, nullptr);
		this->_front->_prev = this->_front->_next = this->_back->_next = this->_back->_prev = this->_front;
		this->_size = 1;
	}

	template<class T>
	CyclicDoubleLinkedList<T>::CyclicDoubleLinkedList(T &elem)
	{
		init(elem);
	}
}
#endif