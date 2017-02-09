#include "pch.h"
#include "CppUnitTest.h"
#include "HashMap.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;

namespace UnitTestLibraryDesktop
{		
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

		TEST_METHOD(HashFunctorTest)
		{

		}

#pragma endregion

#pragma region HashMap

		TEST_METHOD(Ctor)
		{
			HashMap<int, int> c(10);
		}

#pragma endregion

#pragma region Iterator



#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState HashMapTest::sStartMemState;

}