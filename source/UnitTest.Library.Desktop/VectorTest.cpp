#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Vector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTest)
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

#pragma region Iterator
		TEST_METHOD(IteratorConstructorsAndAssignmentTest)
		{
			Vector<int> vect;
			Vector<int>::Iterator it1 = vect.PushBack(0);
			Vector<int>::Iterator it2(it1);
			Assert::IsTrue(it1 == it2); // copy ctor

			Vector<int>::Iterator it3;
			Vector<int>::Iterator it4;
			Assert::IsTrue(it3 == it4); // ctor

			it3 = it1;
			it4 = it2;
			Assert::IsTrue(it1 == it4 && it2 == it3); // assignment operator
		}

		TEST_METHOD(IteratorPrefixIncrementationOperatorTest)
		{
			Vector<int>::Iterator it;
			auto func = [&it] { ++it; };
			Assert::ExpectException<std::exception>(func);

			Vector<int> vect;
			Vector<int>::Iterator it0 = vect.begin();
			auto func0 = [&it0] { ++it0; };
			Assert::ExpectException<std::exception>(func0);


			Vector<int>::Iterator it1 = vect.PushBack(5);
			Vector<int>::Iterator it2 = vect.PushBack(10);
			Assert::IsTrue(it1 != it2);

			Vector<int>::Iterator it3 = ++it1;
			Assert::IsTrue(it1 == it2);
			Assert::IsTrue(it1 == it3);
		}

		TEST_METHOD(IteratorPostfixIncrementationOperatorTest)
		{
			Vector<int>::Iterator it;
			auto func = [&it] { it++; };
			Assert::ExpectException<std::exception>(func);

			Vector<int> vect;
			Vector<int>::Iterator it0 = vect.begin();
			auto func0 = [&it0] { it0++; };
			Assert::ExpectException<std::exception>(func0);


			Vector<int>::Iterator it1 = vect.PushBack(5);
			Vector<int>::Iterator it2 = vect.PushBack(10);

			Vector<int>::Iterator it3 = it1++;
			Assert::IsTrue(it1 == it2);
			Assert::IsTrue(it1 != it3);
		}

		TEST_METHOD(IteratorPrefixDecrementationOperatorTest)
		{
			Vector<int>::Iterator it;
			auto func = [&it] { --it; };
			Assert::ExpectException<std::exception>(func);

			Vector<int> vect;
			Vector<int>::Iterator it0 = vect.begin();
			auto func0 = [&it0] { --it0; };
			Assert::ExpectException<std::exception>(func0);


			Vector<int>::Iterator it1 = vect.PushBack(5);
			Vector<int>::Iterator it2 = vect.PushBack(10);
			Assert::IsTrue(it1 != it2);

			Vector<int>::Iterator it3 = --it2;
			Assert::IsTrue(it2 == it1);
			Assert::IsTrue(it2 == it3);
		}

		TEST_METHOD(IteratorPostfixDecrementationOperatorTest)
		{
			Vector<int>::Iterator it;
			auto func = [&it] { it--; };
			Assert::ExpectException<std::exception>(func);

			Vector<int> vect;
			Vector<int>::Iterator it0 = vect.begin();
			auto func0 = [&it0] { it0--; };
			Assert::ExpectException<std::exception>(func0);


			Vector<int>::Iterator it1 = vect.PushBack(5);
			Vector<int>::Iterator it2 = vect.PushBack(10);

			Vector<int>::Iterator it3 = it2--;
			Assert::IsTrue(it2 == it1);
			Assert::IsTrue(it2 != it3);
		}


		TEST_METHOD(IteratorEqualAndNotOperatorTest)
		{
			Vector<int> vect;

			Vector<int>::Iterator it1 = vect.PushBack(0);
			Vector<int>::Iterator it2 = vect.PushBack(6);

			Assert::IsFalse(it1 == it2);
			Assert::IsTrue(it1 != it2);
			++it1;
			Assert::IsTrue(it1 == it2);
			Assert::IsFalse(it1 != it2);
		}

		TEST_METHOD(IteratorDereferenceOperatorTest)
		{
			Vector<int>::Iterator it;
			auto func = [&it] { *it; };
			Assert::ExpectException<std::exception>(func);

			Vector<int> vect;

			Vector<int>::Iterator it1 = vect.begin();
			auto func1 = [&it1] { *it1; };
			Assert::ExpectException<std::exception>(func1);

			int number = 5;
			Vector<int>::Iterator it2 = vect.PushBack(number);
			Assert::AreEqual(1U, vect.Size());
			Vector<int>::Iterator it3 = vect.end();
			auto func2 = [&it3] { *it3; };
			Assert::ExpectException<std::exception>(func2);
			Assert::AreEqual(number, *it2);
		}

#pragma endregion

#pragma region Vector

