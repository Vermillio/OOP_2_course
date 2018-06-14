#ifndef CYCLIC_LIST_H
#define CYCLIC_LIST_H

#include "common.h"
#include "List.h"

namespace MyContainer {

	template<class T>
	class CyclicList : public List<T>
	{
		using Node = ListNode<T>;
		using Nodep = Node * ;
		virtual void init(const T &elem);
	public:
		CyclicList() : List<T>() {};
		CyclicList(const T &elem);
		~CyclicList() { clear(); }
		virtual void push_front(const T &elem);
		virtual void push_back(const T &elem);
		virtual void pop_front();
		virtual void pop_back();
		virtual void clear();
		virtual void show();

#ifdef UNIT_TESTING
		friend class CyclicListTest;
#endif

	};

	template<class T>
	void MyContainer::CyclicList<T>::clear()
	{
		if (this->m_size == 0)
			return;
		if (this->m_size == 1) {
			delete this->m_front;
			this->m_front = nullptr;
			this->m_size = 0;
			return;
		}
		Nodep p = this->m_front;
		Nodep q = this->m_front->m_next;
		for (size_t i = 0; i < this->m_size-1; ++i) {
			delete p;
			p = q;
			q = q->m_next;
		};
		p->m_next = nullptr;
		delete p;
		this->m_front = nullptr;
		this->m_size = 0;
	}

	template<class T>
	void MyContainer::CyclicList<T>::show()
	{
		Nodep p = this->m_front;
		for (size_t i = 0; i < this->m_size-1; ++i) {
			cout << p->_elem << " ";
			p = p->m_next;
		}
		cout << endl;
	}

	template<class T>
	void MyContainer::CyclicList<T>::pop_back()
	{
		if (this->m_size == 0)
			return;
		if (this->m_size == 1) {
			this->m_front = nullptr;
			this->m_size = 0;
			return;
		}
		Nodep p = this->m_front;
		for (size_t i = 0; i < this->m_size-2; ++i)
			p = p->m_next;
		delete p->m_next;
		p->m_next = this->m_front;
		--this->m_size;
	}

	template<class T>
	void MyContainer::CyclicList<T>::pop_front()
	{
		if (this->m_size == 0)
			return;
		if (this->m_size == 1) {
			this->m_front = nullptr;
			this->m_size = 0;
			return;
		}
		Nodep p = this->m_front->m_next;
		Nodep q = this->m_front;
		for (size_t i = 0; i < this->m_size - 1; ++i)
			q = q->m_next;
		q->m_next = p;
		delete this->m_front;
		this->m_front = p;
		--this->m_size;
	}

	template<class T>
	void MyContainer::CyclicList<T>::push_back(const T &elem)
	{
		if (this->m_size == 0) {
			init(elem);
			return;
		}
		Nodep bck = this->m_front;
		for (size_t i = 0; i < this->m_size - 1; ++i)
			bck = bck->m_next;
		bck->m_next = new Node(elem, this->m_front);
		++this->m_size;
	}

	template<class T>
	void MyContainer::CyclicList<T>::push_front(const T &elem)
	{
		if (this->m_size == 0) {
			init(elem);
			return;
		}
		this->m_front = new Node(elem, this->m_front);
		++this->m_size;
		Nodep bck = this->m_front;
		for (size_t i = 0; i < this->m_size - 1; ++i)
			bck = bck->m_next;
		bck->m_next = this->m_front;
	}

	template<class T>
	void MyContainer::CyclicList<T>::init(const T &elem)
	{
		this->m_front = new Node(elem, nullptr);
		this->m_front->setNext(this->m_front);
		this->m_size = 1;
	}

	template<class T>
	MyContainer::CyclicList<T>::CyclicList(const T &elem)
	{
		init(elem);
	}

}
#endif