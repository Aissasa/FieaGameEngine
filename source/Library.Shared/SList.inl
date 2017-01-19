#pragma once

#include "pch.h"

template <typename T>
SList<T>::SList() :
	mFront(nullptr), mBack(nullptr), mSize(0) 
{

}

template <typename T>
SList<T>::SList(const SList& rhs) :
	mFront(nullptr), mBack(nullptr), mSize(0)
{
	// deep copy
	Node* currentNode = rhs.mFront;
	while (currentNode != nullptr)
	{
		PushBack(currentNode->Data);
		currentNode = currentNode->Next;
	}
}

template <typename T>
SList<T>& SList<T>::operator=(const SList& rhs)
{
	if (this != &rhs)
	{
		Clear();

		// deep copy
		Node* currentNode = rhs.mFront;
		while (currentNode != nullptr)
		{
			PushBack(currentNode->Data);
			currentNode = currentNode->Next;
		}
	}
	return *this;
}

template <typename T>
SList<T>::~SList()
{
	Clear();
}

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

template <typename T>
inline bool SList<T>::IsEmpty()
{
	return mSize == 0;
}

template <typename T>
inline T& SList<T>::Front()
{
	if (IsEmpty())
	{
		throw std::exception("The list is empty! Cannot get the front element.");
	}
	return mFront->Data;
}

template <typename T>
inline T& SList<T>::Back()
{
	if (IsEmpty())
	{
		throw std::exception("The list is empty! Cannot get the back element.");
	}
	return mBack->Data;
}

template <typename T>
inline std::uint32_t SList<T>::Size()
{
	return mSize;
}

template <typename T>
void SList<T>::Clear()
{
	while (!IsEmpty())
	{
		PopFront();
	}
}

