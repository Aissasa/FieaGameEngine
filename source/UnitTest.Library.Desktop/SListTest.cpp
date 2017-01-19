#include "pch.h"
#include "CppUnitTest.h"
#include "SList.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngineLibrary;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTest)
	{
	public:

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

		TEST_METHOD(ConstructorTest)
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

		TEST_METHOD(CopyConstructorTest)
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

		TEST_METHOD(AssignmentOperatorTest)
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

		TEST_METHOD(PushFrontTest)
		{
			int* number1 = new int(3);
			int* number2 = new int(7);

			// primitive type test
			SList<int> intList;
			intList.PushFront(*number1);
			Assert::IsFalse(intList.IsEmpty());
			Assert::AreEqual(intList.Size(), (uint32_t)1);
			Assert::AreEqual(intList.Back(), intList.Front());

			intList.PushFront(*number2);
			Assert::AreEqual(intList.Size(), (uint32_t)2);
			Assert::AreNotEqual(intList.Back(), intList.Front());
			Assert::AreEqual(*number2, intList.Front());
			Assert::AreEqual(*number1, intList.Back());

			// pointer type test
			SList<int*> intPtrList;
			intPtrList.PushFront(number1);
			Assert::IsFalse(intPtrList.IsEmpty());
			Assert::AreEqual(intPtrList.Size(), (uint32_t)1);
			Assert::AreEqual(intPtrList.Back(), intPtrList.Front());

			intPtrList.PushFront(number2);
			Assert::AreEqual(intPtrList.Size(), (uint32_t)2);
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
			Assert::AreEqual(fooList.Size(), (uint32_t)1);
			Assert::IsTrue(fooList.Back() == fooList.Front());

			fooList.PushFront(foo2);
			Assert::AreEqual(fooList.Size(), (uint32_t)2);
			Assert::IsFalse(fooList.Back() == fooList.Front());
			Assert::IsTrue(foo2 == fooList.Front());
			Assert::IsTrue(foo1 == fooList.Back());
		}

		TEST_METHOD(PopFrontTest)
		{
			int* number1 = new int(3);
			int* number2 = new int(7);

			// primitive type test
			SList<int> intList;
			intList.PopFront();
			intList.PushFront(*number1);
			intList.PushFront(*number2);
			intList.PopFront();

			Assert::AreEqual(intList.Size(), (uint32_t)1);
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

			Assert::AreEqual(intPtrList.Size(), (uint32_t)1);
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

			Assert::AreEqual(fooList.Size(), (uint32_t)1);
			Assert::IsTrue(fooList.Back() == fooList.Front());
			Assert::IsTrue(foo1 == fooList.Front());

			fooList.PopFront();
			Assert::IsTrue(fooList.IsEmpty());
			auto func5 = [&fooList] { fooList.Front(); };
			Assert::ExpectException<std::exception>(func5);
			auto func6 = [&fooList] { fooList.Back(); };
			Assert::ExpectException<std::exception>(func6);

		}

		TEST_METHOD(PushBackTest)
		{
			int* number1 = new int(3);
			int* number2 = new int(7);

			// primitive type test
			SList<int> intList;
			intList.PushBack(*number1);
			Assert::IsFalse(intList.IsEmpty());
			Assert::AreEqual(intList.Size(), (uint32_t)1);
			Assert::AreEqual(intList.Back(), intList.Front());

			intList.PushBack(*number2);
			Assert::AreEqual(intList.Size(), (uint32_t)2);
			Assert::AreNotEqual(intList.Back(), intList.Front());
			Assert::AreEqual(*number1, intList.Front());
			Assert::AreEqual(*number2, intList.Back());

			// pointer type test
			SList<int*> intPtrList;
			intPtrList.PushBack(number1);
			Assert::IsFalse(intPtrList.IsEmpty());
			Assert::AreEqual(intPtrList.Size(), (uint32_t)1);
			Assert::AreEqual(intPtrList.Back(), intPtrList.Front());

			intPtrList.PushBack(number2);
			Assert::AreEqual(intPtrList.Size(), (uint32_t)2);
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
			Assert::AreEqual(fooList.Size(), (uint32_t)1);
			Assert::IsTrue(fooList.Back() == fooList.Front());

			fooList.PushBack(foo2);
			Assert::AreEqual(fooList.Size(), (uint32_t)2);
			Assert::IsFalse(fooList.Back() == fooList.Front());
			Assert::IsTrue(foo1 == fooList.Front());
			Assert::IsTrue(foo2 == fooList.Back());
		}

		TEST_METHOD(IsEmptyTest)
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

		TEST_METHOD(FrontTest)
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

		TEST_METHOD(BackTest)
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

		TEST_METHOD(SizeTest)
		{
			// primitive type test
			int* number1 = new int(5);
			int* number2 = new int(0);

			SList<int> intList;
			Assert::AreEqual(intList.Size(), (uint32_t)0);
			intList.PushFront(*number1);
			Assert::AreEqual(intList.Size(), (uint32_t)1);
			intList.PushFront(*number2);
			Assert::AreEqual(intList.Size(), (uint32_t)2);
			intList.PopFront();
			Assert::AreEqual(intList.Size(), (uint32_t)1);

			// pointer type test
			SList<int*> intPtrList;
			Assert::AreEqual(intPtrList.Size(), (uint32_t)0);
			intPtrList.PushFront(number1);
			Assert::AreEqual(intPtrList.Size(), (uint32_t)1);
			intPtrList.PushFront(number2);
			Assert::AreEqual(intPtrList.Size(), (uint32_t)2);
			intPtrList.PopFront();
			Assert::AreEqual(intPtrList.Size(), (uint32_t)1);

			// class type test
			int number3 = 10;
			int number4 = 9;
			Foo foo1(number3, number1);
			Foo foo2(number4, number2);

			SList<Foo> fooList;
			Assert::AreEqual(fooList.Size(), (uint32_t)0);
			fooList.PushFront(foo1);
			Assert::AreEqual(fooList.Size(), (uint32_t)1);
			fooList.PushFront(foo2);
			Assert::AreEqual(fooList.Size(), (uint32_t)2);
			fooList.PopFront();
			Assert::AreEqual(fooList.Size(), (uint32_t)1);
		}

		TEST_METHOD(ClearTest)
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


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListTest::sStartMemState;

}