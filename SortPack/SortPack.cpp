#include "SortPack.h"

bool IntItem::operator<(const Item &item) const
{
	if (IntItem const* p = dynamic_cast<IntItem const*>(&item))
	{
		return value < p->value;
	}
	else return false;
}

bool IntItem::operator<(const IntItem &item) const
{
	return value < item.value;
}

bool IntItem::operator==(const Item & item) const
{
	if (IntItem const* p = dynamic_cast<IntItem const*>(&item))
		return value == p->value;
	else return false;
}

void IntItem::showItem()
{
	cout << value;
}

void ArrayOfInt::swap(int x, int y)
{
	if (x >= length || y >= length)
		exception("Out of range");
	if (x == y)
		return;
	IntItem tmp = arrayToSort[x];
	arrayToSort[x] = arrayToSort[y];
	arrayToSort[y] = tmp;
}

void ArrayOfInt::show()
{
	for (int i = 0; i < length; ++i) {
		arrayToSort[i].showItem();
		cout << " ";
	}
	cout << endl;
}

int SortPack::partition(int first, int last, Item & pivot)
{
	int l = first;
	int r = last;

	while (l + 1 < r) {
		if ((*items)[l] <= pivot)
			++l;
		else if (pivot < (*items)[r-1])
			--r;
		else {
			items->swap(l, r - 1);
			++l;
			--r;
		}
	}
	return l < r ? r : l;
}

void SortPack::split(int first, int last)
{
	if (first + 1 >= last)
		return;
	int pivotIndex = partition(first, last, (*items)[(last + first)/2]);
	if ((*items)[pivotIndex] < (*items)[(last + first) / 2] ||  (*items)[(last + first)] < (*items)[pivotIndex - 1] ) {
		int k = 0;
	}
	split(first, pivotIndex);
	split(pivotIndex, last);
}

void SortPack::bubble()
{
	for (int i = 0; i < quantity-1; ++i) {
		for (int j = i+1; j < quantity; ++j) {
			if ((*items)[j] < (*items)[i])
				items->swap(j, i);
		}
	}
}

void SortPack::select()
{
	for (int i = 0; i < quantity; ++i)
	{
		int minIndex = i;
		for (int j = i + 1; j < quantity; ++j) {
			if ((*items)[j] < (*items)[minIndex])
				minIndex = j;
		}
		items->swap(i, minIndex);
	}
}

void SortPack::insert()
{
	for (int i = 1; i < quantity; ++i) {
		int j = 0;
		while ((*items)[j] <= (*items)[i] && j < i)
			++j;
		items->swap(i, j);
		if (!((*items)[j] == (*items)[i]))
			--i;
	}
}

void SortPack::shell()
{
	for (int k = quantity / 2; k > 0; k /= 2)
		for (int i = k; i < quantity; ++i)
			for (int j = i; j >= k && (*items)[j] < (*items)[j - k]; j -= k)
				items->swap(j, j - k);
}

void SortPack::quick()
{
	split(0, quantity);
}

void SortPack::attach(ArrayOfItems &m_items, int m_quantity)
{
	items = &m_items;
	quantity = m_quantity;
}

void SortPack::show()
{
	items->show();
}

void SortPack::discardCounter()
{
}

void SortPack::increaseConter()
{
}
