#pragma once

template <typename T>
class Alloc
{
public:
	T* allocate(size_t count)
	{
		return (T*)(operator new(sizeof(T) * count));
	}

	void deallocate(T* p, size_t n)
	{
		operator delete(p);
	}

	size_t max_size() const
	{
		return SIZE_MAX / sizeof(T);
	}

	void destroy(T* p)
	{
		p->~T();
	}

	void construct(T* p, const T& val)// pereadelat'
	{
		new (/*(void*)*/p) T(val);
	}
};