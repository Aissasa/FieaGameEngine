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

		/************************************************************************/
		template<typename TKey, typename TData>
		void HashMapConstructorsMethod(TKey key, TData data)
		{
			// default ctor
			HashMap<TKey, TData> hashmap;
			Assert::IsTrue(hashmap.IsEmpty());
	
			auto func1 = [] { HashMap<TKey, TData> hashmap1(0); };
			Assert::ExpectException<std::exception>(func1);

			// copy ctor
			std::pair<const TKey, TData> pair(key, data);
			hashmap.Insert(pair);

			HashMap<TKey, TData> hashmap2 = hashmap;
			hashmap.Clear();
			Assert::IsTrue(hashmap.IsEmpty());
			Assert::IsFalse(hashmap2.IsEmpty());
			Assert::AreEqual(hashmap2.Size(), 1u);
			Assert::IsTrue(hashmap2.ContainsKey(pair.first));

			// asign operator
			hashmap = hashmap2;
			Assert::IsFalse(hashmap.IsEmpty());
			Assert::AreEqual(hashmap.Size(), 1u);
			Assert::IsTrue(hashmap.ContainsKey(pair.first));
		}

		/************************************************************************/
		template<typename TKey, typename TData>
		void HashMapIsEmptyAndContainsKeyMethod(TKey& key, TData& data)
		{
			HashMap<TKey, TData> hashmap;
			Assert::IsTrue(hashmap.IsEmpty());

			std::pair<const TKey, TData> pair(key, data);
			hashmap.Insert(pair);
			Assert::IsFalse(hashmap.IsEmpty());
			Assert::IsTrue(hashmap.ContainsKey(pair.first));

			hashmap.Clear();
			Assert::IsTrue(hashmap.IsEmpty());
			Assert::IsFalse(hashmap.ContainsKey(pair.first));
		}

		/************************************************************************/
		template<typename TKey, typename TData>
		void HashMapSizeMethod(TKey& key, TData& data)
		{
			HashMap<TKey, TData> hashmap;
			std::pair<const TKey, TData> pair(key, data);
			hashmap.Insert(pair);

			Assert::AreEqual(hashmap.Size(), 1u);
		}

		/************************************************************************/
		template<typename TKey, typename TData>
		void HashMapFindMethod(TKey& key, TData& data)
		{
			HashMap<TKey, TData> hashmap;
			std::pair<const TKey, TData> pair(key, data);

			Assert::IsTrue(hashmap.Find(key) == hashmap.end());

			hashmap.Insert(pair);
			auto it = hashmap.Find(key);
			Assert::IsTrue(hashmap.Find(key) != hashmap.end());
			Assert::IsTrue(*it == pair);
		}

		/************************************************************************/
		template<typename TKey, typename TData>
		void HashMapInsertAndClearMethod(TKey& key, TKey& key2, TData& data, TData& data2)
		{
			HashMap<TKey, TData> hashmap;
			std::pair<const TKey, TData> pair(key, data);
			auto it = hashmap.Insert(pair);
			Assert::AreEqual(hashmap.Size(), 1u);
			Assert::IsTrue(hashmap.ContainsKey(key));

			std::pair<const TKey, TData> pair2(key, data2);
			it = hashmap.Insert(pair2);
			Assert::AreEqual(hashmap.Size(), 1u);
			Assert::IsTrue(hashmap.ContainsKey(key));
			Assert::IsTrue((*it).second == data);
			Assert::IsFalse((*it).second == data2);

			std::pair<const TKey, TData> pair3(key2, data2);
			it = hashmap.Insert(pair3);
			Assert::AreEqual(hashmap.Size(), 2u);
			Assert::IsTrue(hashmap.ContainsKey(key2));

			hashmap.Clear();
			Assert::IsTrue(hashmap.IsEmpty());
			Assert::IsFalse(hashmap.ContainsKey(key));
			Assert::IsFalse(hashmap.ContainsKey(key2));
		}

		/************************************************************************/
		template<typename TKey, typename TData>
		void HashMapBracketOperatorMethod(TKey& key, TKey& key2, TData& data)
		{
			const HashMap<TKey, TData> constHashmap;
			auto func1 = [&constHashmap, key] { constHashmap[key]; };
			Assert::ExpectException<std::exception>(func1);

			HashMap<TKey, TData> hashmap;
			std::pair<const TKey, TData> pair(key2, data);
			hashmap[key];

			Assert::IsFalse(hashmap.IsEmpty());
			Assert::IsTrue(hashmap.ContainsKey(key));

			hashmap.Insert(pair);
			const HashMap<TKey, TData>constHashmap2 = hashmap;

			Assert::AreEqual(constHashmap2.Size(), 2u);
			Assert::IsTrue(constHashmap2[key2] == data);
			Assert::IsTrue(hashmap[key2] == data);
		}

		/************************************************************************/
		template<typename TKey, typename TData>
		void HashMapRemoveMethod(TKey& key, TData& data)
		{
			HashMap<TKey, TData> hashmap;
			std::pair<const TKey, TData> pair(key, data);
			hashmap.Insert(pair);
			bool b = hashmap.Remove(key);
			Assert::IsTrue(b);
			Assert::IsTrue(hashmap.IsEmpty());
			Assert::IsFalse(hashmap.ContainsKey(key));

			b = hashmap.Remove(key);
			Assert::IsFalse(b);
		}

		/************************************************************************/
		template<typename TKey, typename TData>
		void HashMapBeginAndEndMethod(TKey& key, TData& data)
		{
			HashMap<TKey, TData> hashmap;
			Assert::IsTrue(hashmap.begin() == hashmap.end());

			std::pair<const TKey, TData> pair(key, data);
			auto it = hashmap.Insert(pair);

			Assert::IsTrue(hashmap.begin() == it);
		}

