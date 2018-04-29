#ifndef LIST_H
#define LIST_H

#include "ListBase.h"
#include <iostream>
using namespace std;

namespace MyContainer {

	template<class T>
	class ListNode : public NodeBase<T, ListNode<T>> {
	public:
		ListNode(T &elem, ListNode<T> * next) : NodeBase<T, ListNode<T>>(elem, next) {}
	};

	template<class T>
	class List : public ListBase<T, ListNode<T>>
	{
		using Node = ListNode<T>;
		using Nodep = Node * ;
	protected:
		void init(T &elem);
		Nodep _back;
	public:
		List();
		List(T &elem);
		~List();
		virtual void push_front(T &elem);
		virtual void push_back(T &elem);
		virtual void pop_front();
		virtual void pop_back();
		virtual void show();
		virtual void clear();
		virtual T& back();
	};

	template<class T>
	T& MyContainer::List<T>::back()
	{
		if (!this->_back)
			return this->null_element;
		return this->_back->_elem;
	}

	template<class T>
	List<T>::List()
	{
		this->_size = 0;
		this->_front = nullptr;
		this->_back = nullptr;
	}

	template<class T>
	void List<T>::init(T &elem)
	{
		this->_back = this->_front = new Node(elem, nullptr);
		this->_size = 1;
	}

	template<class T>
	void List<T>::show()
	{
		if (this->_size == 0) {
			cout << "[empty list]" << endl;
			return;
		}
		Nodep p = this->_front;
		while (p != this->_back) {
			cout << p->_elem << " ";
			p = p->_next;
		}
		cout << p->_elem << endl;
	}

	template<class T>
	void List<T>::clear()
	{
		if (this->_size == 0)
			return;
		if (this->_size == 1) {
			delete this->_front;
			this->_front = nullptr;
			this->_back = nullptr;
			this->_size = 0;
			return;
		}
		Nodep p = this->_front;
		Nodep q = this->_front->_next;
		while (q != this->_back) {
			delete p;
			p = q;
			q = q->_next;
		}
		delete q;
		this->_front = this->_back = nullptr;
		this->_size = 0;
	}

	template<class T>
	List<T>::~List()
	{
		clear();
	}

	template<class T>
	void List<T>::pop_back()
	{
		if (this->_size == 0)
			return;
		if (this->_size == 1) {
			this->_front = this->_back = nullptr;
			this->_size = 0;
			return;
		}
		Nodep p = this->_front;
		while (p->_next->_next)
			p = p->_next;
		delete p->_next;
		p->_next = nullptr;
		this->_back = p;
		--this->_size;
	}

	template<class T>
	List<T>::List(T &elem) {
		init(elem);
	}

	template<class T>
	void List<T>::pop_front()
	{
		if (this->_size == 0)
			return;
		if (this->_size == 1) {
			this->_front = this->_back = nullptr;
			this->_size = 0;
			return;
		}
		Nodep p = this->_front;
		this->_front = this->_front->_next;
		delete p;
		--this->_size;
	}

	template<class T>
	void List<T>::push_back(T &elem)
	{
		if (this->_size == 0) {
			init(elem);
			return;
		}
		this->_back->_next = new Node(elem, nullptr);
		this->_back = this->_back->_next;
		++this->_size;
	}

	template<class T>
	void List<T>::push_front(T &elem)
	{
		if (this->_size == 0) {
			init(elem);
			return;
		}
		this->_front = new Node(elem, this->_front);
		++this->_size;
	}
}

#endif