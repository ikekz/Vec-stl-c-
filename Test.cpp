#include <iostream>
#include "Vec.h"
#include "Alloc.h"
#include "Iter.h"
#include <gtest/gtest.h>
#include <vector>
#include <set>

using namespace std;

class TestIter : public ::testing::Test
{
public:
	Vec<string> v1;
	vector<string> v2;
	void SetUp(int size)
	{
		for (int i = 1; i <= size; i++)
		{
			char tmp1 = rand();
			string tmp = (char*)tmp1;
			v1.push_back(tmp);
			v2.push_back(tmp);
		}
	}
};

TEST_F(TestIter, prefix_increment)
{
	SetUp(10);
	auto it1 = v1.begin();
	auto it2 = v2.begin();
	for (int i = 0; i < 2; i++)
	{
		++it1;
		++it2;
	}

	ASSERT_EQ(*it1, *it2);
	ASSERT_EQ(*it1, v1[2]);
	ASSERT_EQ(*(++it1), *(++it2));
}

TEST_F(TestIter, postfix_increment)
{
	SetUp(10);
	auto it1 = v1.begin();
	auto it2 = v2.begin();
	for (int i = 0; i < 2; i++)
	{
		it1++;
		it2++;
	}

	ASSERT_EQ(*it1, *it2);
	ASSERT_EQ(*it1, v1[2]);
	ASSERT_EQ(*(it1++), *(it2++));
}

TEST_F(TestIter, equal_to)
{
	SetUp(10);
	auto it1 = v1.begin();
	auto it2 = v2.begin();
	ASSERT_EQ(it1 == v1.begin(), it2 == v2.begin());
}

TEST_F(TestIter, not_equal_to)
{
	SetUp(10);
	auto it1 = v1.begin() + 3;
	auto it2 = v2.begin() + 3;
	ASSERT_EQ(it1 != v1.begin(), it2 != v2.begin());
}

TEST_F(TestIter, indirection)
{
	SetUp(10);
	auto it1 = v1.begin() + 3;
	ASSERT_EQ(*it1, v1[3]);
}

TEST_F(TestIter, prefix_decrement)
{
	SetUp(10);
	auto it1 = v1.end();
	auto it2 = v2.end();
	for (int i = 0; i < 2; i++)
	{
		--it1;
		--it2;
	}

	ASSERT_EQ(*it1, *it2);
	ASSERT_EQ(*it1, v1[8]);
	ASSERT_EQ(*(--it1), *(--it2));
}

TEST_F(TestIter, postfix_decrement)
{
	SetUp(10);
	auto it1 = v1.end();
	auto it2 = v2.end();
	for (int i = 0; i < 2; i++)
	{
		it1--;
		it2--;
	}

	ASSERT_EQ(*it1, *it2);
	ASSERT_EQ(*it1, v1[8]);
	ASSERT_EQ(*(it1--), *(it2--));
}

TEST_F(TestIter, addition)
{
	SetUp(10);
	auto it1 = v1.begin() + 4;
	auto it2 = v2.begin() + 4;

	ASSERT_EQ(*it1, *it2);
	ASSERT_EQ(*it1, v1[4]);

	it1 = 4 + v1.begin();
	it2 = 4 + v2.begin();

	ASSERT_EQ(*it1, *it2);
	ASSERT_EQ(*it1, v1[4]);
}

TEST_F(TestIter, subtraction)
{
	SetUp(10);
	auto it1 = v1.end() - 4;
	auto it2 = v2.end() - 4;

	ASSERT_EQ(*it1, *it2);
	ASSERT_EQ(*it1, v1[6]);

	ASSERT_EQ(it1 - v1.end(), it2 - v2.end());
	ASSERT_EQ(v1.end() - it1, v2.end() - it2);
}

TEST_F(TestIter, comparisons)
{
	SetUp(10);
	auto it1 = v1.begin() + 4;
	auto it2 = v2.begin() + 4;

	ASSERT_EQ(it1 > v1.begin(), it2 > v2.begin());
	ASSERT_EQ(v1.begin() > it1, v2.begin() > it2);

	ASSERT_EQ(it1 < v1.begin(), it2 < v2.begin());
	ASSERT_EQ(v1.begin() < it1, v2.begin() < it2);

	ASSERT_EQ(it1 <= v1.begin(), it2 <= v2.begin());
	ASSERT_EQ(v1.begin() <= it1, v2.begin() <= it2);
	ASSERT_EQ(v1.begin() <= v1.begin(), v2.begin() <= v2.begin());

	ASSERT_EQ(it1 >= v1.begin(), it2 >= v2.begin());
	ASSERT_EQ(v1.begin() >= it1, v2.begin() >= it2);
	ASSERT_EQ(v1.begin() >= v1.begin(), v2.begin() >= v2.begin());
}

