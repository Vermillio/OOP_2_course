#pragma once

#include "ListIterator.h"
#include <locale>
using namespace std::literals;

namespace MyContainer {
	template < class T, class Node >
	class NodeBase {
	public:
		T _elem;
		Node * _next;
		NodeBase(T &elem, Node * next) : _elem(elem), _next(next) {}
		void setNext(Node * next) {
			_next = next;
		}
	};

	template <class T, class Node>
	struct is_node_type :
		std::is_base_of<NodeBase<T, Node>, Node>
	{};

	template<class T, class Node>
//	typename std::enable_if<is_node_type<T, Node>::value>::type
	class ListBase {
		static_assert(is_node_type<T, Node>::value, "Node must be derived from NodeBase.");
	protected:
		using Nodep = Node * ;
		size_t _size;
		Nodep _front;
		int null_element;
		ListIterator<T, Node> _end = ListIterator<T, Node>(nullptr);
	public:
		ListIterator<T, Node> getIterator() {
			return ListIterator<T, Node>(_front);
		}
		ListIterator<T, Node>& end() { return _end; };
		size_t size() {
			return this->_size;
		}
		T& front() {
			if (!this->_front)
				return this->null_element;
			return this->_front->_elem;
		};
		bool is_null_element(T &elem) { return &elem == &this->null_element; };
		bool testIter();
	};

	template<class T, class Node>
	//	typename std::enable_if<is_node_type<T, Node>::value>
	bool MyContainer::ListBase<T, Node>::testIter()
	{
		try {
			int i = 10000; // prevent looping
			for (ListIterator<T, Node> iter = getIterator(); iter != end(); iter.next()) {
				auto val = iter.get();
				--i;
				if (i == 0)
					break;
			}
			return true;
		}
		catch (...) {
			return false;
		}
	}

}