#ifndef CYCLIC_DOUBLE_LINKED_LIST_H
#define CYCLIC_DOUBLE_LINKED_LIST_H

#include "common.h"
#include "DoubleLinkedList.h"

namespace MyContainer {

	template<class T>
	class CyclicDoubleLinkedList : public DoubleLinkedList<T>
	{
		using Node = DoubleLinkedNode<T>;
		using Nodep = Node * ;
	protected:
		virtual void init(const T &elem);
	public:
		CyclicDoubleLinkedList() : DoubleLinkedList<T>() {};
		CyclicDoubleLinkedList(const T &elem);
		virtual void clear();
		virtual void push_front(const T &elem);
		virtual void push_back(const T &elem);
		virtual void pop_front();
		virtual void pop_back();
		virtual void show();

#ifdef UNIT_TESTING
		friend class CyclicDoubleLinkedListTest;
#endif
	};

	template<class T>
	void CyclicDoubleLinkedList<T>::show()
	{
		if (this->m_size == 0) {
			cout << "[empty list]" << endl;
			return;
		}
		Nodep p = this->m_front;
		for (size_t i = 0; i < this->m_size - 1; ++i) {
			cout << p->_elem << " ";
			p = p->m_next;
		}
		cout << endl;
	}

	template<class T>
	void CyclicDoubleLinkedList<T>::pop_back()
	{
		if (this->m_size == 0)
			return;
		if (this->m_size == 1) {
			delete this->m_front;
			this->m_front = this->m_back = nullptr;
			this->m_size = 0;
			return;
		}
		Nodep p = this->m_back;
		this->m_back = this->m_back->m_prev;
		this->m_back->m_next = this->m_front;
		this->m_front->m_prev = this->m_back;
		delete p;
		--this->m_size;
	}

	template<class T>
	void CyclicDoubleLinkedList<T>::pop_front()
	{
		if (this->m_size == 0)
			return;
		if (this->m_size == 1) {
			delete this->m_front;
			this->m_front = this->m_back = nullptr;
			this->m_size = 0;
			return;
		}
		Nodep p = this->m_front;
		this->m_front = this->m_front->m_next;
		this->m_front->m_prev = this->m_back;
		this->m_back->m_next = this->m_front;
		delete p;
		--this->m_size;
	}

	template<class T>
	void CyclicDoubleLinkedList<T>::push_back(const T &elem)
	{
		if (this->m_size == 0) {
			init(elem);
			return;
		}
		this->m_back->m_next = new Node(elem, this->m_back, this->m_front);
		this->m_back = this->m_back->m_next;
		this->m_front->m_prev = this->m_back;
		++this->m_size;
	}

	template<class T>
	void CyclicDoubleLinkedList<T>::push_front(const T &elem)
	{
		if (this->m_size == 0) {
			init(elem);
			return;
		}
		this->m_front = new Node(elem, nullptr, this->m_front);
		++this->m_size;
	}

	template<class T>
	void CyclicDoubleLinkedList<T>::init(const T &elem)
	{
		this->m_back = this->m_front = new Node(elem, nullptr, nullptr);
		this->m_front->m_prev = this->m_front->m_next = this->m_back->m_next = this->m_back->m_prev = this->m_front;
		this->m_size = 1;
	}

	template<class T>
	CyclicDoubleLinkedList<T>::CyclicDoubleLinkedList(const T &elem)
	{
		init(elem);
	}

	template<class T>
	inline void CyclicDoubleLinkedList<T>::clear()
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
		for (size_t i = 0; i < this->m_size - 1; ++i) {
			delete p;
			p = q;
			q = q->m_next;
		};
		p->m_next = nullptr;
		delete p;
		this->m_front = nullptr;
		this->m_size = 0;
	}
}
#endif