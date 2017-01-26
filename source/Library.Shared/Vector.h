#pragma once

namespace Library
{
	template <typename T>
	class Vector
	{
	private:
		;

	public:
		class VectorIterator
		{
			friend class Vector;
		public:
			VectorIterator();
			~VectorIterator() = default;
			VectorIterator(const VectorIterator & rhs);
			VectorIterator& operator=(const VectorIterator& rhs);

			VectorIterator operator++();
			VectorIterator operator++(int);
			//VectorIterator operator--();
			//VectorIterator operator--(int);
			bool operator==(const VectorIterator& rhs) const;
			bool operator!=(const VectorIterator& rhs) const;
			const T& operator*() const;
			T& operator*();

		private:
			VectorIterator(Vector owner, std::uint32_t index);
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
		VectorIterator Find(const T& t) const;

		VectorIterator PushBack(const T& t);
		void PopBack();
		bool Remove(const T& t);
		bool Remove(const VectorIterator& first, const VectorIterator& last);
		void Clear();

		VectorIterator begin();
		VectorIterator end();

	private:
		std::uint32_t mSize;
		std::uint32_t mCapacity;

		std::uint32_t DEFAULT_CAPACITY = 4;
	};
#include "Vector.inl"

}
