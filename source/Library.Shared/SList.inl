#pragma once

#include "pch.h"

#pragma region SList
/************************************************************************/
template <typename T>
SList<T>::SList() :
	mFront(nullptr), mBack(nullptr), mSize(0)
{

}

/************************************************************************/
template <typename T>
SList<T>::SList(const SList& rhs) :
	SList()
{
	DeepCopy(rhs);
}

/************************************************************************/
template <typename T>
SList<T>& SList<T>::operator=(const SList& rhs)
{
	if (this != &rhs)
	{
		Clear();
		DeepCopy(rhs);
	}
	return *this;
}

/************************************************************************/
template <typename T>
SList<T>::~SList()
{
	Clear();
}

/************************************************************************/
template <typename T>
typename SList<T>::Iterator SList<T>::PushFront(const T& t)
{
	Node* newNode = new Node(mFront, t);
	mFront = newNode;

	if (IsEmpty())
	{
		mBack = mFront;
	}
	mSize++;

	return begin();
}

/************************************************************************/
template <typename T>
void SList<T>::PopFront()
{
	if (!IsEmpty())
	{
		Node* nodeToPop = mFront;
		mFront = mFront->mNext;

		if (mSize == 1)
		{
			mBack = mFront;
		}

		delete nodeToPop;

		mSize--;
	}
}

/************************************************************************/
template <typename T>
typename SList<T>::Iterator SList<T>::PushBack(const T& t)
{
	Node* newNode = new Node(nullptr, t);

	if (IsEmpty())
	{
		mFront = newNode;
		mBack = newNode;
	}
	else
	{
		mBack->mNext = newNode;
		mBack = newNode;
	}
	mSize++;

	return Iterator(mBack, this);
}

/************************************************************************/
template <typename T>
inline bool SList<T>::IsEmpty() const
{
	return mSize == 0;
}

/************************************************************************/
template <typename T>
const T& SList<T>::Front() const
{
	if (IsEmpty())
	{
		throw std::exception("The list is empty! Cannot get the front element.");
	}
	return mFront->mData;
}

/************************************************************************/
template <typename T>
T& SList<T>::Front()
{
	return const_cast<T&>(const_cast<const SList&>(*this).Front());
}

/************************************************************************/
template <typename T>
const T& SList<T>::Back() const
{
	if (IsEmpty())
	{
		throw std::exception("The list is empty! Cannot get the back element.");
	}
	return mBack->mData;
}

/************************************************************************/
template <typename T>
T& SList<T>::Back()
{
	return const_cast<T&>(const_cast<const SList&>(*this).Back());
}

/************************************************************************/
template <typename T>
inline std::uint32_t SList<T>::Size() const
{
	return mSize;
}

/************************************************************************/
template <typename T>
void SList<T>::Clear()
{
	while (!IsEmpty())
	{
		PopFront();
	}
}

/************************************************************************/
template<typename T>
inline typename SList<T>::Iterator SList<T>::begin() const
{
	return Iterator(mFront, this);
}

/************************************************************************/
template<typename T>
typename SList<T>::Iterator SList<T>::end() const
{
	return Iterator(nullptr, this);
}

/************************************************************************/
template<typename T>
typename SList<T>::Iterator SList<T>::Find(const T & value) const
{
	for (Iterator it = begin(); it != end(); ++it)
	{
		if (*it == value)
		{
			return it;
		}
	}
	return end();
}

/************************************************************************/
template<typename T>
typename SList<T>::Iterator SList<T>::InsertAfter(const T & value, const Iterator& iterator)
{
	if (iterator == end())
	{
		return PushBack(value);
	}
	else
	{
		// build new node and insert after the iterator node
		Node* newNode = new Node(iterator.mNode->mNext, value);
		iterator.mNode->mNext = newNode;
		++mSize;
		return Iterator(newNode, this);
	}
}

/************************************************************************/
template<typename T>
bool SList<T>::Remove(const T & value)
{
	// keep a reference to the previous node to keep the list chained
	Node* previousNode = mFront;
	for (Iterator it = begin(); it != end(); ++it)
	{
		if (*it == value)
		{
			Node* nodeToRemove = it.mNode;
			previousNode->mNext = nodeToRemove->mNext;
			delete nodeToRemove;
			--mSize;
			return true;
		}

		previousNode = it.mNode;
	}

	return false;
}

/************************************************************************/
template<typename T>
void SList<T>::DeepCopy(const SList & rhs)
{
	for (auto& value : rhs)
	{
		PushBack(value);
	}
}
#pragma endregion

#pragma region Iterator
/************************************************************************/
template<typename T>
SList<T>::Iterator::Iterator() :
	mOwner(nullptr), mNode(nullptr)
{
}

/************************************************************************/
template<typename T>
SList<T>::Iterator::Iterator(Node* node, const SList* owner) :
	mNode(node), mOwner(owner)
{
}

/************************************************************************/
template<typename T>
SList<T>::Iterator::Iterator(const Iterator & rhs) :
	mNode(rhs.mNode), mOwner(rhs.mOwner)
{
}

/************************************************************************/
template<typename T>
typename SList<T>::Iterator & SList<T>::Iterator::operator=(const Iterator & rhs)
{
	if (this != &rhs)
	{
		mNode = rhs.mNode;
		mOwner = rhs.mOwner;
	}
	return *this;
}

/************************************************************************/
template<typename T>
typename SList<T>::Iterator& SList<T>::Iterator::operator++()
{
	if (mOwner == nullptr)
	{
		throw std::exception("The iterator is not assigned to a list.");
	}
	if (mNode == nullptr)
	{
		throw std::exception("The iterator is going out of bounds.");
	}

	mNode = mNode->mNext;
	return *this;
}

/************************************************************************/
template<typename T>
typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
{
	Iterator temp = *this;
	++*this;
	return temp;
}

/************************************************************************/
template<typename T>
bool SList<T>::Iterator::operator==(const Iterator & rhs) const
{
	return mOwner == rhs.mOwner && mNode == rhs.mNode;
}

/************************************************************************/
template<typename T>
bool SList<T>::Iterator::operator!=(const Iterator & rhs) const
{
	return !(*this == rhs);
}

/************************************************************************/
template<typename T>
const T& SList<T>::Iterator::operator*() const
{
	if (mOwner == nullptr)
	{
		throw std::exception("The iterator is not assigned to a list.");
	}
	if (mNode == nullptr)
	{
		throw std::exception("The list end cannot be dereferenced.");
	}

	return mNode->mData;
}

/************************************************************************/
template<typename T>
T& SList<T>::Iterator::operator*()
{
	return const_cast<T&>(const_cast<const SList::Iterator&>(*this).operator*());
}

#pragma endregion
