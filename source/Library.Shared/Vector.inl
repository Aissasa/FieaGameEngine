#pragma once

#include "Vector.h"

#pragma region Iterator

/************************************************************************/
template<typename T>
Vector<T>::Iterator::Iterator(const Vector* owner, std::uint32_t index) :
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
typename Vector<T>::Iterator& Vector<T>::Iterator::operator=(const Iterator & rhs)
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
	if (mIndex >= mOwner->mSize)
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
typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
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
		throw std::exception("The iterator is not assigned to a vector.");
	}

	if (mIndex >= mOwner->mSize)
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
Vector<T>::Vector(std::uint32_t initCapacity, bool fixedSize) :
	mSize(0), mCapacity(0), mFront(nullptr)
{
	Reserve(initCapacity);
	if (fixedSize)
	{
		for (uint32_t i = 0; i < mCapacity; i++)
		{
			PushBack(T());
		}
	}
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

	return *this;
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
inline std::uint32_t Vector<T>::Capacity() const
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
		mFront = static_cast<T*>(realloc(mFront, newCapacity * sizeof(T)));
		//Assert::IsNotNull(mFront);
		mCapacity = newCapacity;
		return true;
	}
}

/************************************************************************/
template<typename T>
const T & Vector<T>::operator[](std::uint32_t index) const
{
	if (IsEmpty())
	{
		throw std::exception("The vector is empty!");
	}

	if (index >= mSize)
	{
		throw std::exception("Index out of bounds!");
	}

	return mFront[index];
}

/************************************************************************/
template<typename T>
T & Vector<T>::operator[](std::uint32_t index)
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
const T & Vector<T>::Front() const
{
	return operator[](0);
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
	return operator[](mSize - 1);
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
		// todo add functor
		Reserve(std::max(DEFAULT_CAPACITY, mCapacity * 2));
	}
	// initialize a new element
	new(mFront + mSize)T(t);
	return Iterator(this, mSize++);
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
		// todo refactor vector remove
		// .~T();
		// auto size = (mSize - it.mIndex - 1)* sizeof(T);
// 		if (size > 0)
// 		{
// 			memmove_s(&mFront[it.mIndex], size, &mFront[it.mIndex + 1], size);
// 		}
		// -- mSize;
		ShiftLeftFrom(it); // note get rid of this then
		PopBack();
		return true;
	}
}

/************************************************************************/
template<typename T>
bool Vector<T>::Remove(const Iterator & first, const Iterator & last)
{
	// todo refactor vector remove 2
	if (first.mOwner != this || last.mOwner != this)
	{
		throw std::exception("Iterators don't belong to the vector!");
	}

	if (first.mIndex >= mSize || last.mIndex > mSize)
	{
		throw std::exception("One of the iterators is out of bounds!");
	}

	if (first.mIndex > last.mIndex)
	{
		return false;
	}

	if (first == last)
	{
		ShiftLeftFrom(first);
		PopBack();
		return true;
	}

	uint count = last.mIndex - first.mIndex;

	if (last != end())
	{
		Iterator firstTemp = first;
		Iterator lastTemp = last;

		// just override the data with what's after it
		while (lastTemp != end())
		{
			*firstTemp++ = *lastTemp++;
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
	Empty();
	free(mFront);
	mFront = nullptr;
	mCapacity = 0;
}

/************************************************************************/
template<typename T>
void Vector<T>::Empty()
{
	while (!IsEmpty())
	{
		PopBack();
	}
}

/************************************************************************/
template<typename T>
bool Vector<T>::operator==(const Vector & rhs) const
{
	if (mSize != rhs.mSize)
	{
		return false;
	}
	bool result = true;
	auto it2 = rhs.begin();
	for (auto it1 = begin(); it1 != end(); ++it1)
	{
		if (*it1 != *it2)
		{
			result = false;
			break;
		}
		++it2;
	}

	return result;
}

/************************************************************************/
template<typename T>
bool Vector<T>::operator!=(const Vector & rhs) const
{
	return !(operator==(rhs));
}

/************************************************************************/
template<typename T>
typename Vector<T>::Iterator Vector<T>::begin() const
{
	return Iterator(this, 0);
}

/************************************************************************/
template<typename T>
typename Vector<T>::Iterator Vector<T>::end() const
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

/************************************************************************/
template<typename T>
void Vector<T>::ShiftLeftFrom(Iterator it)
{
	Iterator last(this, mSize - 1);
	while (it != last)
	{
		auto temp = it;
		++it;
		*temp = *it;
	}
}

#pragma endregion
