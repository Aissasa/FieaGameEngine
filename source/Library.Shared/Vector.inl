#pragma once

#include "pch.h"
#include "Vector.h"

#pragma region Vector Iterator
/************************************************************************/
template<typename T>
Vector<T>::VectorIterator::VectorIterator(Vector owner, std::uint32_t index) :
	mOwner(owner), mIndex(index)
{
}

/************************************************************************/
template<typename T>
Vector<T>::VectorIterator::VectorIterator() :
	VectorIterator(nullptr, 0)
{
}

/************************************************************************/
template<typename T>
Vector<T>::VectorIterator::VectorIterator(const VectorIterator & rhs) :
	VectorIterator(rhs.mOwner, rhs.mIndex)
{
}

/************************************************************************/
template<typename T>
typename Vector<T>::VectorIterator & Vector<T>::VectorIterator::operator=(const VectorIterator & rhs)
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
typename Vector<T>::VectorIterator Vector<T>::VectorIterator::operator++()
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
typename Vector<T>::VectorIterator Vector<T>::VectorIterator::operator++(int)
{
	VectorIterator temp = *this;
	++(*this);
	return temp;
}

/************************************************************************/
template<typename T>
typename Vector<T>::VectorIterator Vector<T>::VectorIterator::operator--()
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
typename Vector<T>::VectorIterator Vector<T>::VectorIterator::operator--(int)
{
	VectorIterator temp = *this;
	--(*this);
	return temp;
}

/************************************************************************/
template<typename T>
inline bool Vector<T>::VectorIterator::operator==(const VectorIterator & rhs) const
{
	return mOwner == rhs.mOwner && mIndex == rhs.mIndex;
}

/************************************************************************/
template<typename T>
inline bool Vector<T>::VectorIterator::operator!=(const VectorIterator & rhs) const
{
	return !(*this == rhs);
}

/************************************************************************/
template<typename T>
const T & Vector<T>::VectorIterator::operator*() const
{
	if (mOwner == nullptr)
	{
		throw std::exception("The iterator is not assigned to a list.");
	}

	return mFront[mIndex];
}

/************************************************************************/
template<typename T>
T & Vector<T>::VectorIterator::operator*()
{
	return const_cast<T&>(const_cast<const Vector<T>::VectorIterator&>(*this).operator*());
}

#pragma endregion
