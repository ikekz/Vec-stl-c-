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
	Vec(const Alloc& alloc = Alloc())
	{
		this->alloc = alloc;
	}

	Vec(size_t size, const Alloc& alloc = Alloc())
	{
		this->alloc = alloc;
		Create(size, T());
	}

	Vec(size_t size, const T& val, const Alloc& alloc = Alloc())
	{
		this->alloc = alloc;
		Create(size, val);
	}

	Vec(const Vec& src)
	{
		this->alloc = src.get_allocator();
		Allocate(src.capacity());
		for (int i = 0; i < src.size(); i++)
			push_back(src[i]);
	}

	Vec(std::initializer_list<T> il, Alloc& alloc = Alloc())
	{
		*this = Vec(il.begin(), il.end());
	}
	
	template<typename It>
	Vec(It first, It last, Alloc& alloc = Alloc())
	{
		this->alloc = alloc;
		Construct(val<is_iter<It>::value>(), first, last);
	}

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
			Reallocate(size);
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

	T* data()
	{
		return first;
	}

	const T* data() const
	{
		return first;
	}

	void push_back(const T& val)
	{
		if (size() == capacity())
			if (size() == 0)
				Allocate(1);
			else
				Reallocate(2 * size());

		alloc.construct(last, val);
		last++;
	}

	template <class It>
	void assign(It first, It last)
	{
		clear();
		shrink_to_fit(); // nado li?
		Construct(val<is_iter<It>::value>(), first, last);
	}

	void assign(size_t size, const T& val)
	{
		clear();
		shrink_to_fit(); // nado li?
		Create(size, T());
	}

	void assign(std::initializer_list<T> il)
	{
		assign(il.begin(), il.end());
	}

	void push_back(T&& val)
	{
		if (size() == capacity())
			if (size() == 0)
				Allocate(1);
			else
				Reallocate(2 * size());

		alloc.construct(last, val);
		last++;
	}

	void pop_back()
	{
		alloc.destroy(last - 1);
		last--;
	}

	Iter insert(const Iter position, const T& val)
	{
		ptrdiff_t range = position - begin();
		push_back(val);
		for (Iter it = end() - 1; it != begin() + range; it--)
			SwapElement(it, it - 1);
		return begin() + range; 
	}

	Iter insert(const Iter position, size_t n, const T& va)
	{
		return Insert(val<false>(), position, n, va);
	}

	Iter insert(const Iter position, std::initializer_list<T> il)
	{
		return insert(position, il.begin(), il.end());
	}

	template <class It>
	Iter insert(const Iter position, It first, It last)
	{
		return Insert(val<is_iter<It>::value>(), position, first, last);
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
		ptrdiff_t count = last - first;
        ptrdiff_t s = end() - first - count;

		Iter it = first;
		for (size_t i = 0; i < s; i++)
		{
			alloc.destroy(it.pointer);
			alloc.construct(it.pointer, *(it + count));
			it++;
		}
		resize(size() - count);
		return first;
	}
    
    void swap(Vec& v)
    {
		T* tfirst = v.first;
		T* tlast = v.last;
		T* tmlast = v.mlast;
		Alloc talloc = v.alloc;

		v.first = first;
		v.last = last;
		v.mlast = mlast;
		v.alloc = alloc;

		first = tfirst;
		last = tlast;
		mlast = tmlast;
		alloc = talloc;
    }
    
	void clear()
	{
		resize(0);
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
    T* first;
    T* last = first;
    T* mlast = first;
    Alloc alloc;
     
	void Create(size_t size, const T& val)
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
		alloc.destroy(src1.pointer);
		alloc.construct(src1.pointer, *src2);
		alloc.destroy(src2.pointer);
		alloc.construct(src2.pointer, tmp);
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

	template<typename E>
	struct is_iter
	{
	private:
		static int check(...);
		static Iter check(T*);
		static Iter check(Iter);
	public:
		static const bool value = std::is_same<int, decltype(check(E()))>::value;
	};

	template<bool E>
	struct val {};

	template<typename E>
	void Construct(val<true>, E first, E last)
	{
		for (E it = first; it != last; it++)
			push_back(*it);
	}

	template<typename E>
	void Construct(val<false>, E size, E val)
	{
		Create(size, val);
	}

	Iter Move(size_t n, size_t range)
	{
		if (size() + n > capacity())
		{
			size_t tsize = size();
			while (tsize < size() + n)
				tsize *= 2;
			Reallocate(tsize);
		}
		resize(size() + n);

		Iter it = end();
		for (int i = 0; i <= range; i++)
		{
			alloc.destroy(it.pointer);
			alloc.construct(it.pointer, *(--it - n));
		}
		return it;
	}

	template<typename E>
	Iter Insert(val<true>, const Iter position, E first, E last)
	{
		ptrdiff_t rrange = end() - position;
		ptrdiff_t lrange = position - begin() - 1;
		Iter it = Move(last - first, rrange);

		for (; it != begin() + lrange; it--)
		{
			alloc.destroy(it.pointer);
			alloc.construct(it.pointer, *(--last));
		}

		return begin() + lrange;
	}

	template<typename E>
	Iter Insert(val<false>, const Iter position, E n, E val)
	{
		ptrdiff_t rrange = end() - position;
		ptrdiff_t lrange = position - begin() - 1;
		Iter it = Move(n, rrange);

		for (; it != begin() + lrange; it--)
		{
			alloc.destroy(it.pointer);
			alloc.construct(it.pointer, val);
		}

		return begin() + lrange;
	}
};