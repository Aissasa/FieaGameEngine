#pragma once

namespace UnitTestLibraryDesktop
{
	class FooHashFunctor
	{
	public:
		std::uint32_t operator()(const Foo& key);
	};

	class Foo
	{
	public:
		Foo();
		Foo(int data, int* intPtr);
		Foo(const Foo& rhs);
		Foo& operator=(const Foo& rhs);
		~Foo();

		bool operator==(const Foo& rhs) const;
		int Data() const;
		int* IntPtr() const;

	private:
		int mData;
		int* mIntPtr;
	};
}
