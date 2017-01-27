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


	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState VectorTest::sStartMemState;
}