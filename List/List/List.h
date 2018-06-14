#ifndef LIST_H
#define LIST_H

#include "common.h"
#include "ListBase.h"
#include <iostream>
using namespace std;

namespace MyContainer {

	template<class T>
	class ListNode : public NodeBase<T, ListNode<T>> {
	public:
		ListNode(const T &elem, ListNode<T> * next) : NodeBase<T, ListNode<T>>(elem, next) {}
	};

	template<class T>
	class List : public ListBase<T, ListNode<T>>
	{
		using Node = ListNode<T>;
		using Nodep = Node * ;
	protected:
		void init(const T &elem);
		Nodep m_back;
	public:
		List();
		List(const T &elem);
		~List();
		virtual void push_front(const T &elem);
		virtual void push_back(const T &elem);
		virtual void pop_front();
		virtual void pop_back();
		virtual void show();
		virtual void clear();
		virtual T& back();

#ifdef UNIT_TESTING
		friend class ListTest;
#endif
	};

	template<class T>
	T& MyContainer::List<T>::back()
	{
		if (!this->m_back)
			return this->null_element;
		return this->m_back->_elem;
	}

	template<class T>
	List<T>::List()
	{
		this->m_size = 0;
		this->m_front = nullptr;
		this->m_back = nullptr;
	}

	template<class T>
	void List<T>::init(const T &elem)
	{
		this->m_back = this->m_front = new Node(elem, nullptr);
		this->m_size = 1;
	}

	template<class T>
	void List<T>::show()
	{
		if (this->m_size == 0) {
			cout << "[empty list]" << endl;
			return;
		}
		Nodep p = this->m_front;
		while (p != this->m_back) {
			cout << p->_elem << " ";
			p = p->m_next;
		}
		cout << p->_elem << endl;
	}

	template<class T>
	void List<T>::clear()
	{
		if (this->m_size == 0)
			return;
		if (this->m_size == 1) {
			delete this->m_front;
			this->m_front = nullptr;
			this->m_back = nullptr;
			this->m_size = 0;
			return;
		}
		Nodep p = this->m_front;
		Nodep q = this->m_front->m_next;
		while (q != this->m_back) {
			delete p;
			p = q;
			q = q->m_next;
		}
		delete q;
		this->m_front = this->m_back = nullptr;
		this->m_size = 0;
	}

	template<class T>
	List<T>::~List()
	{
		clear();
	}

	template<class T>
	void List<T>::pop_back()
	{
		if (this->m_size == 0)
			return;
		if (this->m_size == 1) {
			this->m_front = this->m_back = nullptr;
			this->m_size = 0;
			return;
		}
		Nodep p = this->m_front;
		while (p->m_next->m_next)
			p = p->m_next;
		delete p->m_next;
		p->m_next = nullptr;
		this->m_back = p;
		--this->m_size;
	}

	template<class T>
	List<T>::List(const T &elem) {
		init(elem);
	}

	template<class T>
	void List<T>::pop_front()
	{
		if (this->m_size == 0)
			return;
		if (this->m_size == 1) {
			this->m_front = this->m_back = nullptr;
			this->m_size = 0;
			return;
		}
		Nodep p = this->m_front;
		this->m_front = this->m_front->m_next;
		delete p;
		--this->m_size;
	}

	template<class T>
	void List<T>::push_back(const T &elem)
	{
		if (this->m_size == 0) {
			init(elem);
			return;
		}
		this->m_back->m_next = new Node(elem, nullptr);
		this->m_back = this->m_back->m_next;
		++this->m_size;
	}

	template<class T>
	void List<T>::push_front(const T &elem)
	{
		if (this->m_size == 0) {
			init(elem);
			return;
		}
		this->m_front = new Node(elem, this->m_front);
		++this->m_size;
	}
}

#endif