class TestVec : public ::testing::Test 
{
protected:
	Vec<string> v1;
	vector<string> v2;
	const Vec<string> cv1 = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	const vector<string> cv2 = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };

	void SetUp(int size)
	{
		v1.reserve(size);
		v2.reserve(size);
		for (int i = 1; i <= size; i++)
		{
			char tmp1 = rand();
			string tmp = (char*)tmp1;
			v1.push_back(tmp);
			v2.push_back(tmp);
		}
	}

	void Compare(Vec<string> v1, vector<string> v2)
	{
		ASSERT_EQ(v1.size(), v2.size());
		for (int i = 0; i < v1.size(); i++)
			ASSERT_EQ(v1[i], v2[i]);
	}
};

TEST_F(TestVec, default_constructor)
{
	Compare(Vec<string>(), vector<string>());
	Compare(Vec<string>(Alloc<string>()), vector<string>(allocator<string>()));
}

TEST_F(TestVec, fill_constructor)
{
	Compare(Vec<string>(10), vector<string>(10));
	Compare(Vec<string>(3, "4"), vector<string>(3, "4"));
}

TEST_F(TestVec, copy_constructor)
{
	Compare(Vec<string>(Vec<string>(10)), vector<string>(vector<string>(10)));
}

TEST_F(TestVec, range_constructor)
{
	vector<string> a2 = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	Vec<string> a1(a2.begin(), a2.end());
	Compare(a1, a2);
}

TEST_F(TestVec, initializer_list_constructor)
{
	Vec<string> a1 = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	vector<string> a2 = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	Compare(a1, a2);
}

TEST_F(TestVec, basic_assignment)
{
	SetUp(10);
	v1 = Vec<string>(25, "3");
	v2 = vector<string>(25, "3");
	Compare(v1, v2);
}

TEST_F(TestVec, begin)
{
	SetUp(10);
	ASSERT_EQ(*v1.begin(), v1[0]);
	ASSERT_EQ(*v1.begin(), *v2.begin());
	ASSERT_EQ(*cv1.begin(), cv1[0]);
	ASSERT_EQ(*cv1.begin(), *cv2.begin());
}

TEST_F(TestVec, end)
{
	SetUp(10);
	ASSERT_EQ(*(v1.end() - 1), v1[9]);
	ASSERT_EQ(*(v1.end() - 1), *(v2.end() - 1));
	ASSERT_EQ(*(cv1.end() - 1), cv1[9]);
	ASSERT_EQ(*(cv1.end() - 1), *(cv2.end() - 1));
}

TEST_F(TestVec, cbegin)
{
	SetUp(10);
	ASSERT_EQ(*v1.cbegin(), v1[0]);
	ASSERT_EQ(*v1.cbegin(), *v2.cbegin());
}

TEST_F(TestVec, cend)
{
	SetUp(10);
	ASSERT_EQ(*(v1.cend() - 1), v1[9]);
	ASSERT_EQ(*(v1.cend() - 1), *(v2.cend() - 1));
}

TEST_F(TestVec, size)
{
	SetUp(10);
	ASSERT_EQ(v1.size(), 10);
	ASSERT_EQ(v1.size(), v2.size());
}

TEST_F(TestVec, max_size)
{
	SetUp(10);
	ASSERT_EQ(v1.max_size(), v2.max_size());
}

TEST_F(TestVec, resize)
{
	SetUp(10);
	v1.resize(20);
	v2.resize(20);
	Compare(v1, v2);
	v1.resize(5);
	v2.resize(5);
	Compare(v1, v2);
	v1.resize(11, "3");
	v2.resize(11, "3");
	Compare(v1, v2);
	v1.resize(2, "2");
	v2.resize(2, "2");
	Compare(v1, v2);
}

TEST_F(TestVec, capacity)
{
	SetUp(10);
	ASSERT_EQ(v1.capacity(), 10);
	v1.push_back("1");
	ASSERT_EQ(v1.capacity(), 20);
}
//EXPECT_THROW();
TEST_F(TestVec, empty)
{
	SetUp(10);
	ASSERT_EQ(v1.empty(), v2.empty());
	v1.resize(0);
	v2.resize(0);
	ASSERT_EQ(v1.empty(), v2.empty());
}

TEST_F(TestVec, reserve)
{
	SetUp(10);
	ASSERT_EQ(v1.capacity(), 10);
	v1.reserve(15);
	ASSERT_EQ(v1.capacity(), 15);
	v1.reserve(9);
	ASSERT_EQ(v1.capacity(), 15);
}

TEST_F(TestVec, shrink_to_fit)
{
	SetUp(10);
	ASSERT_EQ(v1.capacity(), 10);
	v1.reserve(15);
	v2.reserve(15);
	v1.shrink_to_fit();
	v2.shrink_to_fit();
	ASSERT_EQ(v1.capacity(), 10);
	ASSERT_EQ(v1.capacity(), v2.capacity());
}

TEST_F(TestVec, subscript)
{
	SetUp(10);
	ASSERT_EQ(v1[4], v2[4]);
}

TEST_F(TestVec, at)
{
	SetUp(10);
	ASSERT_EQ(v1.at(4), v2.at(4));
	ASSERT_EQ(cv1.at(4), cv2.at(4));
}

