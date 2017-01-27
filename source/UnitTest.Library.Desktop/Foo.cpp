#include "pch.h"
#include "Foo.h"

namespace UnitTestLibraryDesktop
{
	Foo::Foo():
		Foo(0, nullptr)
	{
	}

	Foo::Foo(int data, int * pointer):
		mData(data), mIntPtr(pointer)
	{
	}

	Foo::Foo(const Foo& rhs):
		mData(rhs.mData), mIntPtr(nullptr)
	{
		mIntPtr = new int;
		*mIntPtr = *(rhs.mIntPtr);
	}

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

	Foo::~Foo()
	{
		delete mIntPtr;
	}

	bool Foo::operator==(const Foo & rhs) const
	{
		return mData==rhs.mData && *mIntPtr==*(rhs.mIntPtr);
	}

	int Foo::Data()
	{
		return mData;
	}

	int * Foo::IntPtr()
	{
		return mIntPtr;
	}
}