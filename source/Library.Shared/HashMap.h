#pragma once

#include "SList.h"
#include "Vector.h"
#include <cstdint>
#include <cstring>
#include <exception>


namespace Library
{
#pragma region Default Hash

	/** Default hash functor for the hash map.
	 * This default hash functor hashes an entry using its bytes.
	 */
	template<typename TKey>
	class DefaultHash
	{
	public:

		/** Default hash function.
		 * This default hash function hashes an entry using its bytes.
		 * @param key: Key to hash.
		 * @return Hash result.
		 */
		std::uint32_t operator()(const TKey& key);

	private:
		const static std::uint32_t HASH_NUMBER = 31U;

	};

	/** Standard string hash functor.
	 * Template specialization of the default hash functor for the standard string.
	 */
	template<>
	class DefaultHash<std::string>
	{
	public:

		/** Standard string hash function.
		 * This hash function hashes a string using its characters.
		 * @param key: String to hash.
		 * @return Hash result.
		 */
		std::uint32_t operator()(const std::string& key);

	private:
		const static std::uint32_t HASH_NUMBER = 31U;

	};

	/** Char* hash functor.
	* Template specialization of the default hash functor for char*.
	*/
	template<>
	class DefaultHash<char*>
	{
	public:
		/** Char* hash function.
		* This hash function hashes a char* using its characters.
		* @param key: Char* to hash.
		* @return Hash result.
		*/
		std::uint32_t operator()(const char* key);

	private:
		const static std::uint32_t HASH_NUMBER = 31U;

	};

#pragma endregion

#pragma region HashMap


	/** HashMap is a templated hash map that uses chaining.
	 * HashMap is implemented using a vector of Slists containing a key and a value pair.
	 * HashMap uses DefaultHash as the default hash functor if none is specified.
	 */
	template <typename TKey, typename TData, typename HashFunctor = DefaultHash<TKey>>
	class HashMap final
	{
	public:
		typedef std::pair<const TKey, TData> PairType;

	private:
		typedef SList<PairType> ChainType;
		typedef Vector<ChainType> BucketType;

	public:

		/** Iterator is an iterator class for HashMap.
		* This is a simple iterator implementation.
		*/
		class Iterator final
		{
			friend class HashMap;
		public:

			/** Iterator constructor.
			* It creates and initializes an iterator that belongs to no HashMap.
			*/
			Iterator();

			/** Iterator destructor.
			* It's the default implementation provided by the compiler.
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
			* It does a prefix incrementation of the iterator allowing it to point to the following element in the HashMap.
			* @exception It throws an exception if it does not belong to a HashMap.
			* @exception It throws an exception if it's going out of bounds.
			* @return Incremented iterator.
			*/
			Iterator& operator++();

			/** Iterator postfix incrementation operator overloading method.
			* It does a postfix incrementation of the iterator allowing it to point to the following element in the HashMap.
			* @exception It throws an exception if it does not belong to a HashMap.
			* @exception It throws an exception if it's going out of bounds.
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
			* @exception It throws an exception if it does not belong to a HashMap.
			* @return Key and data pair pointed to by the iterator.
			*/
			const PairType& operator*() const;

			/** Iterator dereference operator overloading method.
			* @exception It throws an exception if it does not belong to a HashMap.
			* @return Key and data pair pointed to by the iterator.
			*/
			PairType& operator*();

			/** Iterator dereference operator overloading method.
			* @exception It throws an exception if it does not belong to a HashMap.
			* @return Pointer to a key and data pair pointed to by the iterator.
			*/
			const PairType* operator->() const;

			/** Iterator dereference operator overloading method.
			* @exception It throws an exception if it does not belong to a HashMap.
			* @return Pointer to a key and data pair pointed to by the iterator.
			*/
			PairType* operator->();

		private:

			Iterator(const HashMap* owner, std::uint32_t bucketIndex, typename const ChainType::Iterator& chainIterator);

