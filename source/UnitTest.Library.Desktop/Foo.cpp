#include "pch.h"
#include "Foo.h"

namespace UnitTestLibraryDesktop
{
	/************************************************************************/
	Foo::Foo():
		Foo(0, nullptr)
	{
	}

	/************************************************************************/
	Foo::Foo(int data, int* pointer):
		mData(data), mIntPtr(pointer)
	{
	}

	/************************************************************************/
	Foo::Foo(const Foo& rhs):
		mData(rhs.mData), mIntPtr(nullptr)
	{
		if (rhs.mIntPtr != nullptr)
		{
			mIntPtr = new int;
			*mIntPtr = *(rhs.mIntPtr);
		}
	}

	/************************************************************************/
	Foo & Foo::operator=(const Foo & rhs)
	{
		if (this != &rhs)
		{
			delete mIntPtr;
			mData = rhs.mData;
			mIntPtr = new int(*(rhs.mIntPtr));
		}

		return *this;
	}

	/************************************************************************/
	Foo::~Foo()
	{
		delete mIntPtr;
	}

	/************************************************************************/
	bool Foo::operator==(const Foo & rhs) const
	{
		return mData==rhs.mData && *mIntPtr==*(rhs.mIntPtr);
	}

	/************************************************************************/
	int Foo::Data() const
	{
		return mData;
	}

	/************************************************************************/
	int * Foo::IntPtr() const
	{
		return mIntPtr;
	}

	/************************************************************************/
	std::uint32_t FooHashFunctor::operator()(const Foo& key)
	{
		int data = key.Data();
		int* dataPtr = key.IntPtr();

		std::uint32_t hash = 0;

		const unsigned char* pointer = reinterpret_cast<const unsigned char*>(&data);

		for (std::uint32_t i = 0; i < sizeof(data); ++i)
		{
			hash = hash * 31 + pointer[i];
		}

		if (dataPtr)
		{
			pointer = reinterpret_cast<const unsigned char*>(dataPtr);

			for (std::uint32_t i = 0; i < sizeof(*dataPtr); ++i)
			{
				hash = hash * 31 + pointer[i];
			}
		}

		return hash;
	}
}
