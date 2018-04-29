#ifndef DOUBLE_LINKED_LIST_H
#define DOUBLE_LINKED_LIST_H
#include "ListBase.h"

namespace MyContainer {

	template<class T>
	class DoubleLinkedNode : public NodeBase<T, DoubleLinkedNode<T>> {
	public:
		DoubleLinkedNode<T> *_prev;
		DoubleLinkedNode(T &elem, DoubleLinkedNode<T>* prev, DoubleLinkedNode<T>* next) : NodeBase<T, DoubleLinkedNode<T>>(elem, next), _prev(prev) {
			if (prev)
				prev->setNext(this);
			if (next)
				next->_prev = this;
		}
	};

	template<class T>
	class DoubleLinkedList : public ListBase<T, DoubleLinkedNode<T>>
	{
		using Node = DoubleLinkedNode<T>;
		using Nodep = Node * ;
	protected:
		virtual void init(T &elem);
		Nodep _back;
	public:
		DoubleLinkedList() {
			this->_size = 0;
			this->_front = nullptr;
			this->_back = nullptr;
		};
		DoubleLinkedList(T &elem);
		~DoubleLinkedList() { clear(); };

		virtual void show();
		virtual void push_front(T &elem);
		virtual void push_back(T &elem);
		virtual void pop_front();
		virtual void pop_back();
		virtual void clear();
		T& back();
	};

	template<class T>
	T& MyContainer::DoubleLinkedList<T>::back()
	{
		if (!this->_back)
			return this->null_element;
		return this->_back->_elem;
	}

	template<class T>
	void DoubleLinkedList<T>::show()
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
	void DoubleLinkedList<T>::init(T &elem)
	{
		this->_back = this->_front = new Node(elem, nullptr, nullptr);
		this->_size = 1;
	}

	template<class T>
	void DoubleLinkedList<T>::clear()
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
	DoubleLinkedList<T>::DoubleLinkedList(T &elem) {
		init(elem);
	}

	template<class T>
	void DoubleLinkedList<T>::push_front(T &elem)
	{
		if (this->_size == 0) {
			init(elem);
			return;
		}
		this->_front = new Node(elem, nullptr, this->_front);
		++this->_size;
	}

	template<class T>
	void DoubleLinkedList<T>::push_back(T &elem)
	{
		if (this->_size == 0) {
			init(elem);
			return;
		}
		this->_back->_next = new Node(elem, this->_back, nullptr);
		this->_back = this->_back->_next;
		++this->_size;
	}

	template<class T>
	void DoubleLinkedList<T>::pop_front()
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
		this->_front->_prev = nullptr;
		delete p;
		--this->_size;
	}

	template<class T>
	void DoubleLinkedList<T>::pop_back()
	{
		if (this->_size == 0)
			return;
		if (this->_size == 1) {
			this->_front = nullptr;
			this->_back = nullptr;
			this->_size = 0;
			return;
		}
		Nodep p = this->_back;
		this->_back = this->_back->_prev;
		this->_back->_next = nullptr;
		delete p;
		--this->_size;
	}
}

#endif