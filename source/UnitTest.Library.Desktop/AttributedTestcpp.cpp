#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include "AttributedFoo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace glm;
using namespace Library;

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(AttributedTest)
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
			AttributedFoo::ClearCashedAttributes();

			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}
#pragma endregion

#pragma region Test Methods

		/************************************************************************/
		TEST_METHOD(ConstructorsAndAssignmentOperatorTest)
		{
			AttributedFoo attr1;
			AttributedFoo attr2(attr1);
			Assert::IsFalse(attr1 == attr2);

			Datum* dat1 = attr1.Find("this");
			Datum* dat2 = attr2.Find("this");
			Assert::IsFalse(*dat1 == *dat2);

			Datum* dat3 = attr1.Find("InternalInteger");
			Datum* dat4 = attr2.Find("InternalInteger");
			Assert::IsTrue(*dat3 == *dat4);

			Datum* dat5 = attr1.Find("mExternalInteger");
			Datum* dat6 = attr2.Find("mExternalInteger");
			Assert::IsTrue(dat5 != dat6);
			Assert::IsTrue(*dat5 == *dat6);

			attr1.AppendAuxiliaryAttribute("Test");
			AttributedFoo attr3 = attr1;
			Assert::IsNotNull(attr3.Find("Test"));

			attr2 = attr3;
			Datum* dat7 = attr2.Find("InternalInteger");
			Datum* dat8 = attr3.Find("InternalInteger");
			Assert::IsTrue(*dat7 == *dat8);

			Datum* dat9 = attr2.Find("mExternalInteger");
			Datum* dat10 = attr3.Find("mExternalInteger");
			Assert::IsTrue(*dat9 == *dat10);

			Assert::IsNotNull(attr2.Find("Test"));
		}

		/************************************************************************/
		TEST_METHOD(IsAttributeTest)
		{
			AttributedFoo attr;
			attr.AppendAuxiliaryAttribute("Test");
			Assert::IsTrue(attr.IsAttribute("InternalString"));
			Assert::IsTrue(attr.IsAttribute("mExternalFloat"));
			Assert::IsTrue(attr.IsAttribute("NestedScope"));
			Assert::IsTrue(attr.IsAttribute("Test"));
		}

		TEST_METHOD(IsPrescribedAttributeTest)
		{
			AttributedFoo attr;
			attr.AppendAuxiliaryAttribute("Test");
			Assert::IsTrue(attr.IsPrescribedAttribute("InternalString"));
			Assert::IsTrue(attr.IsPrescribedAttribute("mExternalFloat"));
			Assert::IsTrue(attr.IsPrescribedAttribute("NestedScope"));
			Assert::IsFalse(attr.IsPrescribedAttribute("Test"));
		}

		/************************************************************************/
		TEST_METHOD(IsAuxiliaryAttributeTest)
		{
			AttributedFoo attr;
			attr.AppendAuxiliaryAttribute("Test");
			Assert::IsFalse(attr.IsAuxiliaryAttribute("InternalString"));
			Assert::IsFalse(attr.IsAuxiliaryAttribute("mExternalFloat"));
			Assert::IsFalse(attr.IsAuxiliaryAttribute("NestedScope"));
			Assert::IsTrue(attr.IsAuxiliaryAttribute("Test"));
		}

		/************************************************************************/
		TEST_METHOD(AppendAuxiliaryAttributeTest)
		{
			AttributedFoo attr;
			attr.AppendAuxiliaryAttribute("Test1");
			attr.AppendAuxiliaryAttribute("Test2");
			Assert::IsTrue(attr.IsAuxiliaryAttribute("Test1"));
			Assert::IsTrue(attr.IsAuxiliaryAttribute("Test2"));
			Assert::AreEqual(attr.AuxiliaryBegin(), 26U);

		}

#pragma endregion


	private:
		static _CrtMemState sStartMemState;


	};

	_CrtMemState AttributedTest::sStartMemState;
}