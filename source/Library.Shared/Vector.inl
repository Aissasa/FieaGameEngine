#pragma once

#include "pch.h"
#include "Vector.h"

#pragma region Iterator
/************************************************************************/
template<typename T>
Vector<T>::Iterator::Iterator(Vector* owner, std::uint32_t index) :
	mOwner(owner), mIndex(index)
{
}

/************************************************************************/
template<typename T>
Vector<T>::Iterator::Iterator() :
	Iterator(nullptr, 0)
{
}

/************************************************************************/
template<typename T>
Vector<T>::Iterator::Iterator(const Iterator & rhs) :
	Iterator(rhs.mOwner, rhs.mIndex)
{
}

/************************************************************************/
template<typename T>
typename Vector<T>::Iterator & Vector<T>::Iterator::operator=(const Iterator & rhs)
{
	if (this != &rhs)
	{
		mOwner = rhs.mOwner;
		mIndex = rhs.mIndex;
	}
	return *this;
}

/************************************************************************/
template<typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
{
	if (mOwner == nullptr)
	{
		throw std::exception("The iterator is not assigned to a vector.");
	}
	if (mIndex + 1 >= mOwner->mSize)
	{
		throw std::exception("The iterator is going out of bounds.");
	}

	++mIndex;
	return *this;
}

/************************************************************************/
template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
{
	Iterator temp = *this;
	++(*this);
	return temp;
}

/************************************************************************/
template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator--()
{
	if (mOwner == nullptr)
	{
		throw std::exception("The iterator is not assigned to a vector.");
	}
	if (mIndex == 0)
	{
		throw std::exception("The iterator is going out of bounds.");
	}

	--mIndex;
	return *this;
}

/************************************************************************/
template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
{
	Iterator temp = *this;
	--(*this);
	return temp;
}

/************************************************************************/
template<typename T>
inline bool Vector<T>::Iterator::operator==(const Iterator & rhs) const
{
	return mOwner == rhs.mOwner && mIndex == rhs.mIndex;
}

/************************************************************************/
template<typename T>
inline bool Vector<T>::Iterator::operator!=(const Iterator & rhs) const
{
	return !(*this == rhs);
}

/************************************************************************/
template<typename T>
const T & Vector<T>::Iterator::operator*() const
{
	if (mOwner == nullptr)
	{
		throw std::exception("The iterator is not assigned to a list.");
	}

	if (mIndex >= mOwner->mSize || mOwner->mSize == 0)
	{
		throw std::exception("Out of bounds reference.");
	}

	return mOwner->mFront[mIndex];
}

/************************************************************************/
template<typename T>
T & Vector<T>::Iterator::operator*()
{
	return const_cast<T&>(const_cast<const Vector<T>::Iterator&>(*this).operator*());
}

#pragma endregion

#pragma region Vector
/************************************************************************/
template<typename T>
Vector<T>::Vector(std::uint32_t initCapacity = DEFAULT_CAPACITY) :
	mSize(0), mCapacity(0), mFront(nullptr)
{
	Reserve(initCapacity);
}

/************************************************************************/
template<typename T>
Vector<T>::Vector() :
	Vector(DEFAULT_CAPACITY)
{
}

/************************************************************************/
template<typename T>
Vector<T>::~Vector()
{
	Clear();
}

/************************************************************************/
template<typename T>
Vector<T>::Vector(const Vector & rhs) :
	Vector(rhs.mCapacity)
{
	DeepCopy(rhs);
}

/************************************************************************/
template<typename T>
Vector<T> & Vector<T>::operator=(const Vector & rhs)
{
	if (this != &rhs)
	{
		Clear();
		Reserve(rhs.mCapacity);
		DeepCopy(rhs);
	}
}

/************************************************************************/
template<typename T>
inline bool Vector<T>::IsEmpty() const
{
	return mSize == 0;
}

/************************************************************************/
template<typename T>
inline std::uint32_t Vector<T>::Size() const
{
	return mSize;
}

/************************************************************************/
template<typename T>
inline std::uint32_t Vector<T>::Capcity() const
{
	return mCapacity;
}

