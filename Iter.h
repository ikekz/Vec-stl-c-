#pragma once

template<typename T>
class Iter
{
public:
	T* pointer;

	Iter& operator++()
	{	
		++pointer;
		return *this;
	}

	Iter operator++(int)
	{
		Iter tmp = *this;
		++pointer;
		return tmp;
	}

	Iter& operator--()
	{
		--pointer;
		return *this;
	}

	Iter operator--(int)
	{
		Iter tmp = *this;
		--pointer;
		return tmp;
	}

	Iter operator+(size_t n) const
	{	
		return Iter(pointer + n);
	}

	Iter operator-(size_t n) const
	{	
		return Iter(pointer - n);
	}

	ptrdiff_t operator-(const Iter& src) const
	{
		return pointer - src.pointer;
	}

	T& operator*() const
	{	
		T* tmp = pointer; 
		return *tmp;
	}

	Iter(T* p) : pointer(p) {};
};

template<typename T> const
Iter<T> operator+(size_t n, const Iter<T>& src)
{
	return src + n;
}

template<typename T> const
bool operator==(const Iter<T>& src1, const Iter<T>& src2)
{
	return src1.pointer == src2.pointer;
}

template<typename T> const
bool operator!=(const Iter<T>& src1, const Iter<T>& src2)
{
	return !(src1.pointer == src2.pointer);
}

template<typename T> const
bool operator<(const Iter<T>& src1, const Iter<T>& src2)
{
	return src1.pointer < src2.pointer;
}

template<typename T> const
bool operator>(const Iter<T>& src1, const Iter<T>& src2)
{
	return src1.pointer > src2.pointer;
}

template<typename T> const
bool operator<=(const Iter<T>& src1, const Iter<T>& src2)
{
	return !(src1 > src2);
}

template<typename T> const
bool operator>=(const Iter<T>& src1, const Iter<T>& src2)
{
	return !(src1 < src2);
}