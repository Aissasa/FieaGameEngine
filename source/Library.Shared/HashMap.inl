#pragma once

#include "HashMap.h"

#pragma region Iterator

/************************************************************************/
template <typename TKey, typename TData, typename HashFunctor>
HashMap<TKey, TData, HashFunctor>::Iterator::Iterator(const HashMap* owner, std::uint32_t bucketIndex, typename ChainType::Iterator chainIterator) :
	mOwner(owner), mBucketIndex(bucketIndex), mChainIterator(chainIterator)
{
}

/************************************************************************/
template <typename TKey, typename TData, typename HashFunctor>
HashMap<TKey, TData, HashFunctor>::Iterator::Iterator() :
	Iterator(nullptr, 0, ChainType::Iterator())
{
}

/************************************************************************/
template <typename TKey, typename TData, typename HashFunctor>
HashMap<TKey, TData, HashFunctor>::Iterator::Iterator(const Iterator& rhs) :
	Iterator(rhs.mOwner, rhs.mBucketIndex, rhs.mChainIterator)
{
}

/************************************************************************/
template <typename TKey, typename TData, typename HashFunctor>
typename HashMap<TKey, TData, HashFunctor>::Iterator& HashMap<TKey, TData, HashFunctor>::Iterator::operator=(const Iterator& rhs)
{
	if (this != &rhs)
	{
		mOwner = rhs.mOwner;
		mBucketIndex = rhs.mBucketIndex;
		mChainIterator = rhs.mChainIterator;
	}

	return *this;
}

/************************************************************************/
template <typename TKey, typename TData, typename HashFunctor>
typename HashMap<TKey, TData, HashFunctor>::Iterator& HashMap<TKey, TData, HashFunctor>::Iterator::operator++()
{
	if (mOwner == nullptr)
	{
		throw std::exception("The iterator is not assigned to a hash map.");
	}

	if (*this == mOwner->end())
	{
		throw std::exception("The iterator is going out of range.");
	}

	++mChainIterator;

	while (mChainIterator == mOwner->mBuckets[mBucketIndex].end())
	{
		if (++mBucketIndex == mOwner->mNumBuckets)
		{
			mChainIterator = ChainType::Iterator::Iterator();
			break;
		}
		mChainIterator = mOwner->mBuckets[mBucketIndex].begin();
	}

	return *this;
}

/************************************************************************/
template <typename TKey, typename TData, typename HashFunctor>
typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Iterator::operator++(int)
{
	Iterator temp = *this;
	++(*this);
	return temp;
}

/************************************************************************/
template <typename TKey, typename TData, typename HashFunctor>
inline bool HashMap<TKey, TData, HashFunctor>::Iterator::operator==(const Iterator& rhs) const
{
	return mOwner == rhs.mOwner && mBucketIndex == rhs.mBucketIndex && mChainIterator == rhs.mChainIterator;
}

/************************************************************************/
template <typename TKey, typename TData, typename HashFunctor>
inline bool HashMap<TKey, TData, HashFunctor>::Iterator::operator!=(const Iterator& rhs) const
{
	return !(*this == rhs);
}

/************************************************************************/
template <typename TKey, typename TData, typename HashFunctor>
const std::pair<const TKey, TData>& HashMap<TKey, TData, HashFunctor>::Iterator::operator*() const
{
	if (mOwner == nullptr)
	{
		throw std::exception("The iterator is not assigned to a hash map.");
	}

	return *mChainIterator;
}

/************************************************************************/
template <typename TKey, typename TData, typename HashFunctor>
std::pair<const TKey, TData>& HashMap<TKey, TData, HashFunctor>::Iterator::operator*()
{
	return const_cast<PairType&>(const_cast<const HashMap<TKey, TData, HashFunctor>::Iterator&>(*this).operator*());
}

/************************************************************************/
template <typename TKey, typename TData, typename HashFunctor>
const std::pair<const TKey, TData>* HashMap<TKey, TData, HashFunctor>::Iterator::operator->() const
{
	if (mOwner == nullptr)
	{
		throw std::exception("The iterator is not assigned to a hash map.");
	}

	return &(*mChainIterator);
}

/************************************************************************/
template <typename TKey, typename TData, typename HashFunctor>
std::pair<const TKey, TData>* HashMap<TKey, TData, HashFunctor>::Iterator::operator->()
{
	return const_cast<PairType*>(const_cast<const HashMap<TKey, TData, HashFunctor>::Iterator&>(*this).operator->());
}

