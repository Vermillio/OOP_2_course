#ifndef LIST_ITERATOR_H
#define LIST_ITERATOR_H
#include <type_traits>


namespace MyContainer {

	template <class T, class Node>
	struct is_node_type;

	template<class T, class Node>
	class ListIterator {
		static_assert(is_node_type<T, Node>::value, "Node must be derived from NodeBase.");
		Node * current;
	public:
		ListIterator(Node * first) : current(first) {}
		bool operator== (ListIterator<T, Node> &it) { return current == it.current; }
		bool operator!= (ListIterator<T, Node> &it) { return current != it.current; }
		bool hasNext() {
				if (current)
					return true;
				else return false;
			}
		void next() {
				if (hasNext())
					current = current->_next;
			}
		T& get() {
				return current->_elem;
			}
	};
};

#endif