TEST_F(TestVec, front)
{
	SetUp(10);
	ASSERT_EQ(v1.front(), v2.front());
	ASSERT_EQ(cv1.front(), cv2.front());
}

TEST_F(TestVec, back)
{
	SetUp(10);
	ASSERT_EQ(v1.back(), v2.back());
	ASSERT_EQ(cv1.back(), cv2.back());
}

TEST_F(TestVec, data)
{
	SetUp(10);
	ASSERT_EQ(v1.data() + 3, v1.data() + 3);
	ASSERT_EQ(cv1.data() + 3, cv1.data() + 3);
}

TEST_F(TestVec, range_assign)
{
	SetUp(10);
	vector<string> t = { "11", "22", "33", "44", "55", "66", "77" };
	v1.assign(t.begin(), t.end());
	v2.assign(t.begin(), t.end());
	Compare(v1, v2);
}

TEST_F(TestVec, fill_assign)
{
	SetUp(10);
	v1.assign(5, "7");
	v2.assign(5, "7");
	Compare(v1, v2);
}

TEST_F(TestVec, initializer_list_assign)
{
	SetUp(10);
	v1.assign({ "11", "22", "33", "44", "55", "66", "77" });
	v2.assign({ "11", "22", "33", "44", "55", "66", "77" });
	Compare(v1, v2);
}

TEST_F(TestVec, push_back)
{
	SetUp(10);
	v1.push_back("66");
	v2.push_back("66");
	Compare(v1, v2);
	v1.resize(0);
	v2.resize(0);
	v1.push_back("66");
	v2.push_back("66");
	Compare(v1, v2);
}

TEST_F(TestVec, pop_back)
{
	SetUp(10);
	v1.pop_back();
	v2.pop_back();
	Compare(v1, v2);
}

TEST_F(TestVec, insert)
{
	SetUp(10);
	v1.insert(v1.begin() + 4, "88");
	v2.insert(v2.begin() + 4, "88");
	Compare(v1, v2);
	v1.resize(0);
	v2.resize(0);
	v1.insert(v1.begin(), "88");
	v2.insert(v2.begin(), "88");
	Compare(v1, v2);
	//ASSERT_EQ(v1.insert(v1.begin() + 1, "16") - v1.begin(), v2.insert(v2.begin() + 1, "16") - v2.begin());
	v1.insert(v1.begin() + 1, 10, "17");
	v2.insert(v2.begin() + 1, 10, "17");
	Compare(v1, v2);
	//v2.insert(v2.begin() + 2, 7, 5);
	//cout << v1.size() << " " << v2.size();
	//ptrdiff_t a = v2.insert(v2.begin() + 2, 7, 5) - v2.end() + 5;
	//cout << v1.size() << " " << v2.size();
	//ASSERT_EQ(v1.insert(v1.begin() + 2, 7, 5) - v1.begin(), v2.insert(v2.begin() + 2, 7, 5) - v2.begin());
	v1.insert(v1.end() - 1, 10, "52");
	v2.insert(v2.end() - 1, 10, "52");
	Compare(v1, v2);
	vector<string> t = { "-1", "-2", "-3" ,"-4", "-5", "-6" };
	v1.insert(v1.begin() + 2, t.begin() + 1, t.end() - 2);
	v2.insert(v2.begin() + 2, t.begin() + 1, t.end() - 2);
	Compare(v1, v2);
	v1.insert(v1.begin() + 6, { "8", "90", "86", "13" });
	v2.insert(v2.begin() + 6, { "8", "90", "86", "13" });
	//for (string i = 0; i < v1.size(); i++)
	//	cout << v1[i] << " ";
	//cout << endl;
	//for (string i = 0; i < v2.size(); i++)
	//	cout << v2[i] << " ";
	Compare(v1, v2);
	//ASSERT_EQ(*(v1.insert(v1.begin() + 18, 7)), *(v2.insert(v2.begin() + 18, 7)));
}

TEST_F(TestVec, erase)
{
	SetUp(10);
	v1.erase(v1.begin() + 6);
	v2.erase(v2.begin() + 6);
	Compare(v1, v2);
	v1.erase(v1.begin() + 2, v1.begin() + 6);
	v2.erase(v2.begin() + 2, v2.begin() + 6);
	Compare(v1, v2);
}

TEST_F(TestVec, swap)
{
	Vec<string> a1 = { "1", "2", "3"};
	Vec<string> a2 = {"5", "6", "7", "8", "9", "10"};
	Vec<string> ta1 = a1;
	Vec<string> ta2 = a2;
	a1.swap(a2);
	ASSERT_EQ(a1.size(), ta2.size());
	ASSERT_EQ(a1.capacity(), ta2.capacity());
	for (int i = 0; i < a1.size(); i++)
		ASSERT_EQ(a1[i], ta2[i]);

	ASSERT_EQ(a2.size(), ta1.size());
	ASSERT_EQ(a2.capacity(), ta1.capacity());
	for (int i = 0; i < a2.size(); i++)
		ASSERT_EQ(a2[i], ta1[i]);
}

int main(int argc, char* argv[])
{	
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
	return 0;
}
