#pragma once

#include "SList.h"
#include "Vector.h"
#include <cstdint>
#include <cstring>
#include <exception>


namespace Library
{
#pragma region Default Hash

	// generic default hash
	template<typename TKey>
	class DefaultHash
	{
	public:
		std::uint32_t operator()(const TKey& key);
	};

	template<>
	class DefaultHash<std::string>
	{
	public:
		std::uint32_t operator()(const std::string& key);
	};

	template<>
	class DefaultHash<char*>
	{
	public:
		std::uint32_t operator()(const char* key);
	};

#pragma endregion

#pragma region HashMap
	template <typename TKey, typename TData, typename HashFunctor = DefaultHash<TKey>>
	class HashMap final
	{
	public:
		typedef std::pair<const TKey, TData> PairType;

	private:
		typedef SList<PairType> ChainType;
		typedef Vector<ChainType> BucketType;

	public:
		class Iterator final
		{
			friend class HashMap;
		public:
			Iterator();
			~Iterator() = default;
			Iterator(const Iterator & rhs);
			Iterator& operator=(const Iterator& rhs);

			Iterator& operator++();
			Iterator operator++(int);
			bool operator==(const Iterator& rhs) const;
			bool operator!=(const Iterator& rhs) const;
			const PairType& operator*() const;
			PairType& operator*();
			const PairType* operator->() const;
			PairType* operator->();

		private:

			Iterator(const HashMap* owner, std::uint32_t bucketIndex, typename ChainType::Iterator chainIterator);

			const HashMap* mOwner;
			std::uint32_t mBucketIndex;
			typename ChainType::Iterator mChainIterator;
		};

		HashMap(std::uint32_t hashTableSize = DEFAULT_HASH_TABLE_SIZE);
		~HashMap();
		HashMap(const HashMap & rhs) = default;
		HashMap& operator=(const HashMap& rhs) = default;

		Iterator Find(const TKey & key) const;
		Iterator Insert(const PairType & pair);
		const TData& operator[](const TKey & key) const;
		TData& operator[](const TKey & key);
		bool Remove(const TKey & key);
		void Clear();
		bool ContainsKey(const TKey & key) const;
		std::uint32_t Size() const;
		bool IsEmpty() const;
		Iterator begin() const;
		Iterator end() const;

	private:

		BucketType mBuckets;
		std::uint32_t mSize;
		std::uint32_t mNumBuckets;

		const static std::uint32_t DEFAULT_HASH_TABLE_SIZE = 16U;
	};
#pragma endregion

#include "HashMap.inl"

}