#pragma endregion

#pragma region Test Methods

		/************************************************************************/
		TEST_METHOD(HashMapConstructorsTestMethod)
		{
			// uint
			std_uint32 a1 = 5;
			std_uint32 a2 = 6;
			HashMapConstructorsMethod(a1, a2);

			// uint ptr
			std_uint32* numPtr1 = new std_uint32(5);
			std_uint32* numPtr2 = new std_uint32(9);
			HashMapConstructorsMethod(numPtr1, numPtr2);
			delete numPtr1;
			delete numPtr2;

			// char
			std_uchar c1 = 'g';
			std_uchar c2 = 'p';
			HashMapConstructorsMethod(c1, c2);

			// char*
			char* ch1 = "Yo";
			char* ch2 = "Hello World";
			HashMapConstructorsMethod(ch1, ch2);

			// string
			std::string str1 = "yo yo";
			std::string str2 = "hello Paul";
			HashMapConstructorsMethod(str1, str2);

			HashMapConstructorsMethod(str1, a1);
			HashMapConstructorsMethod(a2, str2);

			// foo
			int n1 = 6;
			int n2 = 9;
			int* n3 = new int(8);
			int* n4 = new int(564);

			Foo foo1(n1, n3);
			Foo foo2(n2, n4);

			// default ctor
			HashMap<Foo, Foo> hashmap;
			Assert::IsTrue(hashmap.IsEmpty());

			auto func1 = [] { HashMap<Foo, Foo> hashmap1(0); };
			Assert::ExpectException<std::exception>(func1);

			// copy ctor
			std::pair<Foo, Foo> pair(foo1, foo2);
			hashmap.Insert(pair);

			HashMap<Foo, Foo> hashmap2 = hashmap;
			hashmap.Clear();
			Assert::IsTrue(hashmap.IsEmpty());
			Assert::IsFalse(hashmap2.IsEmpty());
			Assert::AreEqual(hashmap2.Size(), 1u);
			Assert::IsTrue(hashmap2.ContainsKey(pair.first));

			// asign operator
			hashmap = hashmap2;
			Assert::IsFalse(hashmap.IsEmpty());
			Assert::AreEqual(hashmap.Size(), 1u);
			Assert::IsTrue(hashmap.ContainsKey(pair.first));
		}

		/************************************************************************/
		TEST_METHOD(HashMapIsEmptyAndContainsKeyTestMethod)
		{
			// uint
			std_uint32 a1 = 5;
			std_uint32 a2 = 6;
			HashMapIsEmptyAndContainsKeyMethod(a1, a2);

			// uint ptr
			std_uint32* numPtr1 = new std_uint32(5);
			std_uint32* numPtr2 = new std_uint32(9);
			HashMapIsEmptyAndContainsKeyMethod(numPtr1, numPtr2);
			delete numPtr1;
			delete numPtr2;

			// char
			std_uchar c1 = 'g';
			std_uchar c2 = 'p';
			HashMapIsEmptyAndContainsKeyMethod(c1, c2);

			// char*
			char* ch1 = "Yo";
			char* ch2 = "Hello World";
			HashMapIsEmptyAndContainsKeyMethod(ch1, ch2);

			// string
			std::string str1 = "yo yo";
			std::string str2 = "hello Paul";
			HashMapIsEmptyAndContainsKeyMethod(str1, str2);

			HashMapIsEmptyAndContainsKeyMethod(str1, a1);
			HashMapIsEmptyAndContainsKeyMethod(a2, str2);

			// foo
			int n1 = 6;
			int n2 = 9;
			int* n3 = new int(8);
			int* n4 = new int(564);

			Foo foo1(n1, n3);
			Foo foo2(n2, n4);

			HashMap<Foo, Foo> hashmap;
			Assert::IsTrue(hashmap.IsEmpty());

			std::pair<Foo, Foo> pair(foo1, foo2);
			hashmap.Insert(pair);
			Assert::IsFalse(hashmap.IsEmpty());
			Assert::IsTrue(hashmap.ContainsKey(pair.first));

			hashmap.Clear();
			Assert::IsTrue(hashmap.IsEmpty());
			Assert::IsFalse(hashmap.ContainsKey(pair.first));
		}

		/************************************************************************/
		TEST_METHOD(HashMapSizeTestMethod)
		{
			// uint
			std_uint32 a1 = 5;
			std_uint32 a2 = 6;
			HashMapSizeMethod(a1, a2);

			// uint ptr
			std_uint32* numPtr1 = new std_uint32(5);
			std_uint32* numPtr2 = new std_uint32(9);
			HashMapSizeMethod(numPtr1, numPtr2);
			delete numPtr1;
			delete numPtr2;

			// char
			std_uchar c1 = 'g';
			std_uchar c2 = 'p';
			HashMapSizeMethod(c1, c2);

			// char*
			char* ch1 = "Yo";
			char* ch2 = "Hello World";
			HashMapSizeMethod(ch1, ch2);

			// string
			std::string str1 = "yo yo";
			std::string str2 = "hello Paul";
			HashMapSizeMethod(str1, str2);

			HashMapSizeMethod(str1, a1);
			HashMapSizeMethod(a2, str2);

			// foo
			int n1 = 6;
			int n2 = 9;
			int* n3 = new int(8);
			int* n4 = new int(564);

			Foo foo1(n1, n3);
			Foo foo2(n2, n4);

			HashMap<Foo, Foo> hashmap;
			std::pair<Foo, Foo> pair(foo1, foo2);
			hashmap.Insert(pair);

			Assert::AreEqual(hashmap.Size(), 1u);
		}

		/************************************************************************/
		TEST_METHOD(HashMapFindTestMethod)
		{
			// uint
			std_uint32 a1 = 5;
			std_uint32 a2 = 6;
			HashMapFindMethod(a1, a2);

			// uint ptr
			std_uint32* numPtr1 = new std_uint32(5);
			std_uint32* numPtr2 = new std_uint32(9);
			HashMapFindMethod(numPtr1, numPtr2);
			delete numPtr1;
			delete numPtr2;

			// char
			std_uchar c1 = 'g';
			std_uchar c2 = 'p';
			HashMapFindMethod(c1, c2);

			// char*
			char* ch1 = "Yo";
			char* ch2 = "Hello World";
			HashMapFindMethod(ch1, ch2);

			// string
			std::string str1 = "yo yo";
			std::string str2 = "hello Paul";
			HashMapFindMethod(str1, str2);

			HashMapFindMethod(str1, a1);
			HashMapFindMethod(a2, str2);

			// foo
			int n1 = 6;
			int n2 = 9;
			int* n3 = new int(8);
			int* n4 = new int(564);

			Foo foo1(n1, n3);
			Foo foo2(n2, n4);

			HashMap<Foo, Foo> hashmap;
			std::pair<const Foo, Foo> pair(foo1, foo2);

			Assert::IsTrue(hashmap.Find(foo1) == hashmap.end());

			hashmap.Insert(pair);
			auto it = hashmap.Find(foo1);
			Assert::IsTrue(hashmap.Find(foo1) != hashmap.end());
			Assert::IsTrue(*it == pair);
		}

		/************************************************************************/
		TEST_METHOD(HashMapInsertAndClearTestMethod)
		{
			// uint
			std_uint32 a1 = 5;
			std_uint32 a2 = 6;
			std_uint32 a3 = 9;
			std_uint32 a4 = 85;
			HashMapInsertAndClearMethod(a1, a2, a3, a4);

			// uint ptr
			std_uint32* numPtr1 = new std_uint32(5);
			std_uint32* numPtr2 = new std_uint32(9);
			std_uint32* numPtr3 = new std_uint32(98);
			std_uint32* numPtr4 = new std_uint32(54);

			HashMapInsertAndClearMethod(numPtr1, numPtr2, numPtr3, numPtr4);
			delete numPtr1;
			delete numPtr2;
			delete numPtr3;
			delete numPtr4;

			// char
			std_uchar c1 = 'g';
			std_uchar c2 = 'p';
			std_uchar c3 = 'v';
			std_uchar c4 = 'o';
			HashMapInsertAndClearMethod(c1, c2, c3, c4);

			// char*
			char* ch1 = "Yo";
			char* ch2 = "Hello World";
			char* ch3 = "Yo yo";
			char* ch4 = "Hello Tunisia";
			HashMapInsertAndClearMethod(ch1, ch2, ch3, ch4);

			// string
			std::string str1 = "yo yo";
			std::string str2 = "hello Paul";
			std::string str3 = "yo yo yo";
			std::string str4 = "hello Tom";
			HashMapInsertAndClearMethod(str1, str2, str3, str4);

			HashMapInsertAndClearMethod(str1,str2, a1, a2);
			HashMapInsertAndClearMethod(a3, a4, str3, str4);

			// foo
			int n1 = 6;
			int n2 = 9;
			int n3 = 89;
			int n4 = 524;

			int* n5 = new int(8);
			int* n6 = new int(564);
			int* n7 = new int(9);
			int* n8 = new int(0);

			Foo foo1(n1, n5);
			Foo foo2(n2, n6);
			Foo foo3(n3, n7);
			Foo foo4(n4, n8);

			HashMap<Foo, Foo> hashmap;
			std::pair<const Foo, Foo> pair(foo1, foo3);
			auto it = hashmap.Insert(pair);
			Assert::AreEqual(hashmap.Size(), 1u);
			Assert::IsTrue(hashmap.ContainsKey(foo1));

			std::pair<const Foo, Foo> pair2(foo1, foo4);
			it = hashmap.Insert(pair2);
			Assert::AreEqual(hashmap.Size(), 1u);
			Assert::IsTrue(hashmap.ContainsKey(foo1));
			Assert::IsTrue((*it).second == foo3);
			Assert::IsFalse((*it).second == foo4);

			std::pair<const Foo, Foo> pair3(foo2, foo4);
			it = hashmap.Insert(pair3);
			Assert::AreEqual(hashmap.Size(), 2u);
			Assert::IsTrue(hashmap.ContainsKey(foo2));

			hashmap.Clear();
			Assert::IsTrue(hashmap.IsEmpty());
			Assert::IsFalse(hashmap.ContainsKey(foo1));
			Assert::IsFalse(hashmap.ContainsKey(foo2));

		}

		/************************************************************************/
		TEST_METHOD(HashMapBracketOperatorTestMethod)
		{
			// uint
			std_uint32 a1 = 5;
			std_uint32 a2 = 6;
			std_uint32 a3 = 9;
			HashMapBracketOperatorMethod(a1, a2, a3);

			// uint ptr
			std_uint32* numPtr1 = new std_uint32(5);
			std_uint32* numPtr2 = new std_uint32(9);
			std_uint32* numPtr3 = new std_uint32(98);
			HashMapBracketOperatorMethod(numPtr1, numPtr2, numPtr3);
			delete numPtr1;
			delete numPtr2;
			delete numPtr3;

			// char
			std_uchar c1 = 'g';
			std_uchar c2 = 'p';
			std_uchar c3 = 'v';
			HashMapBracketOperatorMethod(c1, c2, c3);

			// char*
			char* ch1 = "Yo";
			char* ch2 = "Hello World";
			char* ch3 = "Yo yo";
			HashMapBracketOperatorMethod(ch1, ch2, ch3);

			// string
			std::string str1 = "yo yo";
			std::string str2 = "hello Paul";
			std::string str3 = "yo yo yo";
			HashMapBracketOperatorMethod(str1, str2, str3);

			HashMapBracketOperatorMethod(str1, str2, a1);
			HashMapBracketOperatorMethod(a2, a3, str2);

			// foo
			int n1 = 6;
			int n2 = 9;
			int n3 = 89;
			int* n4 = new int(8);
			int* n5 = new int(564);
			int* n6 = new int(9);

			Foo foo1(n1, n4);
			Foo foo2(n2, n5);
			Foo foo3(n3, n6);

			const HashMap<Foo, Foo> constHashmap;
			auto func1 = [&constHashmap, foo1] { constHashmap[foo1]; };
			Assert::ExpectException<std::exception>(func1);

			HashMap<Foo, Foo> hashmap;
			std::pair<const Foo, Foo> pair(foo2, foo3);
			hashmap[foo1];

			Assert::IsFalse(hashmap.IsEmpty());
			Assert::IsTrue(hashmap.ContainsKey(foo1));

			hashmap.Insert(pair);
			const HashMap<Foo, Foo>constHashmap2 = hashmap;

			Assert::AreEqual(constHashmap2.Size(), 2u);
			Assert::IsTrue(constHashmap2[foo2] == foo3);
			Assert::IsTrue(hashmap[foo2] == foo3);
		}

		/************************************************************************/
		TEST_METHOD(HashMapRemoveTestMethod)
		{
			// uint
			std_uint32 a1 = 5;
			std_uint32 a2 = 6;
			HashMapRemoveMethod(a1, a2);

			// uint ptr
			std_uint32* numPtr1 = new std_uint32(5);
			std_uint32* numPtr2 = new std_uint32(9);
			HashMapRemoveMethod(numPtr1, numPtr2);
			delete numPtr1;
			delete numPtr2;

			// char
			std_uchar c1 = 'g';
			std_uchar c2 = 'p';
			HashMapRemoveMethod(c1, c2);

			// char*
			char* ch1 = "Yo";
			char* ch2 = "Hello World";
			HashMapRemoveMethod(ch1, ch2);

			// string
			std::string str1 = "yo yo";
			std::string str2 = "hello Paul";
			HashMapRemoveMethod(str1, str2);

			HashMapRemoveMethod(str1, a1);
			HashMapRemoveMethod(a2, str2);

			// foo
			int n1 = 6;
			int n2 = 9;
			int* n3 = new int(8);
			int* n4 = new int(564);

			Foo foo1(n1, n3);
			Foo foo2(n2, n4);

			HashMap<Foo, Foo> hashmap;
			std::pair<const Foo, Foo> pair(foo1, foo2);

			hashmap.Insert(pair);
			bool b = hashmap.Remove(foo1);
			Assert::IsTrue(b);
			Assert::IsTrue(hashmap.IsEmpty());
			Assert::IsFalse(hashmap.ContainsKey(foo1));

			b = hashmap.Remove(foo1);
			Assert::IsFalse(b);
		}

		/************************************************************************/
		TEST_METHOD(HashMapBeginAndEndTestMethod)
		{
			// uint
			std_uint32 a1 = 5;
			std_uint32 a2 = 6;
			HashMapBeginAndEndMethod(a1, a2);

			// uint ptr
			std_uint32* numPtr1 = new std_uint32(5);
			std_uint32* numPtr2 = new std_uint32(9);
			HashMapBeginAndEndMethod(numPtr1, numPtr2);
			delete numPtr1;
			delete numPtr2;

			// char
			std_uchar c1 = 'g';
			std_uchar c2 = 'p';
			HashMapBeginAndEndMethod(c1, c2);

			// char*
			char* ch1 = "Yo";
			char* ch2 = "Hello World";
			HashMapBeginAndEndMethod(ch1, ch2);

			// string
			std::string str1 = "yo yo";
			std::string str2 = "hello Paul";
			HashMapBeginAndEndMethod(str1, str2);

			HashMapBeginAndEndMethod(str1, a1);
			HashMapBeginAndEndMethod(a2, str2);

			// foo
			int n1 = 6;
			int n2 = 9;
			int* n3 = new int(8);
			int* n4 = new int(564);

			Foo foo1(n1, n3);
			Foo foo2(n2, n4);

			HashMap<Foo, Foo> hashmap;
			Assert::IsTrue(hashmap.begin() == hashmap.end());

			std::pair<const Foo, Foo> pair(foo1, foo2);
			auto it = hashmap.Insert(pair);

			Assert::IsTrue(hashmap.begin() == it);
		}


