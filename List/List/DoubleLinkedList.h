#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H

#include "common.h"
#include "ListBase.h"

namespace MyContainer {

	template<class T>
	class DoubleLinkedNode : public NodeBase<T, DoubleLinkedNode<T>> {
	public:
		DoubleLinkedNode<T> *m_prev;
		DoubleLinkedNode(const T &elem, DoubleLinkedNode<T>* prev, DoubleLinkedNode<T>* next) : NodeBase<T, DoubleLinkedNode<T>>(elem, next), m_prev(prev) {
			if (prev)
				prev->setNext(this);
			if (next)
				next->m_prev = this;
		}
	};

	template<class T>
	class DoubleLinkedList : public ListBase<T, DoubleLinkedNode<T>>
	{
		using Node = DoubleLinkedNode<T>;
		using Nodep = Node * ;
	protected:
		virtual void init(const T &elem);
		Nodep m_back;
	public:
		DoubleLinkedList() {
			this->m_size = 0;
			this->m_front = nullptr;
			this->m_back = nullptr;
		};
		DoubleLinkedList(const T &elem);
		~DoubleLinkedList() { clear(); };

		virtual void show();
		virtual void push_front(const T &elem);
		virtual void push_back(const T &elem);
		virtual void pop_front();
		virtual void pop_back();
		virtual void clear();
		T& back();

#ifdef UNIT_TESTING
		friend class DoubleLinkedListTest;
#endif

	};

	template<class T>
	T& MyContainer::DoubleLinkedList<T>::back()
	{
		if (!this->m_back)
			return this->null_element;
		return this->m_back->_elem;
	}

	template<class T>
	void DoubleLinkedList<T>::show()
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
	void DoubleLinkedList<T>::init(const T &elem)
	{
		this->m_back = this->m_front = new Node(elem, nullptr, nullptr);
		this->m_size = 1;
	}

	template<class T>
	void DoubleLinkedList<T>::clear()
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
	DoubleLinkedList<T>::DoubleLinkedList(const T &elem) {
		init(elem);
	}

	template<class T>
	void DoubleLinkedList<T>::push_front(const T &elem)
	{
		if (this->m_size == 0) {
			init(elem);
			return;
		}
		this->m_front = new Node(elem, nullptr, this->m_front);
		++this->m_size;
	}

	template<class T>
	void DoubleLinkedList<T>::push_back(const T &elem)
	{
		if (this->m_size == 0) {
			init(elem);
			return;
		}
		this->m_back->m_next = new Node(elem, this->m_back, nullptr);
		this->m_back = this->m_back->m_next;
		++this->m_size;
	}

	template<class T>
	void DoubleLinkedList<T>::pop_front()
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
		this->m_front->m_prev = nullptr;
		delete p;
		--this->m_size;
	}

	template<class T>
	void DoubleLinkedList<T>::pop_back()
	{
		if (this->m_size == 0)
			return;
		if (this->m_size == 1) {
			this->m_front = nullptr;
			this->m_back = nullptr;
			this->m_size = 0;
			return;
		}
		Nodep p = this->m_back;
		this->m_back = this->m_back->m_prev;
		this->m_back->m_next = nullptr;
		delete p;
		--this->m_size;
	}
}

#endif