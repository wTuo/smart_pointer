#ifndef SmartPointer_h_
#define SmartPointer_h_

#include <iostream>
#include <assert.h>

/**********************************************ReferenceCounter******************************************************/
class ReferenceCounter
{
protected:
	unsigned int counter;
public:
	ReferenceCounter(): counter(0) {}

	
	~ReferenceCounter() { assert(counter == 0); } 
	void Add() { counter++; }
	unsigned int Remove() { return --counter; }
	unsigned int getCounter() const { return counter; }
};

/***********************************************SmartPointer****************************************************/
template <typename T>
class SmartPointer
{
	template <typename U> friend class SmartPointer;
public:
	SmartPointer();
	~SmartPointer() { Remove(); }

	SmartPointer(T *other);	
	template <typename U> SmartPointer(U *other);

	SmartPointer(const SmartPointer<T> &other);
	template <typename U> SmartPointer(const SmartPointer<U> &other);

	SmartPointer& operator=(T* other);
	template<typename U> SmartPointer& operator=(U *other);

	T& operator*() const;
	T* operator->() const;

protected:
	T *pointer;
	ReferenceCounter *ref;

	void Remove();
};

/***************************************************Inline********************************************************/


template <typename T>
SmartPointer<T>::SmartPointer()
	:pointer(nullptr), ref(nullptr)
{
	ref = new ReferenceCounter;
	ref->Add();
}


template <typename T>
SmartPointer<T>::SmartPointer(T *other) 
	:pointer(other), ref(nullptr)
{
	ref = new ReferenceCounter;
	ref->Add();
}


template <typename T> template <typename U>
SmartPointer<T>::SmartPointer(U * other)
	:pointer(static_cast<T*>(other)), ref(nullptr)
{
	ref = new ReferenceCounter;
	ref->Add();
}


template <typename T>
SmartPointer<T>::SmartPointer(const SmartPointer<T> &other)
	:pointer(other.pointer), ref(other.ref)
{
	ref->Add();
}


template <typename T> template <typename U>
inline SmartPointer<T>::SmartPointer(const SmartPointer<U> &other)
	:pointer(static_cast<T*>(other.pointer)), ref(other.ref)
{
	ref->Add();
}

//Operator= dla (T* other)
template <typename T>
SmartPointer<T>& SmartPointer<T>::operator=(T* other)
{
	if (pointer != other)
	{
		Remove();
		pointer = other;
		ref = new ReferenceCounter;
		ref->Add();
	}
	return *this;
}


//Operator= dla (U* other)
template<typename T> template<typename U>
SmartPointer<T>& SmartPointer<T>::operator=(U *other)
{
	*this = static_cast<T*>(other); //this->operator=(static_cast<T*>(other));
	return *this;
}

//! operator*
template <typename T>
T & SmartPointer<T>::operator*() const
{
	assert(pointer != nullptr);
	return *pointer;
}

//! Operator->
template <typename T>
T * SmartPointer<T>::operator->() const
{
	assert(pointer != nullptr);
	return pointer;
}


template <typename T>
inline void SmartPointer<T>::Remove()
{
	if (ref->Remove() == 0)
	{
		delete pointer;
		delete ref;
	}
}

#endif //SmartPointer_h_
