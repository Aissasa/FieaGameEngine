#include "pch.h"
#include "CppUnitTest.h"
#include "SList.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTest)
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

#pragma region SList
		TEST_METHOD(SListConstructorTest)
		{
			// primitive type test
			SList<int> intList;
			Assert::IsTrue(intList.IsEmpty());
			auto func1 = [&intList] { intList.Front(); };
			Assert::ExpectException<std::exception>(func1);
			auto func2 = [&intList] { intList.Back(); };
			Assert::ExpectException<std::exception>(func2);

			// pointer type test
			SList<int*> intPtrList;
			Assert::IsTrue(intPtrList.IsEmpty());
			auto func3 = [&intPtrList] { intPtrList.Front(); };
			Assert::ExpectException<std::exception>(func3);
			auto func4 = [&intPtrList] { intPtrList.Back(); };
			Assert::ExpectException<std::exception>(func4);

			// class type test
			SList<Foo> fooList;
			Assert::IsTrue(fooList.IsEmpty());
			auto func5 = [&fooList] { fooList.Front(); };
			Assert::ExpectException<std::exception>(func5);
			auto func6 = [&fooList] { fooList.Back(); };
			Assert::ExpectException<std::exception>(func6);

		}

		TEST_METHOD(SListCopyConstructorTest)
		{
			// primitive type test
			int* number1 = new int(5);
			int* number2 = new int(10);

			SList<int> intList;
			intList.PushFront(*number1);
			intList.PushFront(*number2);

			SList<int> copyIntList = intList;
			Assert::AreEqual(intList.Size(), copyIntList.Size());
			Assert::AreEqual(intList.Front(), copyIntList.Front());
			Assert::AreEqual(intList.Back(), copyIntList.Back());

			// pointer type test
			SList<int*> intPtrList;
			intPtrList.PushFront(number1);
			intPtrList.PushFront(number2);

			SList<int*> copyIntPtrList = intPtrList;
			Assert::AreEqual(intPtrList.Size(), copyIntPtrList.Size());
			Assert::AreEqual(intPtrList.Front(), copyIntPtrList.Front());
			Assert::AreEqual(intPtrList.Back(), copyIntPtrList.Back());

			// class type test
			int number3 = 10;
			int number4 = 19;
			Foo foo1(number3, number1);
			Foo foo2(number4, number2);

			SList<Foo> fooList;
			fooList.PushFront(foo1);
			fooList.PushFront(foo2);

			SList<Foo> copyFooList = fooList;
			Assert::AreEqual(fooList.Size(), copyFooList.Size());
			Assert::IsTrue(fooList.Front() == copyFooList.Front());
			Assert::IsTrue(fooList.Back() == copyFooList.Back());

			//number1 and number2 are deleted within the foo destructor, so no need to add delete 
		}

		TEST_METHOD(SListAssignmentOperatorTest)
		{
			// primitive type test
			int* number0 = new int(5);
			int* number1 = new int(10);

			SList<int> intList;
			intList.PushFront(*number0);
			intList.PushFront(*number1);

			SList<int> newIntList;
			newIntList = intList;
			Assert::AreEqual(intList.Size(), newIntList.Size());
			Assert::AreEqual(intList.Front(), newIntList.Front());
			Assert::AreEqual(intList.Back(), newIntList.Back());

			// pointer type test
			SList<int*> intPtrList;
			intPtrList.PushFront(number0);
			intPtrList.PushFront(number1);

			SList<int*> newIntPtrList;
			newIntPtrList = intPtrList;
			Assert::AreEqual(intPtrList.Size(), newIntPtrList.Size());
			Assert::AreEqual(intPtrList.Front(), newIntPtrList.Front());
			Assert::AreEqual(intPtrList.Back(), newIntPtrList.Back());

			// class type test
			int number4 = 10;
			int number5 = 19;
			Foo foo1(number4, number0);
			Foo foo2(number5, number1);

			SList<Foo> fooList;
			fooList.PushFront(foo1);
			fooList.PushFront(foo2);

			SList<Foo> newFooList;
			newFooList = fooList;
			Assert::AreEqual(fooList.Size(), newFooList.Size());
			Assert::IsTrue(fooList.Front() == newFooList.Front());
			Assert::IsTrue(fooList.Back() == newFooList.Back());

		}

		TEST_METHOD(SListPushFrontTest)
		{
			int* number1 = new int(3);
			int* number2 = new int(7);

			// primitive type test
			SList<int> intList;
			intList.PushFront(*number1);
			Assert::IsFalse(intList.IsEmpty());
			Assert::AreEqual(intList.Size(), 1U);
			Assert::AreEqual(intList.Back(), intList.Front());

			intList.PushFront(*number2);
			Assert::AreEqual(intList.Size(), 2U);
			Assert::AreNotEqual(intList.Back(), intList.Front());
			Assert::AreEqual(*number2, intList.Front());
			Assert::AreEqual(*number1, intList.Back());

			// pointer type test
			SList<int*> intPtrList;
			intPtrList.PushFront(number1);
			Assert::IsFalse(intPtrList.IsEmpty());
			Assert::AreEqual(intPtrList.Size(), 1U);
			Assert::AreEqual(intPtrList.Back(), intPtrList.Front());

			intPtrList.PushFront(number2);
			Assert::AreEqual(intPtrList.Size(), 2U);
			Assert::AreNotEqual(intPtrList.Back(), intPtrList.Front());
			Assert::AreEqual(number2, intPtrList.Front());
			Assert::AreEqual(number1, intPtrList.Back());

			// class type test
			int number3 = 10;
			int number4 = 19;
			Foo foo1(number3, number1);
			Foo foo2(number4, number2);

			SList<Foo> fooList;
			fooList.PushFront(foo1);
			Assert::IsFalse(fooList.IsEmpty());
			Assert::AreEqual(fooList.Size(), 1U);
			Assert::IsTrue(fooList.Back() == fooList.Front());

			fooList.PushFront(foo2);
			Assert::AreEqual(fooList.Size(), 2U);
			Assert::IsFalse(fooList.Back() == fooList.Front());
			Assert::IsTrue(foo2 == fooList.Front());
			Assert::IsTrue(foo1 == fooList.Back());
		}

		TEST_METHOD(SListPopFrontTest)
		{
			int* number1 = new int(3);
			int* number2 = new int(7);

			// primitive type test
			SList<int> intList;
			intList.PopFront();
			intList.PushFront(*number1);
			intList.PushFront(*number2);
			intList.PopFront();

			Assert::AreEqual(intList.Size(), 1U);
			Assert::AreEqual(intList.Back(), intList.Front());
			Assert::AreEqual(*number1, intList.Front());

			intList.PopFront();
			Assert::IsTrue(intList.IsEmpty());
			auto func1 = [&intList] { intList.Front(); };
			Assert::ExpectException<std::exception>(func1);
			auto func2 = [&intList] { intList.Back(); };
			Assert::ExpectException<std::exception>(func2);

			// pointer type test
			SList<int*> intPtrList;
			intPtrList.PopFront();
			intPtrList.PushFront(number1);
			intPtrList.PushFront(number2);
			intPtrList.PopFront();

			Assert::AreEqual(intPtrList.Size(), 1U);
			Assert::AreEqual(intPtrList.Back(), intPtrList.Front());
			Assert::AreEqual(number1, intPtrList.Front());

			intPtrList.PopFront();
			Assert::IsTrue(intPtrList.IsEmpty());
			auto func3 = [&intPtrList] { intPtrList.Front(); };
			Assert::ExpectException<std::exception>(func3);
			auto func4 = [&intPtrList] { intPtrList.Back(); };
			Assert::ExpectException<std::exception>(func4);

			// class type test
			int number3 = 10;
			int number4 = 19;
			Foo foo1(number3, number1);
			Foo foo2(number4, number2);

			SList<Foo> fooList;
			fooList.PopFront();
			fooList.PushFront(foo1);
			fooList.PushFront(foo2);
			fooList.PopFront();

			Assert::AreEqual(fooList.Size(), 1U);
			Assert::IsTrue(fooList.Back() == fooList.Front());
			Assert::IsTrue(foo1 == fooList.Front());

			fooList.PopFront();
			Assert::IsTrue(fooList.IsEmpty());
			auto func5 = [&fooList] { fooList.Front(); };
			Assert::ExpectException<std::exception>(func5);
			auto func6 = [&fooList] { fooList.Back(); };
			Assert::ExpectException<std::exception>(func6);

		}

		TEST_METHOD(SListPushBackTest)
		{
			int* number1 = new int(3);
			int* number2 = new int(7);

			// primitive type test
			SList<int> intList;
			intList.PushBack(*number1);
			Assert::IsFalse(intList.IsEmpty());
			Assert::AreEqual(intList.Size(), 1U);
			Assert::AreEqual(intList.Back(), intList.Front());

			intList.PushBack(*number2);
			Assert::AreEqual(intList.Size(), 2U);
			Assert::AreNotEqual(intList.Back(), intList.Front());
			Assert::AreEqual(*number1, intList.Front());
			Assert::AreEqual(*number2, intList.Back());

			// pointer type test
			SList<int*> intPtrList;
			intPtrList.PushBack(number1);
			Assert::IsFalse(intPtrList.IsEmpty());
			Assert::AreEqual(intPtrList.Size(), 1U);
			Assert::AreEqual(intPtrList.Back(), intPtrList.Front());

			intPtrList.PushBack(number2);
			Assert::AreEqual(intPtrList.Size(), 2U);
			Assert::AreNotEqual(intPtrList.Back(), intPtrList.Front());
			Assert::AreEqual(number1, intPtrList.Front());
			Assert::AreEqual(number2, intPtrList.Back());

			// class type test
			int number3 = 10;
			int number4 = 19;
			Foo foo1(number3, number1);
			Foo foo2(number4, number2);

			SList<Foo> fooList;
			fooList.PushBack(foo1);
			Assert::IsFalse(fooList.IsEmpty());
			Assert::AreEqual(fooList.Size(), 1U);
			Assert::IsTrue(fooList.Back() == fooList.Front());

			fooList.PushBack(foo2);
			Assert::AreEqual(fooList.Size(), 2U);
			Assert::IsFalse(fooList.Back() == fooList.Front());
			Assert::IsTrue(foo1 == fooList.Front());
			Assert::IsTrue(foo2 == fooList.Back());
		}

		TEST_METHOD(SListIsEmptyTest)
		{
			// primitive type test
			int* number1 = new int(5);

			SList<int> intList;
			Assert::IsTrue(intList.IsEmpty());
			intList.PushFront(*number1);
			Assert::IsFalse(intList.IsEmpty());
			intList.PopFront();
			Assert::IsTrue(intList.IsEmpty());

			// pointer type test
			SList<int*> intPtrList;
			Assert::IsTrue(intPtrList.IsEmpty());
			intPtrList.PushFront(number1);
			Assert::IsFalse(intPtrList.IsEmpty());
			intPtrList.PopFront();
			Assert::IsTrue(intPtrList.IsEmpty());

			// class type test
			int number2 = 10;
			Foo foo(number2, number1);

			SList<Foo> fooList;
			Assert::IsTrue(fooList.IsEmpty());
			fooList.PushFront(foo);
			Assert::IsFalse(fooList.IsEmpty());
			fooList.PopFront();
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(SListFrontTest)
		{
			// primitive type test
			int* number1 = new int(5);
			int* number2 = new int(7);

			SList<int> intList;
			auto func1 = [&intList] { intList.Front(); };
			Assert::ExpectException<std::exception>(func1);

			intList.PushFront(*number1);
			Assert::AreEqual(intList.Front(), *number1);
			intList.PushFront(*number2);
			Assert::AreEqual(intList.Front(), *number2);


			// pointer type test
			SList<int*> intPtrList;
			auto func2 = [&intPtrList] { intPtrList.Front(); };
			Assert::ExpectException<std::exception>(func2);

			intPtrList.PushFront(number1);
			Assert::AreEqual(intPtrList.Front(), number1);
			intPtrList.PushFront(number2);
			Assert::AreEqual(intPtrList.Front(), number2);

			// class type test
			int number3 = 10;
			int number4 = 1;
			Foo foo1(number3, number1);
			Foo foo2(number4, number2);

			SList<Foo> fooList;
			auto func3 = [&fooList] { fooList.Front(); };
			Assert::ExpectException<std::exception>(func3);

			fooList.PushFront(foo1);
			Assert::IsTrue(fooList.Front() == foo1);
			fooList.PushFront(foo2);
			Assert::IsTrue(fooList.Front() == foo2);
		}

		TEST_METHOD(SListBackTest)
		{
			// primitive type test
			int* number1 = new int(5);
			int* number2 = new int(7);

			SList<int> intList;
			auto func1 = [&intList] { intList.Back(); };
			Assert::ExpectException<std::exception>(func1);

			intList.PushBack(*number1);
			Assert::AreEqual(intList.Back(), *number1);
			intList.PushBack(*number2);
			Assert::AreEqual(intList.Back(), *number2);


			// pointer type test
			SList<int*> intPtrList;
			auto func2 = [&intPtrList] { intPtrList.Back(); };
			Assert::ExpectException<std::exception>(func2);

			intPtrList.PushBack(number1);
			Assert::AreEqual(intPtrList.Back(), number1);
			intPtrList.PushBack(number2);
			Assert::AreEqual(intPtrList.Back(), number2);

			// class type test
			int number3 = 10;
			int number4 = 1;
			Foo foo1(number3, number1);
			Foo foo2(number4, number2);

			SList<Foo> fooList;
			auto func3 = [&fooList] { fooList.Back(); };
			Assert::ExpectException<std::exception>(func3);

			fooList.PushBack(foo1);
			Assert::IsTrue(fooList.Back() == foo1);
			fooList.PushBack(foo2);
			Assert::IsTrue(fooList.Back() == foo2);
		}

		TEST_METHOD(SListSizeTest)
		{
			// primitive type test
			int* number1 = new int(5);
			int* number2 = new int(0);

			SList<int> intList;
			Assert::AreEqual(intList.Size(), 0U);
			intList.PushFront(*number1);
			Assert::AreEqual(intList.Size(), 1U);
			intList.PushFront(*number2);
			Assert::AreEqual(intList.Size(), 2U);
			intList.PopFront();
			Assert::AreEqual(intList.Size(), 1U);

			// pointer type test
			SList<int*> intPtrList;
			Assert::AreEqual(intPtrList.Size(), 0U);
			intPtrList.PushFront(number1);
			Assert::AreEqual(intPtrList.Size(), 1U);
			intPtrList.PushFront(number2);
			Assert::AreEqual(intPtrList.Size(), 2U);
			intPtrList.PopFront();
			Assert::AreEqual(intPtrList.Size(), 1U);

			// class type test
			int number3 = 10;
			int number4 = 9;
			Foo foo1(number3, number1);
			Foo foo2(number4, number2);

			SList<Foo> fooList;
			Assert::AreEqual(fooList.Size(), 0U);
			fooList.PushFront(foo1);
			Assert::AreEqual(fooList.Size(), 1U);
			fooList.PushFront(foo2);
			Assert::AreEqual(fooList.Size(), 2U);
			fooList.PopFront();
			Assert::AreEqual(fooList.Size(), 1U);
		}

		TEST_METHOD(SListClearTest)
		{
			// primitive type test
			int* number1 = new int(5);
			int* number2 = new int(0);

			SList<int> intList;
			intList.PushFront(*number1);
			intList.PushFront(*number2);
			intList.Clear();
			Assert::IsTrue(intList.IsEmpty());

			// pointer type test
			SList<int*> intPtrList;
			intPtrList.PushFront(number1);
			intPtrList.PushFront(number2);
			intPtrList.Clear();
			Assert::IsTrue(intPtrList.IsEmpty());

			// class type test
			int number3 = 10;
			int number4 = 9;
			Foo foo1(number3, number1);
			Foo foo2(number4, number2);

			SList<Foo> fooList;
			fooList.PushFront(foo1);
			fooList.PushFront(foo2);
			fooList.Clear();
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(SListBeginTest)
		{
			// primitive type test
			int number1 = 3;
			int number2 = 2;

			SList<int> intList;
			SList<int>::Iterator it1 = intList.PushBack(number1);
			Assert::IsTrue(it1 == intList.begin());
			it1 = intList.PushBack(number2);
			Assert::IsTrue(it1 != intList.begin());

			// pointer type test
			SList<int*> intPtrList;
			SList<int*>::Iterator it2 = intPtrList.PushBack(&number1);
			Assert::IsTrue(it2 == intPtrList.begin());
			it2 = intPtrList.PushBack(&number2);
			Assert::IsTrue(it2 != intPtrList.begin());

			// class type test
			int* number3 = new int(10);
			int* number4 = new int(6);
			Foo foo1(number1, number3);
			Foo foo2(number2, number4);

			SList<Foo> fooList;
			SList<Foo>::Iterator it3 = fooList.PushBack(foo1);
			Assert::IsTrue(it3 == fooList.begin());
			it3 = fooList.PushBack(foo2);
			Assert::IsTrue(it3 != fooList.begin());
		}

		TEST_METHOD(SListEndTest)
		{
			// primitive type test
			int number1 = 3;

			SList<int> intList;
			Assert::IsTrue(intList.begin() == intList.end());

			SList<int>::Iterator it1 = intList.PushBack(number1);
			Assert::IsTrue(it1 != intList.end());
			Assert::IsTrue(++it1 == intList.end());

			// pointer type test
			SList<int*> intPtrList;
			Assert::IsTrue(intPtrList.begin() == intPtrList.end());

			SList<int*>::Iterator it2 = intPtrList.PushBack(&number1);
			Assert::IsTrue(it2 != intPtrList.end());
			Assert::IsTrue(++it2 == intPtrList.end());

			// class type test
			int* number2 = new int(10);
			Foo foo1(number1, number2);

			SList<Foo> fooList;
			Assert::IsTrue(fooList.begin() == fooList.end());

			SList<Foo>::Iterator it3 = fooList.PushBack(foo1);
			Assert::IsTrue(it3 != fooList.end());
			Assert::IsTrue(++it3 == fooList.end());
		}

		TEST_METHOD(SListFindTest)
		{
			// primitive type test
			int number1 = 3;
			int number2 = 2;
			int number0 = 9;

			SList<int> intList;
			SList<int>::Iterator it0 = intList.Find(number0);
			Assert::IsTrue(it0 == intList.end());

			intList.PushBack(number1);
			intList.PushBack(number2);
			it0 = intList.Find(number0);
			Assert::IsTrue(it0 == intList.end());

			it0 = intList.Find(number2);
			Assert::IsTrue(*it0 == number2);

			// pointer type test
			SList<int*> intPtrList;
			SList<int*>::Iterator it1 = intPtrList.Find(&number0);
			Assert::IsTrue(it1 == intPtrList.end());

			intPtrList.PushBack(&number1);
			intPtrList.PushBack(&number2);
			it1 = intPtrList.Find(&number0);
			Assert::IsTrue(it1 == intPtrList.end());

			it1 = intPtrList.Find(&number2);
			Assert::IsTrue(*it1 == &number2);

			// class type test
			int* number3 = new int(10);
			int* number4 = new int(6);
			int* number5 = new int(98);

			Foo foo1(number1, number3);
			Foo foo2(number2, number4);
			Foo foo0(number0, number5);

			SList<Foo> fooList;
			SList<Foo>::Iterator it2 = fooList.Find(foo0);
			Assert::IsTrue(it2 == fooList.end());

			fooList.PushBack(foo1);
			fooList.PushBack(foo2);
			it2 = fooList.Find(foo0);
			Assert::IsTrue(it2 == fooList.end());

			it2 = fooList.Find(foo2);
			Assert::IsTrue(*it2 == foo2);
		}

		TEST_METHOD(SListInsertAfterTest)
		{
			// primitive type test
			int number1 = 3;
			int number2 = 2;
			int number0 = 9;

			SList<int> intList;
			SList<int>::Iterator it0 = intList.PushBack(number1);
			intList.PushBack(number2);
			SList<int>::Iterator it1 = intList.InsertAfter(number0, it0);
			Assert::AreEqual(3U, intList.Size());
			Assert::AreEqual(*it1, number0);
			Assert::IsTrue(++it0 == it1);

			it1 = intList.InsertAfter(number0, intList.end());
			Assert::AreEqual(*it1, intList.Back());

			// pointer type test
			SList<int*> intPtrList;
			SList<int*>::Iterator it2 = intPtrList.PushBack(&number1);
			intPtrList.PushBack(&number2);
			SList<int*>::Iterator it3 = intPtrList.InsertAfter(&number0, it2);
			Assert::AreEqual(3U, intPtrList.Size());
			Assert::AreEqual(*it3, &number0);
			Assert::IsTrue(++it2 == it3);

			it3 = intPtrList.InsertAfter(&number0, intPtrList.end());
			Assert::AreEqual(*it3, intPtrList.Back());

			// class type test
			int* number3 = new int(10);
			int* number4 = new int(6);
			int* number5 = new int(98);

			Foo foo1(number1, number3);
			Foo foo2(number2, number4);
			Foo foo0(number0, number5);

			SList<Foo> fooList;
			SList<Foo>::Iterator it4 = fooList.PushBack(foo1);
			fooList.PushBack(foo2);
			SList<Foo>::Iterator it5 = fooList.InsertAfter(foo0, it4);
			Assert::AreEqual(3U, fooList.Size());
			Assert::IsTrue(*it5 == foo0);
			Assert::IsTrue(++it4 == it5);

			it5 = fooList.InsertAfter(foo0, fooList.end());
			Assert::IsTrue(*it5 == fooList.Back());
		}

		TEST_METHOD(SListRemoveTest)
		{
			// primitive type test
			int number0 = 9;
			int number1 = 3;
			int number2 = 2;
			bool theBool;

			SList<int> intList;
			theBool = intList.Remove(number1);
			Assert::IsFalse(theBool);

			intList.PushBack(number0);
			intList.PushBack(number1);
			intList.PushBack(number2);
			theBool = intList.Remove(number1);
			Assert::IsTrue(theBool);
			Assert::AreEqual(intList.Size(), 2U);

			theBool = intList.Remove(number1);
			Assert::IsFalse(theBool);
			Assert::AreEqual(intList.Size(), 2U);

			// pointer type test
			SList<int*> intPtrList;
			Assert::IsFalse(intPtrList.Remove(&number1));

			intPtrList.PushBack(&number0);
			intPtrList.PushBack(&number1);
			intPtrList.PushBack(&number2);
			theBool = intPtrList.Remove(&number1);
			Assert::IsTrue(theBool);
			Assert::AreEqual(intPtrList.Size(), 2U);

			theBool = intPtrList.Remove(&number1);
			Assert::IsFalse(theBool);
			Assert::AreEqual(intPtrList.Size(), 2U);

			// class type test
			int* number3 = new int(10);
			int* number4 = new int(6);
			int* number5 = new int(98);

			Foo foo1(number1, number3);
			Foo foo2(number2, number4);
			Foo foo0(number0, number5);

			SList<Foo> fooList;
			Assert::IsFalse(fooList.Remove(foo1));

			fooList.PushBack(foo0);
			fooList.PushBack(foo1);
			fooList.PushBack(foo2);
			theBool = fooList.Remove(foo1);
			Assert::IsTrue(theBool);
			Assert::AreEqual(fooList.Size(), 2U);

			theBool = fooList.Remove(foo1);
			Assert::IsFalse(theBool);
			Assert::AreEqual(fooList.Size(), 2U);

		}

#pragma endregion

#pragma region Iterator
		TEST_METHOD(IteratorConstructorsAndAssignmentTest)
		{
			SList<int> list;
			SList<int>::Iterator it1 = list.PushBack(0);
			SList<int>::Iterator it2(it1);
			Assert::IsTrue(it1 == it2); // copy ctor

			SList<int>::Iterator it3;
			SList<int>::Iterator it4;
			Assert::IsTrue(it3 == it4); // ctor

			it3 = it1;
			it4 = it2;
			Assert::IsTrue(it1 == it4 && it2 == it3); // assignment operator
		}

		TEST_METHOD(IteratorPrefixIncrementationOperatorTest)
		{
			SList<int>::Iterator it;
			auto func = [&it] { ++it; };
			Assert::ExpectException<std::exception>(func);

			SList<int> list;
			SList<int>::Iterator it0 = list.begin();
			auto func0 = [&it0] { ++it0; };
			Assert::ExpectException<std::exception>(func0);


			SList<int>::Iterator it1 = list.PushBack(5);
			SList<int>::Iterator it2 = list.PushBack(10);
			Assert::IsTrue(it1 != it2);

			SList<int>::Iterator it3 = ++it1;
			Assert::IsTrue(it1 == it2);
			Assert::IsTrue(it1 == it3);
		}

		TEST_METHOD(IteratorPostfixIncrementationOperatorTest)
		{
			SList<int>::Iterator it;
			auto func = [&it] { it++; };
			Assert::ExpectException<std::exception>(func);

			SList<int> list;
			SList<int>::Iterator it0 = list.begin();
			auto func0 = [&it0] { it0++; };
			Assert::ExpectException<std::exception>(func0);


			SList<int>::Iterator it1 = list.PushBack(5);
			SList<int>::Iterator it2 = list.PushBack(10);

			SList<int>::Iterator it3 = it1++;
			Assert::IsTrue(it1 == it2);
			Assert::IsTrue(it1 != it3);
		}

		TEST_METHOD(IteratorEqualAndNotOperatorTest)
		{
			SList<int> list;

			SList<int>::Iterator it1 = list.PushBack(0);
			SList<int>::Iterator it2 = list.PushBack(6);

			Assert::IsFalse(it1 == it2);
			Assert::IsTrue(it1 != it2);
			++it1;
			Assert::IsTrue(it1 == it2);
			Assert::IsFalse(it1 != it2);
		}

		TEST_METHOD(IteratorDereferenceOperatorTest)
		{
			SList<int>::Iterator it;
			auto func = [&it] { *it; };
			Assert::ExpectException<std::exception>(func);

			SList<int> list;

			SList<int>::Iterator it1 = list.begin();
			auto func1 = [&it1] { *it1; };
			Assert::ExpectException<std::exception>(func1);

			int number = 5;
			SList<int>::Iterator it2 = list.PushBack(number);
			Assert::AreEqual(number, *it2);
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListTest::sStartMemState;

}