#pragma once

#include "pch.h"
#include "Vector.h"

#pragma region Iterator
/************************************************************************/
template<typename T>
Vector<T>::Iterator::Iterator(Vector owner, std::uint32_t index) :
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
	if (mIndex >= mSize - 1)
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
	if (mIndex <= 0)
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

	return mFront[mIndex];
}

/************************************************************************/
template<typename T>
T & Vector<T>::Iterator::operator*()
{
	return const_cast<T&>(const_cast<const Vector<T>::Iterator&>(*this).operator*());
}

#pragma endregion
