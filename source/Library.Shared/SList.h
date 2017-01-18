#pragma once

namespace FieaGameEngine
{
	template <typename T>
	class SList
	{
	public:
		SList();
		SList(const SList& rhs);
		SList& operator=(const SList& rhs);
		~SList();
		void PushFront(const T& t);
		void PopFront();
		void PushBack(const T& t);
		bool IsEmpty();
		T& Front();
		T& Back();
		std::uint32_t Size();
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