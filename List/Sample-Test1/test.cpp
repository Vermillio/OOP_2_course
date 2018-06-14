#include "pch.h"
#include "../List/List.h"
#include "../List/ListIterator.h"
#include "../List/CyclicDoubleLinkedList.h"
#include "../List/CyclicList.h"
#include "../List/DoubleLinkedList.h"


namespace MyContainer {


	class ListTest : public ::testing::Test {

	protected:

		List<int> list;
		virtual void SetUp() {};
		virtual void TearDown() { list.clear(); };
		ListTest() {};
		virtual ~ListTest() {};

	public:
		void push_front(const int &elem) { list.push_front(elem); };
		void push_back(const int &elem) { list.push_back(elem); };
		void pop_front() { list.pop_front(); };
		void pop_back() { list.pop_back(); };
		void show() { list.show(); };
		void clear() { list.clear(); };
		int& back() { return list.back(); };
	};

	TEST_F(ListTest, IsClearWorking) {
		for (size_t i = 0; i < 5; ++i) {
			push_back(rand());
			push_front(rand());
		}

		EXPECT_FALSE(list.size() == 0);
		list.clear();
		EXPECT_EQ(list.size(), 0);
	};

	TEST_F(ListTest, IsPushBackWorking) {
		list.clear();
		int sz = 5;
		for (int i = 0; i < sz; ++i) {
			push_back(i);
		}
		EXPECT_EQ(list.size(), sz);
	};

	TEST_F(ListTest, IsPushFrontWorking) {
		list.clear();
		int sz = 5;
		for (int i = 0; i < sz; ++i) {
			push_front(i);
		}
		EXPECT_EQ(list.size(), sz);
	};

	TEST_F(ListTest, IsPopFrontWorking) {
		int sz = list.size();
		push_front(0);
		EXPECT_EQ(sz + 1, list.size());
		pop_front();
		EXPECT_EQ(sz, list.size());
	};

	TEST_F(ListTest, IsPopBackWorking) {
		int sz = list.size();
		push_back(0);
		EXPECT_EQ(sz + 1, list.size());
		pop_back();
		EXPECT_EQ(sz, list.size());
	};

	class CyclicListTest : public ::testing::Test {

	protected:

		CyclicList<int> list;
		virtual void SetUp() {};
		virtual void TearDown() { list.clear(); };
		CyclicListTest() {};
		virtual ~CyclicListTest() {};

	public:
		void push_front(const int &elem) { list.push_front(elem); };
		void push_back(const int &elem) { list.push_back(elem); };
		void pop_front() { list.pop_front(); };
		void pop_back() { list.pop_back(); };
		void show() { list.show(); };
		void clear() { list.clear(); };
		int& back() { return list.back(); };
	};

	TEST_F(CyclicListTest, IsClearWorking) {
		for (size_t i = 0; i < 5; ++i) {
			push_back(rand());
			push_front(rand());
		}

		EXPECT_FALSE(list.size() == 0);
		list.clear();
		EXPECT_EQ(list.size(), 0);
	};

	TEST_F(CyclicListTest, IsPushBackWorking) {
		list.clear();
		int sz = 5;
		for (int i = 0; i < sz; ++i) {
			push_back(i);
		}
		EXPECT_EQ(list.size(), sz);
	};

	TEST_F(CyclicListTest, IsPushFrontWorking) {
		list.clear();
		int sz = 5;
		for (int i = 0; i < sz; ++i) {
			push_front(i);
		}
		EXPECT_EQ(list.size(), sz);
	};

	TEST_F(CyclicListTest, IsPopFrontWorking) {
		int sz = list.size();
		push_front(0);
		EXPECT_EQ(sz + 1, list.size());
		pop_front();
		EXPECT_EQ(sz, list.size());
	};

	TEST_F(CyclicListTest, IsPopBackWorking) {
		int sz = list.size();
		push_back(0);
		EXPECT_EQ(sz + 1, list.size());
		pop_back();
		EXPECT_EQ(sz, list.size());
	};

