#pragma once

namespace UnitTestLibraryDesktop
{
	class Foo
	{
	public:
		Foo();
		Foo(int data, int* intPtr);
		Foo(const Foo& rhs);
		Foo& operator=(const Foo& rhs);
		~Foo();

		bool operator==(const Foo& rhs) const;
		int Data();
		int* IntPtr();

	private:
		int mData;
		int* mIntPtr;
	};
}