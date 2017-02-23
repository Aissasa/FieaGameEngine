#pragma once

//#include "RTTI.h"

namespace UnitTestLibraryDesktop
{
	class Foo /*: public Library::RTTI*/
	{
	public:
		Foo();
		Foo(const int data,int* intPtr);
		Foo(const Foo& rhs);
		Foo& operator=(const Foo& rhs);
		~Foo();

		bool operator==(const Foo& rhs) const;
		bool operator!=(const Foo& rhs) const;
		int Data() const;
		int* IntPtr() const;

	private:
		int mData;
		int* mIntPtr;
	};

	class FooHashFunctor
	{
	public:
		std::uint32_t operator()(const Foo& key);

	private:
		const static std::uint32_t HASH_NUMBER = 31U;

	};

}
