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
	mFront(nullptr), mBack(nullptr), mSize(0)
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
typename SList<T>::SListIterator SList<T>::PushFront(const T& t)
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
typename SList<T>::SListIterator SList<T>::PushBack(const T& t)
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

	return SListIterator(mBack, this);
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
inline typename SList<T>::SListIterator SList<T>::begin() const
{
	return SListIterator(mFront, this);
}

/************************************************************************/
template<typename T>
typename SList<T>::SListIterator SList<T>::end() const
{
	if (IsEmpty())
	{
		return begin();
	}

	return SListIterator(mBack->mNext, this);
}

/************************************************************************/
template<typename T>
typename SList<T>::SListIterator SList<T>::Find(const T & value) const
{
	for (SListIterator it = begin(); it != end(); ++it)
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
typename SList<T>::SListIterator SList<T>::InsertAfter(const T & value, const SListIterator& iterator)
{
	if (iterator == end())
	{
		SListIterator itBack = PushBack(value);
		return itBack;
	}
	else
	{
		SListIterator it = iterator;
		Node* newNode = new Node(it.mNode->mNext, value);
		it.mNode->mNext = newNode;
		++mSize;
		return ++it;
	}
}

/************************************************************************/
template<typename T>
bool SList<T>::Remove(const T & value)
{
	Node* previousNode = mFront;
	for (SListIterator it = begin(); it != end(); ++it)
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
	//Node* currentNode = rhs.mFront;
	//while (currentNode != nullptr)
	//{
	//	PushBack(currentNode->mData);
	//	currentNode = currentNode->mNext;
	//}
}
#pragma endregion

#pragma region SListIterator
/************************************************************************/
template<typename T>
SList<T>::SListIterator::SListIterator() :
	mOwner(nullptr), mNode(nullptr)
{
}

/************************************************************************/
template<typename T>
SList<T>::SListIterator::SListIterator(Node* node, const SList* owner) :
	mNode(node), mOwner(owner)
{
}

/************************************************************************/
template<typename T>
SList<T>::SListIterator::SListIterator(const SListIterator & rhs) :
	mNode(rhs.mNode), mOwner(rhs.mOwner)
{
}

/************************************************************************/
template<typename T>
typename SList<T>::SListIterator & SList<T>::SListIterator::operator=(const SListIterator & rhs)
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
typename SList<T>::SListIterator SList<T>::SListIterator::operator++()
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
typename SList<T>::SListIterator SList<T>::SListIterator::operator++(int)
{
	SListIterator temp = *this;
	++*this;
	return temp;
}

/************************************************************************/
template<typename T>
bool SList<T>::SListIterator::operator==(const SListIterator & rhs) const
{
	return mOwner == rhs.mOwner && mNode == rhs.mNode;
}

/************************************************************************/
template<typename T>
bool SList<T>::SListIterator::operator!=(const SListIterator & rhs) const
{
	return !(*this == rhs);
}

/************************************************************************/
template<typename T>
const T& SList<T>::SListIterator::operator*() const
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
T& SList<T>::SListIterator::operator*()
{
	return const_cast<T&>(const_cast<const SList::SListIterator&>(*this).operator*());
}

#pragma endregion