			const HashMap* mOwner;
			std::uint32_t mBucketIndex;
			typename ChainType::Iterator mChainIterator;
		};

		/** HashMap constructor.
		* It creates and initializes an empty HashMap.
		* If no capacity is specified, a default hash table size will be used.
		* @exception It throws an exception if the initial hash table size is zero.
		* @param hashTableSize: Initial hash table size.
		*/
		HashMap(std::uint32_t hashTableSize = DEFAULT_HASH_TABLE_SIZE);

		/** HashMap destructor.
		* It destroys the HashMap and its elements.
		*/
		~HashMap() = default;

		/** HashMap copy constructor.
		* It makes a deep copy of the right hand side HashMap.
		* @param rhs: HashMap to copy.
		*/
		HashMap(const HashMap & rhs) = default;

		/** HashMap assignment operator overloading method.
		* It allows the assignment operator to create a deep copy of the assignee HashMap.
		* @param rhs: HashMap to copy.
		* @return Result HashMap copy.
		*/
		HashMap& operator=(const HashMap& rhs) = default;

		/** Finds a pair in the HashMap using a key.
		* If the HashMap is empty or the key is not found, the past-the-end iterator is returned.
		* Key act as an output to the hashed key.
		* @param key: Key to the pair to find in the HashMap.
		* @param bucket: The bucket that the key is in.
		* @return Iterator to the found element in the HashMap.
		*/
		Iterator Find(const TKey & key, std::uint32_t& bucket) const;

		/** Finds a pair in the HashMap using a key.
		* If the HashMap is empty or the key is not found, the past-the-end iterator is returned.
		* @param key: Key to the pair to find in the HashMap.
		* @return Iterator to the found element in the HashMap.
		*/
		Iterator Find(const TKey & key) const;

		/** Adds a new pair to the HashMap.
		* If the HashMap already contains the passed key, an iterator to the pair of that key is returned.
		* @param pair: Pair to add to the HashMap.
		* @return Iterator to the added pair or the already existent pair.
		*/
		Iterator Insert(const PairType & pair);

		/** HashMap const version of the bracket operator overloading method.
		* It returns the value of the passed key.
		* @exception It throws an exception if the key does not exist.
		* @param key: Key of the wanted pair.
		* @return Value of the wanted data.
		*/
		const TData& operator[](const TKey & key) const;

		/** HashMap non-const version of the bracket operator overloading method.
		* It returns the value of the passed key or adds it to the HashMap if it's not existent already.
		* @param key: Key of the wanted pair.
		* @return Value of the wanted or added data.
		*/
		TData& operator[](const TKey & key);

		/** Removes a pair from the HashMap.
		* It finds a pair in the HashMap using the passed key and removes it.
		* If the key is not found, nothing happens.
		* @param t: Key of the pair to remove from the HashMap.
		* @return Boolean that represents the success or failure of the method.
		*/
		bool Remove(const TKey & key);

		/** HashMap clear method.
		* It empties the HashMap by going through the latter and deleting all the existent pairs.
		*/
		void Clear();

		/** Checks if a key exists in the HashMap.
		* @param t: Key to check.
		* @return Boolean that represents if the key exists in the HashMap or not.
		*/
		bool ContainsKey(const TKey & key) const;

		/** Returns the current size of the HashMap.
		* @return Current HashMap size.
		*/
		std::uint32_t Size() const;

		/** Determines if the HashMap is empty.
		*/
		bool IsEmpty() const;

		/** Returns an iterator to the first pair in the HashMap.
		* @return Iterator to the first element in the HashMap.
		*/
		Iterator begin() const;

		/** Returns an iterator to the past-the-end element in the HashMap.
		* @return Iterator to the past-the-end element in the HashMap.
		*/
		Iterator end() const;

	private:

		BucketType mBuckets;
		std::uint32_t mSize;

		const static std::uint32_t DEFAULT_HASH_TABLE_SIZE = 16U;
	};
#pragma endregion

#include "HashMap.inl"

}
