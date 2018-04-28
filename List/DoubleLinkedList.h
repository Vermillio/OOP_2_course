#pragma once

template<class T>
class DoubleLinkedList : virtual public List
{
public:
	virtual struct Node : public List::Node {
		Nodep prev;
		Node(T &elem, Nodep prev, Nodep next) : _elem(elem), _prev(prev), _next(next) {
			if (prev)
				prev->_next = this;
			if (next)
				next->prev = this;
		}
	};

	DoubleLinkedList(T &elem) :List(elem) {};
	virtual void push_front(T &elem);
	virtual void push_back(T &elem);
	virtual void pop_front();
	virtual void pop_back();
	virtual T& front();
	virtual T& back();
};

template<class T>
T& DoubleLinkedList<T>::back()
{
	if (_size == 0)
		return null_element;
	return _front->_elem;
}

template<class T>
T& DoubleLinkedList<T>::front()
{
	if (_size == 0)
		return null_element;
	return _back->_elem;
}

template<class T>
void DoubleLinkedList<T>::push_front(T &elem)
{
	if (_size == 0) {
		init(elem);
		return;
	}
	_front = new Node(elem, nullptr, _front);
	++_size;
}

template<class T>
void DoubleLinkedList<T>::push_back(T &elem)
{
	if (_size == 0) {
		init(elem);
		return;
	}
	_back->_next = new Node(elem, _back, nullptr);
	_back = _back->_next;
	++_size;
}

template<class T>
void DoubleLinkedList<T>::pop_front()
{
	if (_size == 0)
		return;
	if (_size == 1) {
		_front = _back = nullptr;
		_size = 0;
		return;
	}
	Nodep p = _front;
	_front = _front->_next;
	_front->prev = nullptr;
	delete p;
	--_size;
}

template<class T>
void DoubleLinkedList<T>::pop_back()
{
	if (_size == 0)
		return;
	if (_size == 1) {
		_front = nullptr;
		_back = nullptr;
		_size = 0;
		return;
	}
	Nodep p = _back;
	_back = _back->prev;
	_back->_next = nullptr;
	delete p;
	--_size;
}