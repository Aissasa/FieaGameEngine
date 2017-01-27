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
			Iterator(Vector* owner, std::uint32_t index);
			Vector* mOwner;
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

		const static std::uint32_t DEFAULT_CAPACITY = 4U;
	};
#include "Vector.inl"

}
