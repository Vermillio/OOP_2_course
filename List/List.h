#include <iostream>

using namespace std;

template<class T>
class ListIterator;

template<class T>
class List
{
protected:
	struct Node;
	using Nodep = Node*;
	size_t _size;
	friend class ListIterator<T>;
	Nodep _front;
	Nodep _back;
	int null_element;

	void init(T &elem);
public:
	virtual struct Node {
		T _elem;
		Nodep _next;
		Node(T &elem, Nodep next) : _elem(elem), _next(next) {}
	};

//	class BadList{};
	List() : _size(0), _front(nullptr), _back(nullptr) {};
	List(T &elem);
	~List();
	bool is_null_element(T &elem);
	virtual void push_front(T &elem);
	virtual void push_back(T &elem);
	virtual void pop_front();
	virtual void pop_back();
	virtual T& front();
	virtual T& back();
//	virtual ListIterator iterate();
	virtual void show();
	size_t size();
	virtual void clear();
};

template<class T>
void List<T>::init(T &elem)
{
	_back = _front = new Node(elem, nullptr);
	_size = 1;
}

template<class T>
bool List<T>::is_null_element(T &elem)
{
	return &elem == &null_element;
}

template<class T>
size_t List<T>::size()
{
	return _size;
}

template<class T>
void List<T>::show()
{
	if (_size == 0) {
		cout << "[empty list]" << endl;
		return;
	}
	Nodep p = _front;
	while (p != _back) {
		cout << p->_elem << " ";
		p = p->_next;
	}
	cout << p->_elem << endl;
}

template<class T>
T& List<T>::back()
{
	if (!_back)
		return null_element;
	return _back->_elem;
}

template<class T>
T& List<T>::front()
{
	if (!_front)
		return null_element;
	return _front->_elem;
}

template<class T>
void List<T>::clear()
{
	if (_size == 0)
		return;
	if (_size == 1) {
		delete _front;
		_front = nullptr;
		_back = nullptr;
		_size = 0;
		return;
	}
	Nodep p = _front;
	Nodep q = _front->_next;
	while (q != _back) {
		delete p;
		p = q;
		q = q->_next;
	}
	delete q;
	_front = _back = nullptr;
	_size = 0;
}

template<class T>
List<T>::~List()
{
	clear();
}

template<class T>
void List<T>::pop_back()
{
	if (_size == 0)
		return;
	if (_size == 1) {
		_front = nullptr;
		_back = nullptr;
		_size = 0;
		return;
	}
	Nodep p = _front;
	while (p->_next->_next)
		p = p->_next;
	delete p->_next;
	p->_next = nullptr;
	_back = p;
	--_size;
}

template<class T>
List<T>::List(T &elem) {
	init(elem);
}


template<class T>
void List<T>::pop_front()
{
	if (_size==0)
		return;
	if (_size == 1) {
		_front = _back = nullptr;
		_size = 0;
		return;
	}
	Nodep p = _front;
	_front = _front->_next;
	delete p;
	--_size;
}

template<class T>
void List<T>::push_back(T &elem)
{
	if (_size == 0) {
		init(elem);
		return;
	}
	_back->_next = new Node(elem, nullptr);
	_back = _back->_next;
	++_size;
}

template<class T>
void List<T>::push_front(T &elem)
{
	if (_size == 0) {
		init(elem);
		return;
	}
	_front = new Node(elem, _front);
	++_size;
}