#pragma endregion

#pragma endregion

#pragma region Iterator

		/************************************************************************/
		TEST_METHOD(IteratorConstructorsAndAssignmentTest)
		{
			HashMap<int, int> hashmap;
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

		/************************************************************************/
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
			HashMap<int, int>::Iterator it2 = hashmap.Insert(std::pair<int, int>(3, 68));
			HashMap<int, int>::Iterator it3 = hashmap.Insert(std::pair<int, int>(4, 9));
			Assert::IsTrue(it1 != it2);
			Assert::IsTrue(it3 != it2);

			HashMap<int, int>::Iterator it4 = ++it2;
			Assert::IsFalse(++it1 == it2);
			Assert::IsTrue(it2 == it3);
			Assert::IsTrue(it2 == it4);
			Assert::IsTrue(++it4 == hashmap.end());
		}

		/************************************************************************/
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
			HashMap<int, int>::Iterator it2 = hashmap.Insert(std::pair<int, int>(3, 68));
			HashMap<int, int>::Iterator it3 = hashmap.Insert(std::pair<int, int>(4, 9));
			Assert::IsTrue(it1 != it2);
			Assert::IsTrue(it3 != it2);

			HashMap<int, int>::Iterator it4 = it2++;
			Assert::IsFalse(it1++ == it2);
			Assert::IsFalse(it1++ == it3);
			Assert::IsTrue(it2 == it3);
			Assert::IsFalse(it2 == it4);
			Assert::IsTrue(it4++ != hashmap.end());
		}

		/************************************************************************/
		TEST_METHOD(IteratorEqualAndNotOperatorTest)
		{
			HashMap<int, int> hashmap(10);

			HashMap<int, int>::Iterator it1 = hashmap.Insert(std::pair<int, int>(52, 98));
			HashMap<int, int>::Iterator it2 = hashmap.Insert(std::pair<int, int>(23, 68));

			Assert::IsFalse(it1 == it2);
			Assert::IsTrue(it1 != it2);
			++it1;
			Assert::IsTrue(it1 == it2);
			Assert::IsFalse(it1 != it2);
		}

		/************************************************************************/
		TEST_METHOD(IteratorDereferenceOperatorTest)
		{
			HashMap<int, int>::Iterator it;
			auto func = [&it] { *it; };
			Assert::ExpectException<std::exception>(func);
			auto func3 = [&it] { it.operator->(); };
			Assert::ExpectException<std::exception>(func3);

			HashMap<int, int> hashmap(10);

			HashMap<int, int>::Iterator it1 = hashmap.begin();
			auto func1 = [&it1] { *it1; };
			Assert::ExpectException<std::exception>(func1);
			auto func4 = [&it1] { it1.operator->(); };
			Assert::ExpectException<std::exception>(func4);

			std::pair<const int, int> pair(52, 98);
			HashMap<int, int>::Iterator it2 = hashmap.Insert(pair);
			Assert::AreEqual(1U, hashmap.Size());
			Assert::IsTrue(pair == *it2);
			Assert::IsTrue(pair == *(it2.operator->()));

			auto it3 = hashmap.end();
			auto func2 = [&it3] { *it3; };
			Assert::ExpectException<std::exception>(func2);
			auto func5 = [&it3] { it3.operator->(); };
			Assert::ExpectException<std::exception>(func5);
		}


#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState HashMapTest::sStartMemState;

}