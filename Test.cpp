#include <iostream>
#include "Vec.h"
#include "Alloc.h"
#include "Iter.h"
#include <gtest/gtest.h>
#include <vector>

using namespace std;

class TestIter : public ::testing::Test
{
public:
	Vec<int> v1;
	vector<int> v2;
	void SetUp(int size)
	{
		for (int i = 1; i <= size; i++)
		{
			v1.push_back(i);
			v2.push_back(i);
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
	Vec<int> v1;
	vector<int> v2;
	//const Vec<int> cv1 = const Vec<int>(10, 3);
	//const vector<int> cv2;

	void SetUp(int size)
	{
		v1.reserve(size);
		v2.reserve(size);
		for (int i = 1; i <= size; i++)
		{
			v1.push_back(i);
			v2.push_back(i);
		}
	}

	void Compare(Vec<int> v1, vector<int> v2)
	{
		ASSERT_EQ(v1.size(), v2.size());
		for (int i = 0; i < v1.size(); i++)
			ASSERT_EQ(v1[i], v2[i]);
	}
};

TEST_F(TestVec, default_constructor)
{
	Compare(Vec<int>(), vector<int>());
	Compare(Vec<int>(Alloc<int>()), vector<int>(allocator<int>()));
}

TEST_F(TestVec, fill_constructor)
{
	Compare(Vec<int>(10), vector<int>(10));
	Compare(Vec<int>(3, 4), vector<int>(3, 4));
}

TEST_F(TestVec, copy_constructor)
{
	Compare(Vec<int>(Vec<int>(10)), vector<int>(vector<int>(10)));
}

TEST_F(TestVec, basic_assignment)
{
	SetUp(10);
	v1 = Vec<int>(25, 3);
	v2 = vector<int>(25, 3);
	Compare(v1, v2);
}

TEST_F(TestVec, begin)
{
	SetUp(10);
	ASSERT_EQ(*v1.begin(), v1[0]);
	ASSERT_EQ(*v1.begin(), *v2.begin());
}

TEST_F(TestVec, end)
{
	SetUp(10);
	ASSERT_EQ(*(v1.end() - 1), v1[9]);
	ASSERT_EQ(*(v1.end() - 1), *(v2.end() - 1));
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
	v1.resize(11, 3);
	v2.resize(11, 3);
	Compare(v1, v2);
	v1.resize(2, 2);
	v2.resize(2, 2);
	Compare(v1, v2);
}

TEST_F(TestVec, capacity)
{
	SetUp(10);
	ASSERT_EQ(v1.capacity(), 10);
	v1.push_back(1);
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
}

TEST_F(TestVec, front)
{
	SetUp(10);
	ASSERT_EQ(v1.front(), v2.front());
}

TEST_F(TestVec, back)
{
	SetUp(10);
	ASSERT_EQ(v1.back(), v2.back());
}

TEST_F(TestVec, data)
{
	SetUp(10);
	ASSERT_EQ(v1.data() + 3, v1.data() + 3);
}

TEST_F(TestVec, push_back)
{
	SetUp(10);
	v1.push_back(66);
	v2.push_back(66);
	Compare(v1, v2);
	v1.resize(0);
	v2.resize(0);
	v1.push_back(66);
	v2.push_back(66);
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
	v1.insert(v1.begin() + 4, 88);
	v2.insert(v2.begin() + 4, 88);
	Compare(v1, v2);
	v1.resize(0);
	v2.resize(0);
	v1.insert(v1.begin(), 88);
	v2.insert(v2.begin(), 88);
	Compare(v1, v2);
	//ASSERT_EQ(*(v1.insert(v1.begin(), 16)), *(v2.insert(v2.begin(), 16)));
	v1.insert(v1.begin() + 1, 10, 17);
	v2.insert(v2.begin() + 1, 10, 17);
	Compare(v1, v2);
	v1.insert(v1.end() - 1, 1000, 52);
	v2.insert(v2.end() - 1, 1000, 52);
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

int main(int argc, char* argv[])
{	
	{
		Vec<int> v1;
		v1.insert(v1.begin(), 88);
		v1.insert(v1.begin() + 1, 2, 17);
		v1.insert(v1.begin() + 2, 2, 6);
		v1.insert(v1.begin() + 3, 2, 9);
		for (int i = 0; i < v1.size(); i++)
			cout << v1[i] << " ";
		cout << endl;
		v1.erase(v1.begin() + 2, v1.begin() + 3);
		for (int i = 0; i < v1.size(); i++)
			cout << v1[i] << " ";
		cout << endl;
		cout << v1.size() << endl;
	}
	/*{
		vector<int> a;
		for (int i = 1; i < 11; i++)
			a.push_back(i);
		a.insert(a.begin() + 2, 8, 5);
		cout << a[0] << endl;
		for (int i = 0; i < a.size(); i++)
			cout << a[i] << " ";
		cout << endl;
		{
			Vec<int> a;
			for (int i = 1; i < 11; i++)
				a.push_back(i);
			a.insert(a.begin(), 8, 5);
			cout << a[0] << endl;
			for (int i = 0; i < a.size(); i++)
				cout << a[i] << " ";
			cout << endl;
		}
		//Vec<int> b;
		//b.insert(b.begin(), 2);
		//cout << b[0] << endl;
	}
	/*{
		//vector<int> v("sss", "sssss");
		////cout << v.size();
		Vec<int> d(10, 10);
	}
	{
		allocator<int> gggg;
		Alloc<string> as;
		Vec<string> d1;
		//Vec<string> d2(as);
		Vec<string> f1(10);
		Vec<string> f2(15, "55");
		Vec<string> e(f2);

		//cout << f2.size() << " " << f2.capacity() << endl;
		//cout << e.size() << " " << e.capacity() << endl;
		f2[5] = "qqq";
		//cout << f2[5] << " " << e[5] << endl;
		e.push_back("aa");
		//cout << f2.size() << " " << f2.capacity() << endl;
		//cout << e.size() << " " << e.capacity() << endl;

		f2.shrink_to_fit();
		//cout << f1.max_size() << " " << f2.capacity() << endl;
		f2.push_back("qwer");
		f2.push_back("rewq");
		//cout << f2.back() << " " << f2.front() << endl;;
		//f2.clear();
		//f2.resize(0);
		//cout << f2[0] << endl;
		//cout << f2.size() << " " << f2.capacity() << endl;
		f2.pop_back();
		f2.resize(10, "hh");
		f2.resize(20);
		f2.shrink_to_fit();
		//cout << f2.size() << " " << f2.at(15) << " " << f2.capacity() << endl;
		////cout << f1.size() << " " << f1[8] << " " << f1.capacity() << endl;
		d1.reserve(14);
		d1.shrink_to_fit();
		//cout << d1.size() << " " << d1.capacity() << " " << endl;
	}
	//cout << endl;
	{
		//vector<string> b("222", "555");
		vector<string> a(15);
		vector<string> b(10, "h"); // check on max_size in my Vec
		//cout << b.max_size() << endl;
		//b = a;
		//a.resize(0);
		a.reserve(5);
		string e = "qwer";
		a.push_back(e);
		e = "arnold";
		a.resize(18);
		a.shrink_to_fit();
		//cout << a.size() << " " << a[8] << " " << a.capacity() << endl;
	}
	//cout << endl;
	{
		Vec<int> v;
		vector<int> vec;
		for (int i = 1; i < 10; i++)
			vec.push_back(i);
		auto a = vec.begin();
		//cout << a[3] << endl;
		//cout << *(a++) << " " << *(a++) << " " << *(a) << " " << *(++a) << " " << *a << endl;
		++a;
		a++;
		//cout << *a << endl;
		for (int i = 1; i < 10; i++)
			v.push_back(i);
		
		auto it = v.begin();
		//cout << v[0] << v[1] << v[2] << v[3] << endl;
		//cout << *(it++) << " " << *(it++) << " " << *(it) << " " << *(++it) << " " << *it << endl;
		++it;
		it++;
		//cout << *it << endl;
	}
	vector<int> a(10);
	cout << a.size() << endl;
	a.clear();
	cout << a.size() << " " << a.capacity() << endl;*/
	::testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();
	return 0;
}