#pragma region Templates

		template<typename T>
		void EqualityAndNonEqualityTemplate(T t1, T t2)
		{
			Vector<T> vect;
			Vector<T> vect2;

			vect.PushBack(t1);
			vect.PushBack(t2);
			vect2.PushBack(t1);
			vect2.PushBack(t2);

			Assert::IsTrue(vect == vect2);
			Assert::IsFalse(vect != vect2);

			vect2.PopBack();
			Assert::IsFalse(vect == vect2);
			Assert::IsTrue(vect != vect2);

		}

#pragma endregion

		TEST_METHOD(VectorConstructorsTest)
		{
			// primitive type test
			Vector<int> intVect;
			Assert::IsTrue(intVect.IsEmpty());
			Assert::AreEqual(intVect.Capacity(), 4u);
			auto func1 = [&intVect] { intVect.Front(); };
			Assert::ExpectException<std::exception>(func1);
			auto func2 = [&intVect] { intVect.Back(); };
			Assert::ExpectException<std::exception>(func2);

			Vector<int> intVect2(2);
			Assert::IsTrue(intVect2.IsEmpty());
			Assert::AreEqual(intVect2.Capacity(), 2u);

			Vector<int> intVect3(10, true);
			Assert::AreEqual(intVect3.Size(), 10u);
			intVect3[9];

			// pointer type test
			Vector<int*> intPtrVect;
			Assert::IsTrue(intPtrVect.IsEmpty());
			Assert::AreEqual(intPtrVect.Capacity(), 4u);
			auto func3 = [&intPtrVect] { intPtrVect.Front(); };
			Assert::ExpectException<std::exception>(func3);
			auto func4 = [&intPtrVect] { intPtrVect.Back(); };
			Assert::ExpectException<std::exception>(func4);

			Vector<int*> intPtrVect2(2);
			Assert::IsTrue(intPtrVect2.IsEmpty());
			Assert::AreEqual(intPtrVect2.Capacity(), 2u);

			Vector<int*> intPtrVect3(10, true);
			Assert::AreEqual(intPtrVect3.Size(), 10u);
			intPtrVect3[9];

			// class type test
			Vector<Foo> fooVect;
			Assert::IsTrue(fooVect.IsEmpty());
			Assert::AreEqual(fooVect.Capacity(), 4u);
			auto func5 = [&fooVect] { fooVect.Front(); };
			Assert::ExpectException<std::exception>(func5);
			auto func6 = [&fooVect] { fooVect.Back(); };
			Assert::ExpectException<std::exception>(func6);

			Vector<Foo> fooVect2(2);
			Assert::IsTrue(fooVect2.IsEmpty());
			Assert::AreEqual(fooVect2.Capacity(), 2u);

			Vector<Foo> fooVect3(10, true);
			Assert::AreEqual(fooVect3.Size(), 10u);
			fooVect3[9];
		}

		TEST_METHOD(VectorCopyConstructorTest)
		{
			// primitive type test
			int* number1 = new int(5);
			int* number2 = new int(10);

			Vector<int> intVect;
			intVect.PushBack(*number1);
			intVect.PushBack(*number2);

			Vector<int> copyIntVect = intVect;
			Assert::AreEqual(intVect.Size(), copyIntVect.Size());
			Assert::AreEqual(intVect.Capacity(), copyIntVect.Capacity());
			Assert::AreEqual(intVect.Front(), copyIntVect.Front());
			Assert::AreEqual(intVect.Back(), copyIntVect.Back());

			// pointer type test
			Vector<int*> intPtrVect;
			intPtrVect.PushBack(number1);
			intPtrVect.PushBack(number2);

			Vector<int*> copyIntPtrVect = intPtrVect;
			Assert::AreEqual(intPtrVect.Size(), copyIntPtrVect.Size());
			Assert::AreEqual(intPtrVect.Capacity(), copyIntPtrVect.Capacity());
			Assert::AreEqual(intPtrVect.Front(), copyIntPtrVect.Front());
			Assert::AreEqual(intPtrVect.Back(), copyIntPtrVect.Back());

			// class type test
			int number3 = 10;
			int number4 = 19;
			Foo foo1(number3, number1);
			Foo foo2(number4, number2);

			Vector<Foo> fooVect;
			fooVect.PushBack(foo1);
			fooVect.PushBack(foo2);

			Vector<Foo> copyFooVect = fooVect;
			Assert::AreEqual(fooVect.Size(), copyFooVect.Size());
			Assert::AreEqual(fooVect.Capacity(), copyFooVect.Capacity());
			Assert::IsTrue(fooVect.Front() == copyFooVect.Front());
			Assert::IsTrue(fooVect.Back() == copyFooVect.Back());
		}

		TEST_METHOD(VectorAssignmentOperatorTest)
		{
			// primitive type test
			int* number0 = new int(5);
			int* number1 = new int(10);

			Vector<int> intVector;
			intVector.PushBack(*number0);
			intVector.PushBack(*number1);

			Vector<int> newIntVect;
			newIntVect = intVector;
			Assert::AreEqual(intVector.Size(), newIntVect.Size());
			Assert::AreEqual(intVector.Capacity(), newIntVect.Capacity());
			Assert::AreEqual(intVector.Front(), newIntVect.Front());
			Assert::AreEqual(intVector.Back(), newIntVect.Back());

			// pointer type test
			Vector<int*> intPtrVect;
			intPtrVect.PushBack(number0);
			intPtrVect.PushBack(number1);

			Vector<int*> newIntPtrVect;
			newIntPtrVect = intPtrVect;
			Assert::AreEqual(intPtrVect.Size(), newIntPtrVect.Size());
			Assert::AreEqual(intPtrVect.Capacity(), newIntPtrVect.Capacity());
			Assert::AreEqual(intPtrVect.Front(), newIntPtrVect.Front());
			Assert::AreEqual(intPtrVect.Back(), newIntPtrVect.Back());

			// class type test
			int number4 = 10;
			int number5 = 19;
			Foo foo1(number4, number0);
			Foo foo2(number5, number1);

			Vector<Foo> fooVect;
			fooVect.PushBack(foo1);
			fooVect.PushBack(foo2);

			Vector<Foo> newFooVect;
			newFooVect = fooVect;
			Assert::AreEqual(fooVect.Size(), newFooVect.Size());
			Assert::AreEqual(fooVect.Capacity(), newFooVect.Capacity());
			Assert::IsTrue(fooVect.Front() == newFooVect.Front());
			Assert::IsTrue(fooVect.Back() == newFooVect.Back());
		}

		TEST_METHOD(VectorIsEmptyTest)
		{
			// primitive type test
			int* number1 = new int(5);

			Vector<int> intVect;
			Assert::IsTrue(intVect.IsEmpty());
			intVect.PushBack(*number1);
			Assert::IsFalse(intVect.IsEmpty());
			intVect.PopBack();
			Assert::IsTrue(intVect.IsEmpty());

			// pointer type test
			Vector<int*> intPtrVect;
			Assert::IsTrue(intPtrVect.IsEmpty());
			intPtrVect.PushBack(number1);
			Assert::IsFalse(intPtrVect.IsEmpty());
			intPtrVect.PopBack();
			Assert::IsTrue(intPtrVect.IsEmpty());

			// class type test
			int number2 = 10;
			Foo foo(number2, number1);

			Vector<Foo> fooVect;
			Assert::IsTrue(fooVect.IsEmpty());
			fooVect.PushBack(foo);
			Assert::IsFalse(fooVect.IsEmpty());
			fooVect.PopBack();
			Assert::IsTrue(fooVect.IsEmpty());
		}

		TEST_METHOD(VectorSizeTest)
		{
			// primitive type test
			int* number1 = new int(5);
			int* number2 = new int(0);

			Vector<int> intVect;
			Assert::AreEqual(intVect.Size(), 0U);
			intVect.PushBack(*number1);
			Assert::AreEqual(intVect.Size(), 1U);
			intVect.PushBack(*number2);
			Assert::AreEqual(intVect.Size(), 2U);
			intVect.PopBack();
			Assert::AreEqual(intVect.Size(), 1U);

			// pointer type test
			Vector<int*> intPtrVect;
			Assert::AreEqual(intPtrVect.Size(), 0U);
			intPtrVect.PushBack(number1);
			Assert::AreEqual(intPtrVect.Size(), 1U);
			intPtrVect.PushBack(number2);
			Assert::AreEqual(intPtrVect.Size(), 2U);
			intPtrVect.PopBack();
			Assert::AreEqual(intPtrVect.Size(), 1U);

			// class type test
			int number3 = 10;
			int number4 = 9;
			Foo foo1(number3, number1);
			Foo foo2(number4, number2);

			Vector<Foo> fooVect;
			Assert::AreEqual(fooVect.Size(), 0U);
			fooVect.PushBack(foo1);
			Assert::AreEqual(fooVect.Size(), 1U);
			fooVect.PushBack(foo2);
			Assert::AreEqual(fooVect.Size(), 2U);
			fooVect.PopBack();
			Assert::AreEqual(fooVect.Size(), 1U);
		}

		TEST_METHOD(VectorCapacityTest)
		{
			// primitive type test
			int* number1 = new int(5);

			Vector<int> intVect;
			Assert::AreEqual(intVect.Capacity(), 4U);
			intVect.Reserve(2u);
			Assert::AreEqual(intVect.Capacity(), 4U);
			intVect.PushBack(*number1);
			Assert::AreEqual(intVect.Capacity(), 4U);
			intVect.Clear();
			Assert::AreEqual(intVect.Capacity(), 0U);

			// pointer type test
			Vector<int*> intPtrVect;
			Assert::AreEqual(intPtrVect.Capacity(), 4U);
			intPtrVect.Reserve(2u);
			Assert::AreEqual(intPtrVect.Capacity(), 4U);
			intPtrVect.PushBack(number1);
			Assert::AreEqual(intPtrVect.Capacity(), 4U);
			intPtrVect.Clear();
			Assert::AreEqual(intPtrVect.Capacity(), 0U);

			// class type test
			int number2 = 10;
			Foo foo1(number2, number1);

			Vector<Foo> fooVect;
			Assert::AreEqual(fooVect.Capacity(), 4U);
			fooVect.Reserve(2u);
			Assert::AreEqual(fooVect.Capacity(), 4U);
			fooVect.PushBack(foo1);
			Assert::AreEqual(fooVect.Capacity(), 4U);
			fooVect.Clear();
			Assert::AreEqual(fooVect.Capacity(), 0U);
		}

		TEST_METHOD(VectorReserveTest)
		{
			// primitive type test
			int* number1 = new int(5);
			bool theBool;

			Vector<int> intVect;
			Assert::AreEqual(intVect.Capacity(), 4U);
			theBool = intVect.Reserve(2u);
			Assert::IsFalse(theBool);
			Assert::AreEqual(intVect.Capacity(), 4U);

			intVect.PushBack(*number1);
			Assert::AreEqual(intVect.Capacity(), 4U);

			theBool = intVect.Reserve(6u);
			Assert::IsTrue(theBool);
			Assert::AreEqual(intVect.Capacity(), 6U);

			// pointer type test
			Vector<int*> intPtrVect;
			Assert::AreEqual(intPtrVect.Capacity(), 4U);
			theBool = intPtrVect.Reserve(2u);
			Assert::IsFalse(theBool);
			Assert::AreEqual(intPtrVect.Capacity(), 4U);

			intPtrVect.PushBack(number1);
			Assert::AreEqual(intPtrVect.Capacity(), 4U);

			theBool = intPtrVect.Reserve(6u);
			Assert::IsTrue(theBool);
			Assert::AreEqual(intPtrVect.Capacity(), 6U);

			// class type test
			int number2 = 10;
			Foo foo1(number2, number1);

			Vector<Foo> fooVect;
			Assert::AreEqual(fooVect.Capacity(), 4U);
			theBool = fooVect.Reserve(2u);
			Assert::AreEqual(fooVect.Capacity(), 4U);

			fooVect.PushBack(foo1);
			Assert::AreEqual(fooVect.Capacity(), 4U);

			theBool = fooVect.Reserve(6u);
			Assert::IsTrue(theBool);
			Assert::AreEqual(fooVect.Capacity(), 6U);
		}

		TEST_METHOD(VectorBracketOperatorTest)
		{
			// primitive type test
			int number1 = 9;
			int number2 = 8;

			Vector<int> intVect;
			auto func1 = [&intVect] { intVect[0]; };
			Assert::ExpectException<std::exception>(func1);

			intVect.PushBack(number1);
			auto func3 = [&intVect] { intVect[1]; };
			Assert::ExpectException<std::exception>(func3);
			Assert::AreEqual(intVect[0], number1);

			intVect.PushBack(number2);
			Assert::AreEqual(intVect[1], number2);

			// pointer type test
			Vector<int*> intPtrVect;
			auto func4 = [&intPtrVect] { intPtrVect[0]; };
			Assert::ExpectException<std::exception>(func4);

			intPtrVect.PushBack(&number1);
			auto func6 = [&intPtrVect] { intPtrVect[1]; };
			Assert::ExpectException<std::exception>(func6);
			Assert::AreEqual(intPtrVect[0], &number1);

			intPtrVect.PushBack(&number2);
			Assert::AreEqual(intPtrVect[1], &number2);

			// class type test
			int* number3 = new int(10);
			int* number4 = new int(19);
			Foo foo1(number1, number3);
			Foo foo2(number2, number4);

			Vector<Foo> fooVect;
			auto func7 = [&fooVect] { fooVect[0]; };
			Assert::ExpectException<std::exception>(func7);

			fooVect.PushBack(foo1);
			auto func9 = [&fooVect] { fooVect[1]; };
			Assert::ExpectException<std::exception>(func9);
			Assert::IsTrue(fooVect[0] == foo1);

			fooVect.PushBack(foo2);
			Assert::IsTrue(fooVect[1] == foo2);
		}

		TEST_METHOD(VectorAtTest)
		{
			// primitive type test
			int number1 = 9;
			int number2 = 8;

			Vector<int> intVect;
			auto func1 = [&intVect] { intVect.At(0); };
			Assert::ExpectException<std::exception>(func1);

			intVect.PushBack(number1);
			auto func2 = [&intVect] { intVect.At(1); };
			Assert::ExpectException<std::exception>(func2);
			Assert::AreEqual(intVect.At(0), number1);

			intVect.PushBack(number2);
			Assert::AreEqual(intVect.At(1), number2);

			// pointer type test
			Vector<int*> intPtrVect;
			auto func3 = [&intPtrVect] { intPtrVect.At(0); };
			Assert::ExpectException<std::exception>(func3);

			intPtrVect.PushBack(&number1);
			auto func4 = [&intPtrVect] { intPtrVect.At(1); };
			Assert::ExpectException<std::exception>(func4);
			Assert::AreEqual(intPtrVect.At(0), &number1);

			intPtrVect.PushBack(&number2);
			Assert::AreEqual(intPtrVect.At(1), &number2);

			// class type test
			int* number3 = new int(10);
			int* number4 = new int(19);
			Foo foo1(number1, number3);
			Foo foo2(number2, number4);

			Vector<Foo> fooVect;
			auto func5 = [&fooVect] { fooVect.At(0); };
			Assert::ExpectException<std::exception>(func5);

			fooVect.PushBack(foo1);
			auto func6 = [&fooVect] { fooVect.At(1); };
			Assert::ExpectException<std::exception>(func6);
			Assert::IsTrue(fooVect.At(0) == foo1);

			fooVect.PushBack(foo2);
			Assert::IsTrue(fooVect.At(1) == foo2);
		}

		TEST_METHOD(VectorFrontTest)
		{
			// primitive type test
			int number1 = 9;

			Vector<int> intVect;
			auto func1 = [&intVect] { intVect.Front(); };
			Assert::ExpectException<std::exception>(func1);

			intVect.PushBack(number1);
			Assert::AreEqual(intVect.Front(), number1);

			// pointer type test
			Vector<int*> intPtrVect;
			auto func2 = [&intPtrVect] { intPtrVect.Front(); };
			Assert::ExpectException<std::exception>(func2);

			intPtrVect.PushBack(&number1);
			Assert::AreEqual(intPtrVect.Front(), &number1);

			// class type test
			int* number2 = new int(10);
			Foo foo1(number1, number2);

			Vector<Foo> fooVect;
			auto func3 = [&fooVect] { fooVect.Front(); };
			Assert::ExpectException<std::exception>(func3);

			fooVect.PushBack(foo1);
			Assert::IsTrue(fooVect.Front() == foo1);
		}

		TEST_METHOD(VectorBackTest)
		{
			// primitive type test
			int number1 = 9;
			int number2 = 8;

			Vector<int> intVect;
			auto func1 = [&intVect] { intVect.Back(); };
			Assert::ExpectException<std::exception>(func1);

			intVect.PushBack(number1);
			Assert::AreEqual(intVect.Back(), number1);
			intVect.PushBack(number2);
			Assert::AreEqual(intVect.Back(), number2);

			// pointer type test
			Vector<int*> intPtrVect;
			auto func2 = [&intPtrVect] { intPtrVect.Back(); };
			Assert::ExpectException<std::exception>(func2);

			intPtrVect.PushBack(&number1);
			Assert::AreEqual(intPtrVect.Back(), &number1);
			intPtrVect.PushBack(&number2);
			Assert::AreEqual(intPtrVect.Back(), &number2);

			// class type test
			int* number3 = new int(10);
			int* number4 = new int(19);
			Foo foo1(number1, number3);
			Foo foo2(number2, number4);

			Vector<Foo> fooVect;
			auto func3 = [&fooVect] { fooVect.Back(); };
			Assert::ExpectException<std::exception>(func3);

			fooVect.PushBack(foo1);
			Assert::IsTrue(fooVect.Back() == foo1);
			fooVect.PushBack(foo2);
			Assert::IsTrue(fooVect.Back() == foo2);
		}

		TEST_METHOD(VectorFindTest)
		{
			// primitive type test
			int number1 = 3;
			int number2 = 2;
			int number0 = 9;

			Vector<int> intVect;
			Vector<int>::Iterator it0 = intVect.Find(number0);
			Assert::IsTrue(it0 == intVect.end());

			intVect.PushBack(number1);
			intVect.PushBack(number2);
			it0 = intVect.Find(number0);
			Assert::IsTrue(it0 == intVect.end());

			it0 = intVect.Find(number2);
			Assert::IsTrue(*it0 == number2);

			// pointer type test
			Vector<int*> intPtrVect;
			Vector<int*>::Iterator it1 = intPtrVect.Find(&number0);
			Assert::IsTrue(it1 == intPtrVect.end());

			intPtrVect.PushBack(&number1);
			intPtrVect.PushBack(&number2);
			it1 = intPtrVect.Find(&number0);
			Assert::IsTrue(it1 == intPtrVect.end());

			it1 = intPtrVect.Find(&number2);
			Assert::IsTrue(*it1 == &number2);

			// class type test
			int* number3 = new int(10);
			int* number4 = new int(6);
			int* number5 = new int(98);

			Foo foo1(number1, number3);
			Foo foo2(number2, number4);
			Foo foo0(number0, number5);

			Vector<Foo> fooList;
			Vector<Foo>::Iterator it2 = fooList.Find(foo0);
			Assert::IsTrue(it2 == fooList.end());

			fooList.PushBack(foo1);
			fooList.PushBack(foo2);
			it2 = fooList.Find(foo0);
			Assert::IsTrue(it2 == fooList.end());

			it2 = fooList.Find(foo2);
			Assert::IsTrue(*it2 == foo2);
		}

		TEST_METHOD(VectorPushBackTest)
		{
			int number1 = 3;
			int number2 = 7;

			// primitive type test
			Vector<int> intVector;
			intVector.PushBack(number1);
			Assert::AreEqual(intVector.Back(), intVector.Front());
			Assert::AreEqual(intVector.Back(), number1);

			intVector.PushBack(number1);
			intVector.PushBack(number2);
			intVector.PushBack(number2);
			Assert::AreEqual(intVector.Size(), 4U);
			Assert::AreEqual(intVector.Capacity(), 4U);
			Assert::AreNotEqual(intVector.Back(), intVector.Front());
			Assert::AreEqual(number2, intVector.Back());

			intVector.PushBack(number1);
			Assert::AreEqual(intVector.Capacity(), 8U);

			// pointer type test
			Vector<int*> intPtrVector;
			intPtrVector.PushBack(&number1);
			Assert::AreEqual(intPtrVector.Back(), intPtrVector.Front());
			Assert::AreEqual(intPtrVector.Back(), &number1);

			intPtrVector.PushBack(&number1);
			intPtrVector.PushBack(&number2);
			intPtrVector.PushBack(&number2);
			Assert::AreEqual(intPtrVector.Size(), 4U);
			Assert::AreEqual(intPtrVector.Capacity(), 4U);
			Assert::AreNotEqual(intPtrVector.Back(), intPtrVector.Front());
			Assert::AreEqual(&number2, intPtrVector.Back());

			intPtrVector.PushBack(&number1);
			Assert::AreEqual(intPtrVector.Capacity(), 8U);

			// class type test
			int* number3 = new int(10);
			int* number4 = new INT(19);
			Foo foo1(number1, number3);
			Foo foo2(number2, number4);

			Vector<Foo> fooVector;
			fooVector.PushBack(foo1);
			Assert::IsTrue(fooVector.Back() == fooVector.Front());
			Assert::IsTrue(fooVector.Back() == foo1);

			fooVector.PushBack(foo1);
			fooVector.PushBack(foo2);
			fooVector.PushBack(foo2);
			Assert::AreEqual(fooVector.Size(), 4U);
			Assert::AreEqual(fooVector.Capacity(), 4U);
			Assert::IsFalse(fooVector.Back() == fooVector.Front());
			Assert::IsTrue(foo2 == fooVector.Back());

			fooVector.PushBack(foo1);
			Assert::AreEqual(fooVector.Capacity(), 8U);
		}

		TEST_METHOD(VectorPopBackTest)
		{
			int number1 = 3;
			int number2 = 7;

			// primitive type test
			Vector<int> intList;
			intList.PopBack();
			intList.PushBack(number1);
			intList.PushBack(number2);
			intList.PopBack();

			Assert::AreEqual(intList.Size(), 1U);
			Assert::AreEqual(intList.Back(), intList.Front());
			Assert::AreEqual(number1, intList.Back());

			intList.PopBack();
			Assert::IsTrue(intList.IsEmpty());

			// pointer type test
			Vector<int*> intPtrList;
			intPtrList.PopBack();
			intPtrList.PushBack(&number1);
			intPtrList.PushBack(&number2);
			intPtrList.PopBack();

			Assert::AreEqual(intPtrList.Size(), 1U);
			Assert::AreEqual(intPtrList.Back(), intPtrList.Front());
			Assert::AreEqual(&number1, intPtrList.Back());

			intPtrList.PopBack();
			Assert::IsTrue(intPtrList.IsEmpty());

			// class type test
			int* number3 = new int(10);
			int* number4 = new int(19);
			Foo foo1(number1, number3);
			Foo foo2(number2, number4);

			Vector<Foo> fooList;
			fooList.PopBack();
			fooList.PushBack(foo1);
			fooList.PushBack(foo2);
			fooList.PopBack();

			Assert::AreEqual(fooList.Size(), 1U);
			Assert::IsTrue(fooList.Back() == fooList.Front());
			Assert::IsTrue(foo1 == fooList.Back());

			fooList.PopBack();
			Assert::IsTrue(fooList.IsEmpty());
		}

		TEST_METHOD(VectorRemoveTest)
		{
			// primitive type test
			int number0 = 9;
			int number1 = 3;
			int number2 = 2;
			bool theBool;

			Vector<int> intVect;
			theBool = intVect.Remove(number1);
			Assert::IsFalse(theBool);

			intVect.PushBack(number0);
			intVect.PushBack(number1);
			intVect.PushBack(number2);
			theBool = intVect.Remove(number1);
			Assert::IsTrue(theBool);
			Assert::AreEqual(intVect.Size(), 2U);

			theBool = intVect.Remove(number1);
			Assert::IsFalse(theBool);
			Assert::AreEqual(intVect.Size(), 2U);

			// pointer type test
			Vector<int*> intPtrVect;
			Assert::IsFalse(intPtrVect.Remove(&number1));

			intPtrVect.PushBack(&number0);
			intPtrVect.PushBack(&number1);
			intPtrVect.PushBack(&number2);
			theBool = intPtrVect.Remove(&number1);
			Assert::IsTrue(theBool);
			Assert::AreEqual(intPtrVect.Size(), 2U);

			theBool = intPtrVect.Remove(&number1);
			Assert::IsFalse(theBool);
			Assert::AreEqual(intPtrVect.Size(), 2U);

			// class type test
			int* number3 = new int(10);
			int* number4 = new int(6);
			int* number5 = new int(98);

			Foo foo1(number1, number3);
			Foo foo2(number2, number4);
			Foo foo0(number0, number5);

			Vector<Foo> fooVect;
			Assert::IsFalse(fooVect.Remove(foo1));

			fooVect.PushBack(foo0);
			fooVect.PushBack(foo1);
			fooVect.PushBack(foo2);
			theBool = fooVect.Remove(foo1);
			Assert::IsTrue(theBool);
			Assert::AreEqual(fooVect.Size(), 2U);

			theBool = fooVect.Remove(foo1);
			Assert::IsFalse(theBool);
			Assert::AreEqual(fooVect.Size(), 2U);
		}

		TEST_METHOD(VectorBeginTest)
		{
			// primitive type test
			int number1 = 3;
			int number2 = 2;

			Vector<int> intVect;
			auto it1 = intVect.PushBack(number1);
			Assert::IsTrue(it1 == intVect.begin());
			it1 = intVect.PushBack(number2);
			Assert::IsTrue(it1 != intVect.begin());

			// pointer type test
			Vector<int*> intPtrVect;
			auto it2 = intPtrVect.PushBack(&number1);
			Assert::IsTrue(it2 == intPtrVect.begin());
			it2 = intPtrVect.PushBack(&number2);
			Assert::IsTrue(it2 != intPtrVect.begin());

			// class type test
			int* number3 = new int(10);
			int* number4 = new int(6);
			Foo foo1(number1, number3);
			Foo foo2(number2, number4);

			Vector<Foo> fooList;
			auto it3 = fooList.PushBack(foo1);
			Assert::IsTrue(it3 == fooList.begin());
			it3 = fooList.PushBack(foo2);
			Assert::IsTrue(it3 != fooList.begin());
		}

		TEST_METHOD(VectorEndTest)
		{
			// primitive type test
			int number1 = 3;

			Vector<int> intVect;
			Assert::IsTrue(intVect.begin() == intVect.end());

			auto it1 = intVect.PushBack(number1);
			Assert::IsTrue(it1 != intVect.end());
			Assert::IsTrue(++it1 == intVect.end());

			// pointer type test
			Vector<int*> intPtrVect;
			Assert::IsTrue(intPtrVect.begin() == intPtrVect.end());

			auto it2 = intPtrVect.PushBack(&number1);
			Assert::IsTrue(it2 != intPtrVect.end());
			Assert::IsTrue(++it2 == intPtrVect.end());

			// class type test
			int* number2 = new int(10);
			Foo foo1(number1, number2);

			Vector<Foo> fooVect;
			Assert::IsTrue(fooVect.begin() == fooVect.end());

			auto it3 = fooVect.PushBack(foo1);
			Assert::IsTrue(it3 != fooVect.end());
			Assert::IsTrue(++it3 == fooVect.end());
		}

		TEST_METHOD(VectorRemoveRangeTest)
		{
			// primitive type test
			int number0 = 11;
			int number1 = 84;
			int number2 = 25;
			int number3 = 54;
			bool theBool;

			Vector<int> intVect;
			intVect.PushBack(number0);

			auto intVect2 = intVect;
			auto it0 = intVect2.begin();
			auto func1 = [&intVect, &it0] { intVect.Remove(it0, it0); };
			Assert::ExpectException<std::exception>(func1);
			it0 = intVect2.end();
			auto func2 = [&intVect, &it0] { intVect.Remove(it0, it0); };
			Assert::ExpectException<std::exception>(func2);

			it0 = intVect.PushBack(number0);
			intVect.PopBack();
			auto func3 = [&intVect, &it0] { intVect.Remove(it0, it0); };
			Assert::ExpectException<std::exception>(func3);

			it0 = intVect.PushBack(number1);
			intVect.PushBack(number2);
			auto it1 = intVect.PushBack(number3);
			intVect.PushBack(number0);

			theBool = intVect.Remove(it1, it0);
			Assert::IsFalse(theBool);

			intVect.PushBack(number0);
			Assert::AreEqual(intVect.Size(), 6u);
			theBool = intVect.Remove(--intVect.end(), --intVect.end());
			Assert::IsTrue(theBool);
			Assert::AreEqual(intVect.Size(), 5u);

			theBool = intVect.Remove(it0, it1);
			Assert::IsTrue(theBool);
			Assert::AreEqual(intVect.Size(), 3u);
			Assert::IsTrue(intVect.Find(number1) == intVect.end());
			Assert::IsTrue(intVect.Find(number2) == intVect.end());
			Assert::AreEqual(intVect[1], number3);

			// pointer type test
			Vector<int*> intPtrVect;
			intPtrVect.PushBack(&number0);

			auto intPtrVect2 = intPtrVect;
			auto it2 = intPtrVect2.begin();
			auto func4 = [&intPtrVect, &it2] { intPtrVect.Remove(it2, it2); };
			Assert::ExpectException<std::exception>(func4);
			it2 = intPtrVect2.end();
			auto func5 = [&intPtrVect, &it2] { intPtrVect.Remove(it2, it2); };
			Assert::ExpectException<std::exception>(func5);

			it2 = intPtrVect.PushBack(&number0);
			intPtrVect.PopBack();
			auto func6 = [&intPtrVect, &it2] { intPtrVect.Remove(it2, it2); };
			Assert::ExpectException<std::exception>(func6);

			it2 = intPtrVect.PushBack(&number1);
			intPtrVect.PushBack(&number2);
			auto it3 = intPtrVect.PushBack(&number3);
			intPtrVect.PushBack(&number0);

			theBool = intPtrVect.Remove(it3, it2);
			Assert::IsFalse(theBool);

			intPtrVect.PushBack(&number0);
			Assert::AreEqual(intPtrVect.Size(), 6u);
			theBool = intPtrVect.Remove(--intPtrVect.end(), --intPtrVect.end());
			Assert::IsTrue(theBool);
			Assert::AreEqual(intPtrVect.Size(), 5u);

			theBool = intPtrVect.Remove(it2, it3);
			Assert::IsTrue(theBool);
			Assert::AreEqual(intPtrVect.Size(), 3u);
			Assert::IsTrue(intPtrVect.Find(&number1) == intPtrVect.end());
			Assert::IsTrue(intPtrVect.Find(&number2) == intPtrVect.end());
			Assert::AreEqual(intPtrVect[1], &number3);


			// class type test
			int* number4 = new int(10);
			int* number7 = new int(10);
			int* number6 = new int(6);
			int* number5 = new int(98);

			Foo foo0(number0, number4);
			Foo foo1(number1, number5);
			Foo foo2(number2, number6);
			Foo foo3(number3, number7);

			Vector<Foo> fooVect;
			fooVect.PushBack(foo0);

			auto fooVect2 = fooVect;
			auto it4 = fooVect2.begin();
			auto func7 = [&fooVect, &it4] { fooVect.Remove(it4, it4); };
			Assert::ExpectException<std::exception>(func7);
			it4 = fooVect2.end();
			auto func8 = [&fooVect, &it4] { fooVect.Remove(it4, it4); };
			Assert::ExpectException<std::exception>(func8);

			it4 = fooVect.PushBack(foo0);
			fooVect.PopBack();
			auto func9 = [&fooVect, &it4] { fooVect.Remove(it4, it4); };
			Assert::ExpectException<std::exception>(func9);

			it4 = fooVect.PushBack(foo1);
			fooVect.PushBack(foo2);
			auto it5 = fooVect.PushBack(foo3);
			fooVect.PushBack(foo0);

			theBool = fooVect.Remove(it5, it4);
			Assert::IsFalse(theBool);

			fooVect.PushBack(foo0);
			Assert::AreEqual(fooVect.Size(), 6u);
			theBool = fooVect.Remove(--fooVect.end(), --fooVect.end());
			Assert::IsTrue(theBool);
			Assert::AreEqual(fooVect.Size(), 5u);

			theBool = fooVect.Remove(it4, it5);
			Assert::IsTrue(theBool);
			Assert::AreEqual(fooVect.Size(), 3u);
			Assert::IsTrue(fooVect.Find(foo1) == fooVect.end());
			Assert::IsTrue(fooVect.Find(foo2) == fooVect.end());
			Assert::IsTrue(fooVect[1] == foo3);
		}

		TEST_METHOD(VectorClearTest)
		{
			// primitive type test
			int number1 = 5;
			int number2 = 0;

			Vector<int> intVect;
			intVect.PushBack(number1);
			intVect.PushBack(number2);
			intVect.Clear();
			Assert::IsTrue(intVect.IsEmpty());

			// pointer type test
			Vector<int*> intPtrVect;
			intPtrVect.PushBack(&number1);
			intPtrVect.PushBack(&number2);
			intPtrVect.Clear();
			Assert::IsTrue(intPtrVect.IsEmpty());

			// class type test
			int* number3 = new int(10);
			int* number4 = new int(9);
			Foo foo1(number1, number3);
			Foo foo2(number2, number4);

			Vector<Foo> fooVect;
			fooVect.PushBack(foo1);
			fooVect.PushBack(foo2);
			fooVect.Clear();
			Assert::IsTrue(fooVect.IsEmpty());
		}

		TEST_METHOD(EqualityAndNonEqualityTest)
		{
			int a1 = 5;
			int a2 = 9;
			int* a3 = new int(65);
			int* a4 = new int(78);
			Foo f1(a1, a3);
			Foo f2(a2, a4);
			EqualityAndNonEqualityTemplate<int>(a1, a2);
			EqualityAndNonEqualityTemplate<int*>(&a1, &a2);
			EqualityAndNonEqualityTemplate<Foo>(f1, f2);
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState VectorTest::sStartMemState;
}