#pragma once
#include "Alloc.h"
#include "Iter.h"

template<typename T, typename Alloc = Alloc<T>>
class Vec
{
public:
	~Vec()
	{
		clear();
		if (capacity() > 0)
			alloc.deallocate(first, size());
	}

	typedef Iter<T> Iter;
	Vec(Alloc& alloc = Alloc())
	{
		this->alloc = alloc;
	}

	Vec(size_t size, Alloc& alloc = Alloc())
	{
		this->alloc = alloc;
		Construct(size, T());
	}

	Vec(size_t size, const T& val, Alloc& alloc = Alloc()) //wtf why int???
	{
		this->alloc = alloc;
		Construct(size, val);
	}

	Vec(const Vec& src)
	{
		this->alloc = src.get_allocator();
		Allocate(src.capacity());
		for (int i = 0; i < src.size(); i++)
			push_back(src[i]);
	}

	//Vec(std::initializer_list<T> il, Alloc& alloc = Alloc())
	//{
	//
	//}

	/*template<typename It>
	Vec(It first, It last, Alloc& alloc = Alloc())
	{
		//for (; first != last; ++first)
		//	push_back(*first);
	};*/

	Vec<T>& operator=(const Vec<T>& src)
	{	
		if (this != &src)
		{
			clear();
			alloc = src.alloc;
			reserve(src.capacity());
			for (int i = 0; i < src.size(); i++)
				push_back(src[i]);
		}
		return (*this);
	}

	size_t size() const
	{
		return last - first;
	}

	size_t max_size() const
	{
		return alloc.max_size();
	}

	void resize(size_t size)
	{
		if (this->size() > size)
			for (size_t i = this->size(); i != size; i--)
				pop_back();
		else if (this->size() < size)
			for (size_t i = this->size(); i != size; i++)
				push_back(T());
	}

	void resize(size_t size, const T& val)
	{
		if (this->size() > size)
			for (size_t i = this->size(); i != size; i--)
				pop_back();
		else if (this->size() < size)
			for (size_t i = this->size(); i != size; i++)
				push_back(val);
	}

	void shrink_to_fit()
	{
		if (capacity() > size())
			Reallocate(size());
	}
	
	size_t capacity() const
	{
		return mlast - first;
	}

	void reserve(size_t size)
	{
		if (size > capacity())
			Reallocate(size); //??????????????
	}

	bool empty() const
	{
		return size() == 0 ? 1 : 0;
	}

	T& operator[](size_t src)
	{
		return *(first + src);
	}

	const T& operator[](size_t src) const
	{
		return *(first + src);
	}

	T& at(size_t src)
	{
		return *(first + src);
	}

	const T& at(size_t src) const
	{
		return *(first + src);
	}

	T& front()
	{
		return *first;
	}

	const T& front() const
	{
		return *first;
	}

	T& back()
	{
		return *(last - 1);
	}

	const T& back() const
	{
		return *(last - 1);
	}

	T* data() /*noexept ?*/
	{
		return first;
	}

	const T* data() const /*noexept ?*/
	{
		return first;
	}

	void push_back(const T& val)
	{
		if (size() == capacity())
			if (size() == 0)
				Allocate(1);
			else
				Reallocate(2 * size()); //tyt gl91nyt'

		alloc.construct(last, val);
		last++;
	}

	void push_back(T&& val)
	{
		if (size() == capacity())
			if (size() == 0)
				Allocate(1);
			else
				Reallocate(2 * size()); //tyt gl91nyt'

		alloc.construct(last, val);
		last++;
	}

	void pop_back()
	{
		//if (empty())
		//	_DEBUG_ERROR("vector empty before pop");
		//else
		//{	
		alloc.destroy(last - 1);
		last--;
		//}
	}

	Iter insert(const Iter position, const T& val)
	{
		ptrdiff_t range = position - begin();
		push_back(val);
		for (Iter it = end() - 1; it != begin() + range; it--)
			SwapElement(it, it - 1);
		return begin() + range; //nepravilmno
	}

	Iter insert(const Iter position, size_t n, const T& val)
	{
		ptrdiff_t rrange = end() - position;
		ptrdiff_t lrange = position - begin() - 1;
		if (size() + n > capacity())
		{
			size_t tsize = size();
			while (tsize < size() + n)
				tsize *= 2;
			Reallocate(tsize);
		}
		resize(size() + n);

		Iter it = end();
		for (int i = 0; i <= rrange; i++)
		{
			//mojet destructor
			alloc.destroy(it.Pointer());
			alloc.construct(it.Pointer(), *(--it - n));
		}

		for (; it != begin() + lrange; it--)
		{
			alloc.destroy(it.Pointer());
			alloc.construct(it.Pointer(), val);
		}

		return begin() + lrange;//nepravilmno
	}

	Iter erase(const Iter position)
	{
		ptrdiff_t range = position - begin();
		for (Iter it = position; it != end() - 1; it++)
			SwapElement(it, it + 1);
		pop_back();
		return begin() + range;
	}

	Iter erase(const Iter first, const Iter last)
	{
		ptrdiff_t range = first - begin();
		ptrdiff_t count = last - first;

		Iter it = begin() + range;
		Iter it1 = end();
		for (size_t i = 0; i < count; i++)
		{
			alloc.destroy(it.Pointer());
			alloc.construct(it.Pointer(), *(--it1 - count));
		}
		resize(size() - count);
		return begin() + range;
	}

	void clear()
	{
		resize(0); // ili relocated ili erase po iteratory
	}
	
	Iter begin()
	{
		return Iter(first);
	}

	const Iter begin() const
	{
		return Iter(first);
	}

	Iter end()
	{
		return Iter(last);
	}

	const Iter end() const
	{
		return Iter(last);
	}

	const Iter cbegin()
	{
		return begin();
	}

	const Iter cend()
	{
		return end();
	}

	Alloc get_allocator() const
	{
		return alloc;
	}

private:
	void Construct(size_t size, const T& val)
	{
		Allocate(size);
		for (size_t i = 0; i < size; i++)
			alloc.construct(first + i, val);
		last = first + size;
	}

	void Allocate(size_t size)
	{
		size_t tsize = this->size();
		first = alloc.allocate(size);
		mlast = first + size;
		last = first + tsize;
	}

	void SwapElement(const Iter& src1, const Iter& src2) 
	{
		T tmp = *src1;
		alloc.destroy(src1.Pointer());
		alloc.construct(src1.Pointer(), *src2);
		alloc.destroy(src2.Pointer());
		alloc.construct(src2.Pointer(), tmp);
	}

	void Reallocate(size_t size)
	{
		T* tfirst = first;
		size_t tcapacity = capacity();
		Allocate(size);
		for (size_t i = 0; i < this->size(); i++)
			alloc.construct(first + i, *(tfirst + i));

		for (size_t i = 0; i < this->size(); i++)
			alloc.destroy(tfirst + i);

		if (tcapacity > 0)
			alloc.deallocate(tfirst, size);
	}

	T* first;
	T* last = first;
	T* mlast = first;
	Alloc alloc;
	
};