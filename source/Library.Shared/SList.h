#pragma once

namespace Library
{
	/** SList is a singly-linked list class.
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

		/** SListIterator is an iterator class for SList.
		* This is a simple iterator implementation.
		*/
		class SListIterator
		{
			friend class SList;
		public:
			/** SListIterator constructor.
			* It creates and initializes an iterator that points to null and belong to no list.
			*/
			SListIterator();

			/** SList iterator destructor.
			*/
			~SListIterator() = default;

			/** SListIterator copy constructor.
			* @param SListIterator list to copy.
			*/
			SListIterator(const SListIterator& rhs);

			/** SListIterator assignment operator overloading method.
			* It allows the assignment operator to create a copy of the assignee iterator.
			* @param SListIterator to copy.
			* @return Result copy.
			*/
			SListIterator& operator=(const SListIterator& rhs); 

			/** SListIterator prefix incrementation operator overloading method.
			* It does a prefix incrementation of the iterator allowing it to point to the following element in the SList.
			* It throws an exception if the iterator has no owner list or the list is empty.
			* @return Incremented iterator.
			*/
			SListIterator operator++();

			/** SListIterator postfix incrementation operator overloading method.
			* It does a postfix incrementation of the iterator allowing it to point to the following element in the SList.
			* It throws an exception if the iterator has no owner list or the list is empty.
			* @param Denotes the postfix form of the increment. Has no effect.
			* @return Iterator before the incrementation.
			*/
			SListIterator operator++(int);

			/** SListIterator equal operator overloading method.
			* @param Iterator to compare to.
			* @return Result of the equality comparison.
			*/
			bool operator==(const SListIterator& rhs) const;

			/** SListIterator not equal operator overloading method.
			* @param Iterator to compare to.
			* @return Result of the non equality comparison.
			*/
			bool operator!=(const SListIterator& rhs) const;

			/** SListIterator dereference operator overloading method.
			* @return Data encapsulated in the node pointed to by the iterator.
			*/
			T& operator*();

			/** SListIterator dereference operator overloading method.
			* @return Data encapsulated in the node pointed to by the iterator.
			*/
			const T& operator*() const;

		private:

			SListIterator(Node* node, const SList* owner);
			Node* mNode;
			const SList* mOwner;

		};

		/** Singly-linked list constructor.
		 * It creates and initializes an empty list.
		 */
		SList();

		/** Singly-linked list copy constructor.
		 * It makes a deep copy of the right hand side list.
		 * @param Singly-linked list to copy.
		 */
		SList(const SList& rhs);

		/** Singly-linked list assignment operator overloading method.
		 * It allows the assignment operator to create a deep copy of the assignee list.
		 * @param Singly-linked list to copy.
		 * @return Result list copy.
		 */
		SList& operator=(const SList& rhs);

		/** Singly-linked list destructor.
		 * It destroys the list and its elements.
		 */
		~SList();

		/** Adds a new element to the front of the list.
		 * @param Value of the element to add.
		 * @return Iterator pointing to the node of the added value.
		 */
		SListIterator PushFront(const T& t);

		/** Deletes the first element of the list.
		 */
		void PopFront();

		/** Adds a new element to the back of the list.
		 * @param Value of the element to add.
		 * @return Iterator pointing to the node of the added value.
		 */
		SListIterator PushBack(const T& t);

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
		SListIterator begin() const;

		/** Returns an iterator pointing to the past-the-end element in the list.
		* @return Iterator to the past-the-end element in the list.
		*/
		SListIterator end() const;

		/** Finds an element in the list and returns an iterator pointing to it.
		* If the list is empty or the element is not found, the past-the-end iterator is returned.
		* @parameter Value to find in the list.
		* @return Iterator to the found element in the list.
		*/
		SListIterator Find(const T& value) const;

		/** Adds a value as an element to the list, just after the iterator element passed as argument.
		* If the iterator is the past-the-end iterator, the value is added at the end of the list.
		* @parameter Value to add to the list.
		* @parameter Iterator to the element to add after.
		* @return Iterator to the added element.
		*/
		SListIterator InsertAfter(const T& value, const SListIterator& iterator);

		/** Finds an element in the list and removes it.
		* If the element is not found, nothing happens.
		* @parameter Value to remove from the list.
		* @return Boolean that represents the success or failure of method.
		*/
		bool Remove(const T& value);

	};

#include "SList.inl"

}