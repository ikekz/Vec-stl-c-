#pragma once

template<typename T>
class Iter
{
public:
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
		return pointer - src.Pointer();
	}

	T& operator*() const
	{	
		T* tmp = pointer; //pochekat'
		return *tmp;
	}

	T* Pointer() const
	{
		return pointer;
	}

	Iter(T* p) : pointer(p) {};
private:
	T* pointer;
};

template<typename T> const
Iter<T> operator+(size_t n, const Iter<T>& src)
{
	return src + n;
}

//template<typename T> const
//Iter<T> operator-(const Iter<T>& src1, const Iter<T>& src2)
//{	// return difference of reverse_iterators
//	return (src1.Pointer() - src2.Pointer());
//}

template<typename T> const
bool operator==(const Iter<T>& src1, const Iter<T>& src2)
{
	return src1.Pointer() == src2.Pointer();
}

template<typename T> const
bool operator!=(const Iter<T>& src1, const Iter<T>& src2)
{
	return !(src1.Pointer() == src2.Pointer());
}

template<typename T> const
bool operator<(const Iter<T>& src1, const Iter<T>& src2)
{
	return src1.Pointer() < src2.Pointer();
}

template<typename T> const
bool operator>(const Iter<T>& src1, const Iter<T>& src2)
{
	return src1.Pointer() > src2.Pointer();
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