	class DoubleLinkedListTest : public ::testing::Test {

	protected:

		DoubleLinkedList<int> list;
		virtual void SetUp() {};
		virtual void TearDown() { list.clear(); };
		DoubleLinkedListTest() {};
		virtual ~DoubleLinkedListTest() {};

	public:
		void push_front(const int &elem) { list.push_front(elem); };
		void push_back(const int &elem) { list.push_back(elem); };
		void pop_front() { list.pop_front(); };
		void pop_back() { list.pop_back(); };
		void show() { list.show(); };
		void clear() { list.clear(); };
		int& back() { return list.back(); };
	};

	TEST_F(DoubleLinkedListTest, IsClearWorking) {
		for (size_t i = 0; i < 5; ++i) {
			push_back(rand());
			push_front(rand());
		}

		EXPECT_FALSE(list.size() == 0);
		list.clear();
		EXPECT_EQ(list.size(), 0);
	};

	TEST_F(DoubleLinkedListTest, IsPushBackWorking) {
		list.clear();
		int sz = 5;
		for (int i = 0; i < sz; ++i) {
			push_back(i);
		}
		EXPECT_EQ(list.size(), sz);
	};

	TEST_F(DoubleLinkedListTest, IsPushFrontWorking) {
		list.clear();
		int sz = 5;
		for (int i = 0; i < sz; ++i) {
			push_front(i);
		}
		EXPECT_EQ(list.size(), sz);
	};

	TEST_F(DoubleLinkedListTest, IsPopFrontWorking) {
		int sz = list.size();
		push_front(0);
		EXPECT_EQ(sz + 1, list.size());
		pop_front();
		EXPECT_EQ(sz, list.size());
	};

	TEST_F(DoubleLinkedListTest, IsPopBackWorking) {
		int sz = list.size();
		push_back(0);
		EXPECT_EQ(sz + 1, list.size());
		pop_back();
		EXPECT_EQ(sz, list.size());
	};

	class CyclicDoubleLinkedListTest : public ::testing::Test {

	protected:

		CyclicDoubleLinkedList<int> list;
		virtual void SetUp() {};
		virtual void TearDown() { list.clear(); };
		CyclicDoubleLinkedListTest() {};
		virtual ~CyclicDoubleLinkedListTest() {};

	public:
		void push_front(const int &elem) { list.push_front(elem); };
		void push_back(const int &elem) { list.push_back(elem); };
		void pop_front() { list.pop_front(); };
		void pop_back() { list.pop_back(); };
		void show() { list.show(); };
		void clear() { list.clear(); };
		int& back() { return list.back(); };
	};

	TEST_F(CyclicDoubleLinkedListTest, IsClearWorking) {
		for (size_t i = 0; i < 5; ++i) {
			push_back(rand());
			push_front(rand());
		}

		EXPECT_FALSE(list.size() == 0);
		list.clear();
		EXPECT_EQ(list.size(), 0);
	};

	TEST_F(CyclicDoubleLinkedListTest, IsPushBackWorking) {
		list.clear();
		int sz = 5;
		for (int i = 0; i < sz; ++i) {
			push_back(i);
		}
		EXPECT_EQ(list.size(), sz);
	};

	TEST_F(CyclicDoubleLinkedListTest, IsPushFrontWorking) {
		list.clear();
		int sz = 5;
		for (int i = 0; i < sz; ++i) {
			push_front(i);
		}
		EXPECT_EQ(list.size(), sz);
	};

	TEST_F(CyclicDoubleLinkedListTest, IsPopFrontWorking) {
		int sz = list.size();
		push_front(0);
		EXPECT_EQ(sz + 1, list.size());
		pop_front();
		EXPECT_EQ(sz, list.size());
	};

	TEST_F(CyclicDoubleLinkedListTest, IsPopBackWorking) {
		int sz = list.size();
		push_back(0);
		EXPECT_EQ(sz + 1, list.size());
		pop_back();
		EXPECT_EQ(sz, list.size());
	};

}