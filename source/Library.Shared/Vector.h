#pragma once

namespace Library
{
	template <typename T>
	class Vector
	{
	private:
		;

	public:
		class Iterator
		{
			friend class Vector;
		public:
			Iterator();
			~Iterator() = default;
			Iterator(const Iterator & rhs);
			Iterator& operator=(const Iterator& rhs);

			Iterator& operator++();
			Iterator operator++(int);
			Iterator operator--();
			Iterator operator--(int);
			bool operator==(const Iterator& rhs) const;
			bool operator!=(const Iterator& rhs) const;
			const T& operator*() const;
			T& operator*();

		private:
			Iterator(Vector owner, std::uint32_t index);
			Vector mOwner;
			std::uint32_t mIndex;
		};

		Vector();
		Vector(std::uint32_t initCapacity);
		~Vector();
		Vector(const Vector & rhs);
		Vector& operator=(const Vector& rhs);

		bool IsEmpty() const;
		std::uint32_t Size() const;
		std::uint32_t Capcity() const;
		bool Reserve(std::uint32_t newCapacity);

		const T& operator[](int index) const;
		T& operator[](int index);
		const T& At(const std::uint32_t index) const;
		T& At(const std::uint32_t index);
		const T& Front() const;
		T& Front();
		const T& Back() const;
		T& Back();
		Iterator Find(const T& t) const;

		Iterator PushBack(const T& t);
		void PopBack();
		bool Remove(const T& t);
		bool Remove(const Iterator& first, const Iterator& last);
		void Clear();

		Iterator begin();
		Iterator end();

	private:
		void DeepCopy(const Vector& rhs);

		T* mFront;
		std::uint32_t mSize;
		std::uint32_t mCapacity;

		std::uint32_t DEFAULT_CAPACITY = 4U;
	};
#include "Vector.inl"

#pragma region Vector
	/************************************************************************/
	template<typename T>
	Vector<T>::Vector(std::uint32_t initCapacity = DEFAULT_CAPACITY) :
		mSize(0), mCapacity(0), mFront(nullptr)
	{
		Reserve(initCapacity);
	}

	/************************************************************************/
	template<typename T>
	Vector<T>::Vector() :
		Vector(DEFAULT_CAPACITY)
	{
	}

	/************************************************************************/
	template<typename T>
	Vector<T>::~Vector()
	{
		Clear();
		free(nFront);
	}

	/************************************************************************/
	template<typename T>
	Vector<T>::Vector(const Vector & rhs) :
		Vector(rhs.mCapacity)
	{
		DeepCopy(rhs);
	}

	/************************************************************************/
	template<typename T>
	Vector<T> & Vector<T>::operator=(const Vector & rhs)
	{
		if (this != &rhs)
		{
			Clear();
			Reserve(rhs.mCapacity);
			DeepCopy(rhs);
		}
	}

	/************************************************************************/
	template<typename T>
	inline bool Vector<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	/************************************************************************/
	template<typename T>
	inline std::uint32_t Vector<T>::Size() const
	{
		return mSize;
	}

	/************************************************************************/
	template<typename T>
	inline std::uint32_t Vector<T>::Capcity() const
	{
		return mCapacity;
	}

	/************************************************************************/
	template<typename T>
	bool Vector<T>::Reserve(std::uint32_t newCapacity)
	{
		if (newCapacity < mCapacity)
		{
			return false;
		}
		else
		{
			mFront = (T*)realloc(mFront, newCapacity * sizeof(T));
			if (mFront == nullptr)
			{
				throw std::exception("Allocation failed!");
			}
			mCapacity = newCapacity;
			return true;
		}
	}

	/************************************************************************/
	template<typename T>
	const T & Vector<T>::operator[](int index) const
	{
		if (IsEmpty())
		{
			throw std::exception("The vector is empty!");
		}

		if (index < 0 || index >= mSize)
		{
			throw std::exception("Index out of bounds!");
		}

		return mFront[index];
	}

	/************************************************************************/
	template<typename T>
	T & Vector<T>::operator[](int index)
	{
		return const_cast<T&>(const_cast<const Vector&>(*this).operator[](index));
	}

	/************************************************************************/
	template<typename T>
	const T & Vector<T>::At(const std::uint32_t index) const
	{
		return operator[](index);
	}

	/************************************************************************/
	template<typename T>
	T & Vector<T>::At(const std::uint32_t index)
	{
		return const_cast<T&>(const_cast<const Vector&>(*this).At(index));
	}

	/************************************************************************/
	template<typename T>
	inline const T & Vector<T>::Front() const
	{
		return *mFront;
	}

	/************************************************************************/
	template<typename T>
	T & Vector<T>::Front()
	{
		return const_cast<T&>(const_cast<const Vector&>(*this).Front());
	}

	/************************************************************************/
	template<typename T>
	const T & Vector<T>::Back() const
	{
		return At(mSize - 1);
	}

	/************************************************************************/
	template<typename T>
	T & Vector<T>::Back()
	{
		return const_cast<T&>(const_cast<const Vector&>(*this).Back());
	}

	/************************************************************************/
	template<typename T>
	typename Vector<T>::Iterator Vector<T>::Find(const T & t) const
	{
		Iterator it = begin();
		for (; it != end(); ++it)
		{
			if (*it == t)
			{
				break;
			}
		}
		return it;
	}

	/************************************************************************/
	template<typename T>
	typename Vector<T>::Iterator Vector<T>::PushBack(const T & t)
	{
		if (mSize == mCapacity)
		{
			Reserve(mCapacity * 2);
		}
		mFront[mSize] = new(mFront + mSize) T;
		++mSize;

		return Iterator(this, mSize - 1);
	}

	/************************************************************************/
	template<typename T>
	void Vector<T>::PopBack()
	{
		if (!IsEmpty())
		{
			mFront[--mSize].~T();
		}
	}

	/************************************************************************/
	template<typename T>
	bool Vector<T>::Remove(const T & t)
	{
		Iterator it = Find(t);
		if (it == end())
		{
			return false;
		}
		else
		{
			Iterator last(this, mSize - 1);
			while (it != last)
			{
				*it = *(++it);
			}

			PopBack();
		}
	}

	/************************************************************************/
	template<typename T>
	bool Vector<T>::Remove(const Iterator & first, const Iterator & last)
	{
		if (first.mIndex >= mSize || last.mIndex > mSize)
		{
			throw std::exception("Iterators are out of bounds!");
		}

		if (first.mIndex >= last.mIndex )
		{
			return false;
		}

		if (last == end())
		{
			std::uint32_t count = last.mIndex - first.index;
			for (; count > 0; --i)
			{
				PopBack();
			}
		}
		else
		{
			// todo think about swapping the end and the area to delete
		}

		return true;
	}

	/************************************************************************/
	template<typename T>
	void Vector<T>::Clear()
	{
		while (!IsEmpty())
		{
			PopBack();
		}
	}

	/************************************************************************/
	template<typename T>
	typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Iterator(this, 0);
	}

	/************************************************************************/
	template<typename T>
	typename Vector<T>::Iterator Vector<T>::end()
	{
		if (IsEmpty())
		{
			return begin();
		}
		return Iterator(this, mSize);
	}

	/************************************************************************/
	template<typename T>
	void Vector<T>::DeepCopy(const Vector & rhs)
	{
		for (auto& value : rhs)
		{
			PushBack(value);
		}
	}
}
