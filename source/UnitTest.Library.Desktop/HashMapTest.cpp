#include "pch.h"
#include "CppUnitTest.h"
#include "HashMap.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	typedef std::uint32_t std_uint32;
	typedef std::uint8_t std_uchar;

	TEST_CLASS(HashMapTest)
	{
	public:
#pragma region Memory leak check
		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#pragma endregion

#pragma region Hash Functor

#pragma region Template Methods

		template<typename T>
		void HashFunctorTestMethod(T& t1, T& t2)
		{
			DefaultHash<T> hasher;

			T t3(t1);
			std_uint32 h1 = hasher(t1);
			std_uint32 h2 = hasher(t2);
			std_uint32 h3 = hasher(t3);

			Assert::IsTrue(t1 == t3);
			Assert::AreEqual(h1, hasher(t1));
			Assert::AreEqual(h2, hasher(t2));
			Assert::AreEqual(h3, hasher(t3));
			Assert::AreNotEqual(h1, h2);
			Assert::AreEqual(h1, h3);
		}

#pragma endregion

#pragma region Test Methods
		/************************************************************************/
		TEST_METHOD(HashFunctorTest)
		{
			// uint
			std_uint32 num1 = 5;
			std_uint32 num2 = 3;
			HashFunctorTestMethod(num1, num2);

			// uint ptr
			std_uint32* numPtr1 = new std_uint32(5);
			std_uint32* numPtr2 = new std_uint32(9);
			HashFunctorTestMethod(numPtr1, numPtr2);
			delete numPtr1;
			delete numPtr2;

			// float
			float f1 = 5.6f;
			float f2 = 3.3f;
			HashFunctorTestMethod(f1, f2);

			// char
			std_uchar c1 = 'a';
			std_uchar c2 = 'j';
			HashFunctorTestMethod(c1, c2);

			// char*
			char* ch1 = "Yo";
			char* ch2 = "Hello World";
			HashFunctorTestMethod(ch1, ch2);

			// string
			std::string str1 = "Yo Yo";
			std::string str2 = "Hello Paul";
			HashFunctorTestMethod(str1, str2);

			// foo
			int a1 = 6;
			int a2 = 9;
			int* a3 = new int(8);
			int* a4 = new int(564);

			Foo foo1(a1, a3);
			Foo foo2(a2, a4);
			FooHashFunctor hasher;

			Foo foo3(foo1);
			std_uint32 h1 = hasher(foo1);
			std_uint32 h2 = hasher(foo2);
			std_uint32 h3 = hasher(foo3);

			Assert::IsTrue(foo1 == foo3);
			Assert::AreEqual(h1, hasher(foo1));
			Assert::AreEqual(h2, hasher(foo2));
			Assert::AreEqual(h3, hasher(foo3));
			Assert::AreNotEqual(h1, h2);
			Assert::AreEqual(h1, h3);
		}

#pragma endregion

#pragma endregion

#pragma region HashMap

#pragma region Template Methods



#pragma endregion

#pragma region Template Methods

		TEST_METHOD(Ctor)
		{
			HashMap<int, int> c(10);
		}

#pragma endregion

#pragma endregion

#pragma region Iterator

		TEST_METHOD(IteratorConstructorsAndAssignmentTest)
		{
			HashMap<int, int> hashmap(10);
			std::pair<int, int> pair(16, 5);
			HashMap<int, int>::Iterator it1 = hashmap.Insert(pair);
			HashMap<int, int>::Iterator it2(it1);
			Assert::IsTrue(it1 == it2); // copy ctor

			HashMap<int, int>::Iterator it3;
			HashMap<int, int>::Iterator it4;
			Assert::IsTrue(it3 == it4); // ctor

			it3 = it1;
			it4 = it2;
			Assert::IsTrue(it1 == it4 && it2 == it3); // assignment operator
		}

		TEST_METHOD(IteratorPrefixIncrementationOperatorTest)
		{
			HashMap<int, int>::Iterator it;
			auto func = [&it] { ++it; };
			Assert::ExpectException<std::exception>(func);

			HashMap<int, int> hashmap(10);
			HashMap<int, int>::Iterator it0 = hashmap.begin();
			auto func0 = [&it0] { ++it0; };
			Assert::ExpectException<std::exception>(func0);

			HashMap<int, int>::Iterator it1 = hashmap.Insert(std::pair<int, int>(2, 98));
			HashMap<int, int>::Iterator it2 = hashmap.Insert(std::pair<int, int>(2, 68));
			HashMap<int, int>::Iterator it3 = hashmap.Insert(std::pair<int, int>(3, 9));
			Assert::IsTrue(it1 != it2);
			Assert::IsTrue(it3 != it2);

			HashMap<int, int>::Iterator it4 = ++it2;
			Assert::IsTrue(it2 == it3);
			Assert::IsTrue(it2 == it4);
			Assert::IsTrue(++it4 == hashmap.end());
		}

		TEST_METHOD(IteratorPostfixIncrementationOperatorTest)
		{
			HashMap<int, int>::Iterator it;
			auto func = [&it] { it++; };
			Assert::ExpectException<std::exception>(func);

			HashMap<int, int> hashmap(10);
			HashMap<int, int>::Iterator it0 = hashmap.begin();
			auto func0 = [&it0] { it0++; };
			Assert::ExpectException<std::exception>(func0);


			HashMap<int, int>::Iterator it1 = hashmap.Insert(std::pair<int, int>(2, 98));
			HashMap<int, int>::Iterator it2 = hashmap.Insert(std::pair<int, int>(2, 68));

			HashMap<int, int>::Iterator it3 = it1++;
			Assert::IsTrue(it1 == it2);
			Assert::IsTrue(it1 != it3);
		}

		TEST_METHOD(IteratorEqualAndNotOperatorTest)
		{
			HashMap<int, int> hashmap(15);

			HashMap<int, int>::Iterator it1 = hashmap.Insert(std::pair<int, int>(52, 98));
			HashMap<int, int>::Iterator it2 = hashmap.Insert(std::pair<int, int>(23, 68));

			Assert::IsFalse(it1 == it2);
			Assert::IsTrue(it1 != it2);
			++it1;
			Assert::IsTrue(it1 == it2);
			Assert::IsFalse(it1 != it2);
		}


#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState HashMapTest::sStartMemState;

}