/************************************************************************/
template<typename T>
bool Vector<T>::Reserve(std::uint32_t newCapacity)
{
	if (newCapacity < mCapacity)
	{
		return false;
	}
	else
	{
		mFront = (T*)realloc(mFront, newCapacity * sizeof(T));
		if (mFront == nullptr)
		{
			throw std::exception("Allocation failed!");
		}
		mCapacity = newCapacity;
		return true;
	}
}

/************************************************************************/
template<typename T>
const T & Vector<T>::operator[](int index) const
{
	if (IsEmpty())
	{
		throw std::exception("The vector is empty!");
	}

	if (index < 0 || index >= mSize)
	{
		throw std::exception("Index out of bounds!");
	}

	return mFront[index];
}

/************************************************************************/
template<typename T>
T & Vector<T>::operator[](int index)
{
	return const_cast<T&>(const_cast<const Vector&>(*this).operator[](index));
}

/************************************************************************/
template<typename T>
const T & Vector<T>::At(const std::uint32_t index) const
{
	return operator[](index);
}

/************************************************************************/
template<typename T>
T & Vector<T>::At(const std::uint32_t index)
{
	return const_cast<T&>(const_cast<const Vector&>(*this).At(index));
}

/************************************************************************/
template<typename T>
inline const T & Vector<T>::Front() const
{
	return *mFront;
}

/************************************************************************/
template<typename T>
T & Vector<T>::Front()
{
	return const_cast<T&>(const_cast<const Vector&>(*this).Front());
}

/************************************************************************/
template<typename T>
const T & Vector<T>::Back() const
{
	return At(mSize - 1);
}

/************************************************************************/
template<typename T>
T & Vector<T>::Back()
{
	return const_cast<T&>(const_cast<const Vector&>(*this).Back());
}

/************************************************************************/
template<typename T>
typename Vector<T>::Iterator Vector<T>::Find(const T & t) const
{
	Iterator it = begin();
	for (; it != end(); ++it)
	{
		if (*it == t)
		{
			break;
		}
	}
	return it;
}

/************************************************************************/
template<typename T>
typename Vector<T>::Iterator Vector<T>::PushBack(const T & t)
{
	if (mSize == mCapacity)
	{
		if (mCapacity == 0)
		{
			Reserve(DEFAULT_CAPACITY);
		}
		else
		{
			Reserve(mCapacity * 2);
		}
	}
	T* temp = mFront + mSize;
	temp = new(mFront + mSize) T;
	*temp = t;
	++mSize;

	return Iterator(this, mSize - 1);
}

/************************************************************************/
template<typename T>
void Vector<T>::PopBack()
{
	if (!IsEmpty())
	{
		mFront[--mSize].~T();
	}
}

/************************************************************************/
template<typename T>
bool Vector<T>::Remove(const T & t)
{
	Iterator it = Find(t);
	if (it == end())
	{
		return false;
	}
	else
	{
		Iterator last(this, mSize - 1);
		while (it != last)
		{
			*it = *(++it);
		}

		PopBack();
		return true;
	}
}

/************************************************************************/
template<typename T>
bool Vector<T>::Remove(const Iterator & first, const Iterator & last)
{
	if (first.mIndex >= mSize || last.mIndex > mSize)
	{
		throw std::exception("Iterators are out of bounds!");
	}

	if (first.mIndex >= last.mIndex)
	{
		return false;
	}

	std::uint32_t count = last.mIndex - first.index;

	if (last != end())
	{
		Iterator firstTemp = first;
		Iterator lastTemp = last;

		// just override the data with what's after it
		while (lastTemp != end())
		{
			*lastTemp = *firstTemp;
		}
	}

	for (std::uint32_t i = 0; i < count; ++i)
	{
		PopBack();
	}

	return true;
}

/************************************************************************/
template<typename T>
void Vector<T>::Clear()
{
	while (!IsEmpty())
	{
		PopBack();
	}
	free(mFront);
	mCapacity = 0;
}

/************************************************************************/
template<typename T>
typename Vector<T>::Iterator Vector<T>::begin()
{
	return Iterator(this, 0);
}

/************************************************************************/
template<typename T>
typename Vector<T>::Iterator Vector<T>::end()
{
	return Iterator(this, mSize);
}

/************************************************************************/
template<typename T>
void Vector<T>::DeepCopy(const Vector & rhs)
{
	for (auto& value : rhs)
	{
		PushBack(value);
	}
}
#pragma endregion