#pragma endregion

#pragma region HashMap

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
HashMap<TKey, TData, HashFunctor>::HashMap(std::uint32_t hashTableSize) :
	mBuckets(Vector<ChainType>(hashTableSize)), mSize(0), mNumBuckets(hashTableSize)
{
	if (hashTableSize == 0)
	{
		throw std::exception("The hash map should have more buckets!");
	}

	// initialize the vector
	for (uint32_t i = 0; i <mNumBuckets; ++i)
	{
		mBuckets.PushBack(SList<PairType>());
	}
}

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
HashMap<TKey, TData, HashFunctor>::~HashMap()
{
	Clear();
	mBuckets.Clear();
}

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Find(const TKey& key) const
{
	Iterator it = begin();
	for (; it != end(); ++it)
	{
		if ((*it).first == key)
		{
			break;
		}
	}
	return it;
}

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::Insert(const PairType & pair)
{
	Iterator it = Find(pair.first);
	if (it == end())
	{
		static HashFunctor hashFunc;

		std::uint32_t bucket = hashFunc(pair.first) % mNumBuckets;
		ChainType::Iterator chainIterator = mBuckets[bucket].PushBack(pair);
		++mSize;
		it = Iterator(this, bucket, chainIterator);
	}
	return it;
}

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
const TData& HashMap<TKey, TData, HashFunctor>::operator[](const TKey& key) const
{
	Iterator it = Find(key);
	if (it == end())
	{
		throw std::exception("Invalid entry. This element does not exist.");
	}
	return (*it).second;
}

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
TData& HashMap<TKey, TData, HashFunctor>::operator[](const TKey& key)
{
	Iterator it = Find(key);
	if (it == end())
	{
		PairType p(key, TData());
		it = Insert(p);
	}
	return (*it).second;
}

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
bool HashMap<TKey, TData, HashFunctor>::Remove(const TKey & key)
{
	Iterator it = Find(key);
	if (it == end())
	{
		return false;
	}
	else
	{
		--mSize;
		return mBuckets[it.mBucketIndex].Remove(*it);
	}
}

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
void HashMap<TKey, TData, HashFunctor>::Clear()
{
	BucketType::Iterator it = mBuckets.begin();
	while (it != mBuckets.end())
	{
		(*(it++)).Clear();
	}
	mSize = 0;
}

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
bool HashMap<TKey, TData, HashFunctor>::ContainsKey(const TKey & key) const
{
	return Find(key) != end();
}

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
std::uint32_t HashMap<TKey, TData, HashFunctor>::Size() const
{
	return mSize;
}

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
inline bool HashMap<TKey, TData, HashFunctor>::IsEmpty() const
{
	return mSize == 0;
}

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::begin() const
{
	if (IsEmpty())
	{
		return end();
	}

	std::uint32_t index = 0;
	while (mBuckets[index].IsEmpty())
	{
		index++;
	}

	return Iterator(this, index, mBuckets[index].begin());
}

/************************************************************************/
template<typename TKey, typename TData, typename HashFunctor>
typename HashMap<TKey, TData, HashFunctor>::Iterator HashMap<TKey, TData, HashFunctor>::end() const
{
	return Iterator(this, mNumBuckets, ChainType::Iterator::Iterator());
}

#pragma endregion

#pragma region Default Hash

/************************************************************************/
template<typename TKey>
std::uint32_t DefaultHash<TKey>::operator()(const TKey& key)
{
	const unsigned char* pointer = reinterpret_cast<const unsigned char*>(&key);
	std::uint32_t hash = 0;
	for (std::uint32_t i = 0; i < sizeof(key); ++i)
	{
		hash = hash * 31 + pointer[i];
	}

	return hash;
}

/************************************************************************/
std::uint32_t DefaultHash<std::string>::operator()(const std::string& key)
{
	std::uint32_t hash = 0;
	for (std::uint32_t i = 0; i < key.length(); ++i)
	{
		hash = hash * 31 + key[i];
	}

	return hash;
}

/************************************************************************/
std::uint32_t DefaultHash<char*>::operator()(const char* key)
{
	std::uint32_t hash = 0;
	for (std::uint32_t i = 0; i < strlen(key); ++i)
	{
		hash = hash * 31 + key[i];
	}

	return hash;
}

#pragma endregion
