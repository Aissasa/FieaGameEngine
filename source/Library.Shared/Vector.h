#pragma once

namespace Library
{
	/** Vector is a simple dynamic array template class.
	 * This is a simple dynamic array template implementation.
	 */
	template <typename T>
	class Vector
	{
	public:
		/** Iterator is an iterator class for Vector.
		 * This is a simple iterator implementation.
		 */
		class Iterator
		{
			friend class Vector;
		public:
			/** Iterator constructor.
			* It creates and initializes an iterator that belongs to no vector.
			*/
			Iterator();

			/** Vector iterator destructor.
			*/
			~Iterator() = default;

			/** Iterator copy constructor.
			* @param rhs: Iterator to copy.
			*/
			Iterator(const Iterator & rhs);

			/** Iterator assignment operator overloading method.
			* It allows the assignment operator to create a copy of the assignee iterator.
			* @param rhs: Iterator to copy.
			* @return Result copy.
			*/
			Iterator& operator=(const Iterator& rhs);

			/** Iterator prefix incrementation operator overloading method.
			* It does a prefix incrementation of the iterator allowing it to point to the following element in the Vector.
			* It throws an exception if the iterator has no owner vector or if it's going out of bounds.
			* @return Incremented iterator.
			*/
			Iterator& operator++();

			/** Iterator postfix incrementation operator overloading method.
			* It does a postfix incrementation of the iterator allowing it to point to the following element in the Vector.
			* It throws an exception if the iterator has no owner vector or if it's going out of bounds.
			* @param i: Denotes the postfix form of the increment. Has no effect.
			* @return Iterator before the incrementation.
			*/
			Iterator operator++(int);

			/** Iterator prefix decrementation operator overloading method.
			* It does a prefix decrementation of the iterator allowing it to point to the previous element in the Vector.
			* It throws an exception if the iterator has no owner vector or if it's going out of bounds.
			* @return Decremented iterator.
			*/
			Iterator& operator--();

			/** Iterator postfix decrementation operator overloading method.
			* It does a postfix decrementation of the iterator allowing it to point to the previous element in the Vector.
			* It throws an exception if the iterator has no owner vector or if it's going out of bounds.
			* @param i: Denotes the postfix form of the decrement. Has no effect.
			* @return Iterator before the decrementation.
			*/
			Iterator operator--(int);

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
			* @return Data pointed to by the iterator.
			*/
			const T& operator*() const;

			/** Iterator dereference operator overloading method.
			* @return Data pointed to by the iterator.
			*/
			T& operator*();

		private:
			Iterator(const Vector* owner, std::uint32_t index);
			const Vector* mOwner;
			std::uint32_t mIndex;
		};

		/** Vector constructor.
		 * It creates an empty vector with a default capacity.
		 */
		Vector();

		/** Vector constructor.
		 * It creates an empty vector with an initial capacity.
		 * @param initialCapacity: Initial capacity.
		 */
		Vector(std::uint32_t initCapacity);

		/** Vector destructor.
		 * It destroys the vector and its elements.
		 */
		~Vector();

		/** Vector copy constructor.
		 * It makes a deep copy of the right hand side vector.
		 * @param rhs: Vector to copy.
		 */
		Vector(const Vector & rhs);

		/** Vector assignment operator overloading method.
		 * It allows the assignment operator to create a deep copy of the assignee vector.
		 * @param rhs: Vector to copy.
		 * @return Result vector copy.
		 */
		Vector& operator=(const Vector& rhs);

		/** Determines if the vector is empty.
		 */
		bool IsEmpty() const;

		/** Returns the current size of the vector.
		 * @return Current vector size.
		 */
		std::uint32_t Size() const;

		/** Returns the current capacity of the vector.
		 * @return Current vector capacity.
		 */
		std::uint32_t Capacity() const;

		/** Reallocates memory for the vector.
		 * It reallocates the current memory of the vector. Only succeeds if the new capacity is more than the old capacity.
		 * @param newCapacity: New desired capacity of the vector.
		 * @return Boolean expressing the success of the reallocation.
		 */
		bool Reserve(std::uint32_t newCapacity);

