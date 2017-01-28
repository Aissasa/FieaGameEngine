#pragma once

namespace Library
{
	/** SList is a singly-linked list template class.
	 * This is a simple singly-linked list template implementation.
	 */
	template <typename T>
	class SList
	{
	private:

		struct Node
		{
			Node(Node* next, const T& data) :mNext(next), mData(data) {};

			Node* mNext;
			T mData;
		};

		Node* mFront;
		Node* mBack;
		std::uint32_t mSize;

		void DeepCopy(const SList& rhs);

	public:

		/** Iterator is an iterator class for SList.
		 * This is a simple iterator implementation.
		 */
		class Iterator
		{
			friend class SList;
		public:
			/** Iterator constructor.
			 * It creates and initializes an iterator that points to null and belong to no list.
			 */
			Iterator();

			/** SList iterator destructor.
			 */
			~Iterator() = default;

			/** Iterator copy constructor.
			 * @param rhs: Iterator to copy.
			 */
			Iterator(const Iterator& rhs);

			/** Iterator assignment operator overloading method.
			 * It allows the assignment operator to create a copy of the assignee iterator.
			 * @param rhs: Iterator to copy.
			 * @return Result copy.
			 */
			Iterator& operator=(const Iterator& rhs);

			/** Iterator prefix incrementation operator overloading method.
			 * It does a prefix incrementation of the iterator allowing it to point to the following element in the SList.
			 * It throws an exception if the iterator has no owner list or the list is empty.
			 * @return Incremented iterator.
			 */
			Iterator& operator++();

			/** Iterator postfix incrementation operator overloading method.
			 * It does a postfix incrementation of the iterator allowing it to point to the following element in the SList.
			 * It throws an exception if the iterator has no owner list or the list is empty.
			 * @param i: Denotes the postfix form of the increment. Has no effect.
			 * @return Iterator before the incrementation.
			 */
			Iterator operator++(int);

			/** Iterator equal operator overloading method.
			 * @param rhs: Iterator to compare to.
			 * @return Result of the equality comparison.
			 */
			bool operator==(const Iterator& rhs) const;

			/** Iterator not equal operator overloading method.
			 * @param rhs: Iterator to compare to.
			 * @return Result of the non equality comparison.
			 */
			bool operator!=(const Iterator& rhs) const;

			/** Iterator dereference operator overloading method.
			 * @return Data encapsulated in the node pointed to by the iterator.
			 */
			T& operator*();

			/** Iterator dereference operator overloading method.
			 * @return Data encapsulated in the node pointed to by the iterator.
			 */
			const T& operator*() const;

		private:

			Iterator(Node* node, const SList* owner);
			Node* mNode;
			const SList* mOwner;

		};

		/** Singly-linked list constructor.
		 * It creates and initializes an empty list.
		 */
		SList();

		/** Singly-linked list copy constructor.
		 * It makes a deep copy of the right hand side list.
		 * @param rhs: Singly-linked list to copy.
		 */
		SList(const SList& rhs);

		/** Singly-linked list assignment operator overloading method.
		 * It allows the assignment operator to create a deep copy of the assignee list.
		 * @param rhs: Singly-linked list to copy.
		 * @return Result list copy.
		 */
		SList& operator=(const SList& rhs);

		/** Singly-linked list destructor.
		 * It destroys the list and its elements.
		 */
		~SList();

		/** Adds a new element to the front of the list.
		 * @param t: Value of the element to add.
		 * @return Iterator pointing to the node of the added value.
		 */
		Iterator PushFront(const T& t);

		/** Deletes the first element of the list.
		 * Does nothing if the list is empty.
		 */
		void PopFront();

		/** Adds a new element to the back of the list.
		 * @param t: Value of the element to add.
		 * @return Iterator pointing to the node of the added value.
		 */
		Iterator PushBack(const T& t);

		/** Determines if the list is empty.
		 */
		bool IsEmpty() const;

		/** Returns the value of first element in the list.
		 * It throws an exception if the list is empty.
		 * @return First element value.
		 */
		const T& Front() const;

		/** Returns the value of first element in the list.
		 * It throws an exception if the list is empty.
		 * @return First element value.
		 */
		T& Front();

		/** Returns the value of last element in the list.
		 * It throws an exception if the list is empty.
		 * @return Last element value.
		 */
		const T& Back() const;

		/** Returns the value of last element in the list.
		 * It throws an exception if the list is empty.
		 * @return Last element value.
		 */
		T& Back();

		/** Returns the current size of the list.
		 * @return Current list size.
		 */
		std::uint32_t Size() const;

		/** Singly-linked list clear method.
		 * It goes through the list and deletes its elements.
		 */
		void Clear();

		/** Returns an iterator pointing to the first element in the list.
		 * @return Iterator to the first element in the list.
		 */
		Iterator begin() const;

		/** Returns an iterator pointing to the past-the-end element in the list.
		 * @return Iterator to the past-the-end element in the list.
		 */
		Iterator end() const;

		/** Finds an element in the list and returns an iterator pointing to it.
		 * If the list is empty or the element is not found, the past-the-end iterator is returned.
		 * @param value: Value to find in the list.
		 * @return Iterator to the found element in the list.
		 */
		Iterator Find(const T& value) const;

		/** Adds a value as an element to the list, just after the iterator element passed as argument.
		 * If the iterator is the past-the-end iterator, the value is added at the end of the list.
		 * @param value: Value to add to the list.
		 * @param iterator: Iterator to the element to add after.
		 * @return Iterator to the added element.
		 */
		Iterator InsertAfter(const T& value, const Iterator& iterator);

		/** Finds the first occurrence of an element in the list and removes it.
		 * If the element is not found, nothing happens.
		 * @param value: Value to remove from the list.
		 * @return Boolean that represents the success or failure of the method.
		 */
		bool Remove(const T& value);

	};

#include "SList.inl"

}