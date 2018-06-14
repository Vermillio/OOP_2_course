#include "SortPack.h"
#include <ctime>
using namespace std;

int main() {
	int length = 200;
	ArrayOfInt arr(length);
	arr.fill();
	arr.show();
	cout << "Sorting..." << endl;
	SortPack sorter;
	cout << endl;
	sorter.attach(arr, length);
	time_t timer = clock();
	sorter.bubble();
	timer = clock() - timer;
	sorter.show();
	cout << "Bubble sort took " << timer/(double)1000 << " s." << endl;
	cout << endl;
//	arr.fill();
	sorter.attach(arr, length);
	timer = clock();
	sorter.select();
	timer = clock() - timer;
	sorter.show();
	cout << "Selection sort took " << timer / (double)1000 << " s." << endl;
	cout << endl;
//	arr.fill();
	sorter.attach(arr, length);
	timer = clock();
	sorter.insert();
	timer = clock() - timer;
	sorter.show();
	cout << "Insertion sort took " << timer / (double)1000 << " s." << endl;
	cout << endl;
//	arr.fill();
	sorter.attach(arr, length);
	timer = clock();
	sorter.shell();
	timer = clock() - timer;
	sorter.show();
	cout << "Shell sort took " << timer / (double)1000 << " s." << endl;
	cout << endl;
//	arr.fill();
	sorter.attach(arr, length);
	timer = clock();
	sorter.quick();
	timer = clock() - timer;
	sorter.show();
	cout << "Quick sort took " << timer / (double)1000 << " s." << endl;
	cout << endl;
	system("pause");
	return 0;
}