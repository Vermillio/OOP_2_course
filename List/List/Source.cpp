//#include "DoubleLinkedList.h"
#include "CyclicList.h"
#include "CyclicDoubleLinkedList.h"
#include "List.h"

//using namespace std;
using namespace MyContainer;

//template<class T, class = typename std::enable_if<std::is_base_of<ListBase, T>::value>::type>
//void testList(T &lst) {
//	for (int i = 0; i < 10; ++i) {
//		lst.push_front(i);
//		lst.show();
//	}
//	for (int i = 0; i < 10; ++i) {
//		lst.pop_back();
//		lst.show();
//	}
//	cout << "sz: " << lst.size() << endl;
//	for (int i = 0; i < 10; ++i) {
//		lst.push_back(i);
//		lst.show();
//	}
//	for (int i = 0; i < 10; ++i) {
//		lst.pop_front();
//		lst.show();
//	}
//}


int main() {
	int arr[] = { 9, 8, 7, 6, 5 };
	CyclicList<int> lst;
	for (auto i : arr)
		lst.push_back(i);
	if (lst.testIter())
		cout << "Successfully tested iterator. " << endl;
	else cout << "Error during testing iterator." << endl;
	lst.clear();
	system("pause");
	return 0;
}