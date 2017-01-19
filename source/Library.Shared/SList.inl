#pragma once

#include "pch.h"
#include "SList.h"

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
void SList<T>::PushFront(const T& t)
{
	Node* newNode = new Node();

	newNode->Data = t;
	newNode->Next = mFront;
	mFront = newNode;

	if (IsEmpty())
	{
		mBack = mFront;
	}
	mSize++;
}

/************************************************************************/
template <typename T>
void SList<T>::PopFront()
{
	if (!IsEmpty())
	{
		Node* nodeToPop = mFront;
		mFront = mFront->Next;

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
void SList<T>::PushBack(const T& t)
{
	Node* newNode = new Node();

	newNode->Data = t;
	newNode->Next = nullptr;

	if (IsEmpty())
	{
		mFront = newNode;
		mBack = newNode;
	}
	else
	{
		mBack->Next = newNode;
		mBack = mBack->Next;
	}
	mSize++;
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
	return mFront->Data;
}

/************************************************************************/
template <typename T>
T& SList<T>::Front()
{
	return const_cast<T&>(static_cast<const SList&>(*this).Front());
}

/************************************************************************/
template <typename T>
const T& SList<T>::Back() const
{
	if (IsEmpty())
	{
		throw std::exception("The list is empty! Cannot get the back element.");
	}
	return mBack->Data;
}

/************************************************************************/
template <typename T>
T& SList<T>::Back()
{
	return const_cast<T&>(static_cast<const SList&>(*this).Back());
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
inline void SList<T>::DeepCopy(const SList & rhs)
{
	Node* currentNode = rhs.mFront;
	while (currentNode != nullptr)
	{
		PushBack(currentNode->Data);
		currentNode = currentNode->Next;
	}
}