		/** Vector bracket operator overloading method.
		 * It returns the value of the indexed element on the vector.
		 * It throws an exception if the vector is empty or the index is out of bounds.
		 * @param index: Index of the element on the vector.
		 * @return Value of the indexed element on the vector.
		 */
		const T& operator[](int index) const;

		/** Vector bracket operator overloading method.
		 * It returns the value of the indexed element on the vector.
		 * It throws an exception if the vector is empty or the index is out of bounds.
		 * @param index: Index of the element on the vector.
		 * @return Value of the indexed element on the vector.
		 */
		T& operator[](int index);

		/** Returns the value of the indexed element on the vector.
		 * It throws an exception if the vector is empty or the index is out of bounds.
		 * @param index: Index of the element on the vector.
		 * @return Value of the indexed element on the vector.
		 */
		const T& At(const std::uint32_t index) const;

		/** Returns the value of the indexed element on the vector.
		 * It throws an exception if the vector is empty or the index is out of bounds.
		 * @param index: Index of the element on the vector.
		 * @return Value of the indexed element on the vector.
		 */
		T& At(const std::uint32_t index);

		/** Returns the value of the first element on the vector.
		 * It throws an exception if the vector is empty.
		 * @return Value of the first element on the vector.
		 */
		const T& Front() const;

		/** Returns the value of the first element on the vector.
		 * It throws an exception if the vector is empty.
		 * @return Value of the first element on the vector.
		 */
		T& Front();

		/** Returns the value of the last element on the vector.
		 * It throws an exception if the vector is empty.
		 * @return Value of the last element on the vector.
		 */
		const T& Back() const;

		/** Returns the value of the last element on the vector.
		 * It throws an exception if the vector is empty.
		 * @return Value of the last element on the vector.
		 */
		T& Back();

		/** Finds an element in the vector.
		 * If the vector is empty or the element is not found, the past-the-end iterator is returned.
		 * @param t: Value to find in the vector.
		 * @return Iterator to the found element in the vector.
		 */
		Iterator Find(const T& t) const;

		/** Adds a new element to the back of the vector.
		 * It reserve additional memory if necessary.
		 * @param t: Value of the element to add.
		 * @return Iterator pointing to the added value.
		 */
		Iterator PushBack(const T& t);

		/** Deletes the last element of the vector.
		 * Does nothing if the vector is empty.
		 */
		void PopBack();

		/** Finds the first occurrence of an element in the vector and removes it.
		 * It keeps the data contiguous after removing the element.
		 * If the element is not found, nothing happens.
		 * @param t: Value to remove from the vector.
		 * @return Boolean that represents the success or failure of the method.
		 */
		bool Remove(const T& t);

		/** Removes a range of elements in the vector.
		 * It keeps the data contiguous after removing the elements.
		 * It removes the elements in [first, last[
		 * It throws an exception if one of the iterators is invalid.
		 * It fails if first > last.
		 * @param first: Iterator of the first element.
		 * @param last: Iterator of the last element.
		 * @return Boolean that represents the success or failure of the method.
		 */
		bool Remove(const Iterator& first, const Iterator& last);

		/** Vector clear method.
		* It goes through the list and deletes its elements.
		*/
		void Clear();

		/** Returns an iterator to the first element in the vector.
		* @return Iterator to the first element in the vector.
		*/
		Iterator begin() const;

		/** Returns an iterator to the past-the-end element in the vector.
		* @return Iterator to the past-the-end element in the vector.
		*/
		Iterator end() const;

	private:
		void DeepCopy(const Vector& rhs);
		void ShiftLeftFrom(Iterator it);

		T* mFront;
		std::uint32_t mSize;
		std::uint32_t mCapacity;

		const static std::uint32_t DEFAULT_CAPACITY = 4U;
	};
#include "Vector.inl"

}
