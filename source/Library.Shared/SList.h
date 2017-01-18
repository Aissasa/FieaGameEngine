#pragma once

namespace FieaGameEngine
{
	/**
	*/
	/** SList is a singly-linked list class.
	*This is a simple singly-linked list template implementation.
	*/
	template <typename T>
	class SList
	{
	public:
		/** Singly-linked list constructor.
		It create and initializes an empty list.
		*/
		SList();

		/** Singly-linked list copy constructor.
		It makes a deep copy of the right hand side list.
		@param Singly-linked list to copy.
		*/
		SList(const SList& rhs);
		
		/** Singly-linked list assignment operator overloading method.
		It allows the assignment operator to create a deep copy of the assignee list.
		@param Singly-linked list to copy.
		@return Result copy.
		*/
		SList& operator=(const SList& rhs);

		/** Singly-linked list destructor.
		It destroys the list and its elements.
		*/
		~SList();

		/** Adds a new element to the front of the list.
		@param Value of the element to add.
		*/
		void PushFront(const T& t);

		/** Deletes the first element of the list.
		*/
		void PopFront();

		/** Adds a new element to the back of the list.
		@param Value of the element to add.
		*/
		void PushBack(const T& t);

		/** Determines if the list is empty.
		*/
		bool IsEmpty();

		/** Returns the value of first element in the list.
		@return First element value.
		*/
		T& Front();

		/** Returns the value of last element in the list.
		@return Last element value.
		*/
		T& Back();

		/** Returns the current size of the list.
		@return Current list size.
		*/
		std::uint32_t Size();

		/** Singly-linked list clear method method.
		It goes through the list and destroys its elements.
		*/
		void Clear();

	private:

		struct Node
		{
			Node* Next;
			T Data;
		};

		Node* mFront;
		Node* mBack;
		std::uint32_t mSize;
	};

#include "SList.inl"
}