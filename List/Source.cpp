#include "List.h"

int main() {
	List<int> lst;

	for (int i = 0; i < 10; ++i) {
		lst.push_front(i);
		lst.show();
	}
	for (int i = 0; i < 10; ++i) {
		lst.pop_back();
		lst.show();
	}
	cout << "sz: " << lst.size() << endl;
	for (int i = 0; i < 10; ++i) {
		lst.push_back(i);
		lst.show();
	}
	for (int i = 0; i < 10; ++i) {
		lst.pop_front();
		lst.show();
	}
	system("pause");
	return 0;
}