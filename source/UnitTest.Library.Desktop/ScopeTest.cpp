#include "pch.h"
#include "CppUnitTest.h"
#include "Scope.h"
#include "HashMap.h"
#include "Vector.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace glm;

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(ScopeTest)
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

#pragma region Template Methods

		/************************************************************************/
		template<typename T>
		void ScopeCtorsAndAssignmentOperatorTemplate(Datum::DatumType type)
		{
			T t1;
			Scope scope1;
			auto& dat = scope1.Append("Test", type);
			dat.PushBack(t1);
			Scope scope2(scope1);
			Assert::IsTrue(scope1 == scope2);

			Scope& childScope = scope1.AppendScope("child scope");
			childScope.Append("Yo", type);
			Assert::IsTrue(scope1 != scope2);
			Scope scope3(scope1);
			Assert::IsTrue(scope1 == scope3);

			scope2 = scope1;
			Assert::IsTrue(scope1 == scope2);
		}

		/************************************************************************/
		template<typename T>
		void ScopeFindTemplate(Datum::DatumType type)
		{
			Scope scope1;
			auto func = [&scope1] { scope1.Find(""); };
			Assert::ExpectException<invalid_argument>(func);
			Assert::IsNull(scope1.Find("Paul"));

			T t1;
			auto& dat = scope1.Append("Hey");
			dat.PushBack(t1);
			Datum* datptr = scope1.Find("Hey");
			Assert::IsNotNull(datptr);
			Assert::IsTrue(dat == *datptr);

			Scope& childScope = scope1.AppendScope("child scope");
			childScope.Append("Yo", type);
			Datum* datptr2 = scope1.Find("child scope");
			Assert::IsTrue(childScope == (*datptr2)[0]);
		}

		/************************************************************************/
		template<typename T>
		void ScopeSearchTemplate()
		{
			Scope scope1;
			Scope* foundScope = nullptr;
			auto func = [&scope1] { scope1.Search(""); };
			Assert::ExpectException<invalid_argument>(func);
			Assert::IsNull(scope1.Search("Paul", &foundScope));
			Assert::IsNull(foundScope);

			T t1;
			auto& dat = scope1.Append("JJ");
			dat.PushBack(t1);
			Datum* datptr = scope1.Search("JJ", &foundScope);
			Assert::IsNotNull(datptr);
			Assert::IsTrue(dat == *datptr);
			Assert::IsTrue(foundScope == &scope1);

			Scope& childScope = scope1.AppendScope("child scope");
			auto& dat2 = childScope.Append("Yo");
			T t2;
			dat2.PushBack(t2);
			Assert::IsNull(scope1.Search("Yo"));

			foundScope = nullptr;
			Datum* datptr2 = childScope.Search("JJ", &foundScope);
			Assert::IsTrue(dat == *datptr2);
			Assert::IsTrue(foundScope == &scope1);
		}

		/************************************************************************/
		template<typename T>
		void ScopeAppendTemplate()
		{
			Scope scope1;
			auto func = [&scope1] { scope1.Append(""); };
			Assert::ExpectException<invalid_argument>(func);

			T t1;
			auto& dat = scope1.Append("Hey");
			dat.PushBack(t1);
			Datum* datptr = scope1.Find("Hey");
			Assert::IsNotNull(datptr);
			Assert::IsTrue(dat == *datptr);

			auto& dat2 = scope1.Append("Hey");
			Assert::IsTrue(dat2 == *datptr);
		}

		/************************************************************************/
		template<typename T>
		void ScopeAppendScopeTemplate()
		{
			Scope scope1;
			auto func = [&scope1] { scope1.AppendScope(""); };
			Assert::ExpectException<invalid_argument>(func);

			T t1;
			auto& dat = scope1.Append("Hey");
			dat.PushBack(t1);
			auto func1 = [&scope1] { scope1.AppendScope("Hey"); };
			Assert::ExpectException<exception>(func1);

			Scope& scope2 = scope1.AppendScope("Yo");
			Scope& scope3 = scope1.AppendScope("Yo");
			Datum* datptr1 = scope1.Find("Yo");
			Assert::IsTrue(scope2 == (*datptr1)[0]);
			Assert::IsTrue(scope3 == (*datptr1)[1]);
		}

		/************************************************************************/
		template<typename T>
		void ScopeAdoptTemplate()
		{
			Scope scope1;
			auto func = [&scope1] { scope1.Adopt(scope1, "Hey"); };
			Assert::ExpectException<invalid_argument>(func);

			Scope* scope2 = new Scope();
			scope1.Adopt(*scope2, "Hey");
			Assert::IsTrue(scope2->GetParent() == &scope1);
			Datum* datptr1 = scope1.Find("Hey");
			Assert::IsTrue(*scope2 == (*datptr1)[0]);

			Scope scope3;
			scope3.Adopt(*scope2, "Yo");
			Assert::IsTrue(scope2->GetParent() == &scope3);
			Datum* datptr2 = scope3.Find("Yo");
			Assert::IsTrue(*scope2 == (*datptr2)[0]);
			Assert::IsFalse(scope2->GetParent() == &scope1);			
		}

		/************************************************************************/
		template<typename T>
		void ScopeOperatorBracketTemplate()
		{
			Scope scope1;
			auto func = [&scope1] { scope1[""]; };
			Assert::ExpectException<invalid_argument>(func);

			T t1;
			auto& dat = scope1["Hey"];
			dat.PushBack(t1);
			Datum* datptr = scope1.Find("Hey");
			Assert::IsNotNull(datptr);
			Assert::IsTrue(dat == *datptr);

			auto& dat2 = scope1["Hey"];
			Assert::IsTrue(dat2 == *datptr);
			Assert::IsTrue(scope1[0] == *datptr);

			T t2;
			auto& dat3 = scope1["uy"];
			dat3.PushBack(t2);
			Datum* datptr2 = scope1.Find("uy");
			Assert::IsTrue(scope1[1] == *datptr2);
		}

		/************************************************************************/
		template<typename T>
		void ScopeEqualityTemplate()
		{
			T t1;
			Scope scope1;
			auto& dat = scope1.Append("Test");
			dat.PushBack(t1);
			Scope scope2(scope1);
			Assert::IsTrue(scope1 == scope2);
			Assert::IsFalse(scope1 != scope2);
			Assert::IsTrue(scope1.Equals(&scope2));

			Scope& childScope = scope1.AppendScope("child scope");
			auto& dat2 = childScope.Append("Yo");
			dat2.PushBack(t1);
			Assert::IsFalse(scope1 == scope2);
			Assert::IsTrue(scope1 != scope2);
			Assert::IsFalse(scope1.Equals(&scope2));

			Scope scope3(scope1);
			Assert::IsTrue(scope1 == scope3);
			Assert::IsFalse(scope1 != scope3);
			Assert::IsTrue(scope1.Equals(&scope3));
		}

		/************************************************************************/
		template<typename T>
		void ScopeFindNameTemplate()
		{
			Scope scope1, scope2;
			Assert::IsTrue(scope1.FindName(&scope2) == "");

			Scope& childScope = scope1.AppendScope("child scope");
			Assert::IsTrue(scope1.FindName(&childScope) == "child scope");
		}

		/************************************************************************/
		template<typename T>
		void ScopeClearTemplate()
		{
			Scope scope1;
			T t1;
			auto& dat = scope1.Append("JJ");
			dat.PushBack(t1);

			Scope& childScope = scope1.AppendScope("child scope");
			auto& dat2 = childScope.Append("Yo");
			T t2;
			dat2.PushBack(t2);

			scope1.Clear();
			Assert::IsNull(scope1.Find("JJ"));
			Assert::IsNull(scope1.Find("child scope"));

			auto& dat3 = scope1.Append("JJ");
			dat3.PushBack(t1);

			Datum* datPrt = scope1.Find("JJ");

			Assert::IsNotNull(datPrt);
			Assert::IsTrue(*datPrt == dat3);

		}

		/************************************************************************/
		template<typename T>
		void ScopeToStringTemplate(const T& t1, const T& t2, const string& str1, const string& str2)
		{
			Scope scope1;
			auto& dat = scope1.Append("JJ");
			dat.PushBack(t1);

			Scope& childScope = scope1.AppendScope("child scope");
			auto& dat2 = childScope.Append("Yo");
			dat2.PushBack(t2);

			string result = scope1.ToString();
			bool b = (result.find(str1) != string::npos);
			Assert::IsTrue(b);
			b = (result.find(str2) != string::npos);
			Assert::IsTrue(b);
		}

#pragma endregion


#pragma region Test Methods

		/************************************************************************/
		TEST_METHOD(ScopeCtorsAndAssignmentOperatorTest)
		{
			ScopeCtorsAndAssignmentOperatorTemplate<int32_t>(Datum::DatumType::Integer);
			ScopeCtorsAndAssignmentOperatorTemplate<std::float_t>(Datum::DatumType::Float);
			ScopeCtorsAndAssignmentOperatorTemplate<vec4>(Datum::DatumType::Vector);
			ScopeCtorsAndAssignmentOperatorTemplate<mat4x4>(Datum::DatumType::Matrix);
			ScopeCtorsAndAssignmentOperatorTemplate<string>(Datum::DatumType::String);
		}

		/************************************************************************/
		TEST_METHOD(ScopeFindTest)
		{
			ScopeFindTemplate<int32_t>(Datum::DatumType::Integer);
			ScopeFindTemplate<std::float_t>(Datum::DatumType::Float);
			ScopeFindTemplate<vec4>(Datum::DatumType::Vector);
			ScopeFindTemplate<mat4x4>(Datum::DatumType::Matrix);
			ScopeFindTemplate<string>(Datum::DatumType::String);
		}

		/************************************************************************/
		TEST_METHOD(ScopeSearchTest)
		{
			ScopeSearchTemplate<int32_t>();
			ScopeSearchTemplate<std::float_t>();
			ScopeSearchTemplate<vec4>();
			ScopeSearchTemplate<mat4x4>();
			ScopeSearchTemplate<string>();
		}

		/************************************************************************/
		TEST_METHOD(ScopeAppendTest)
		{
			ScopeAppendTemplate<int32_t>();
			ScopeAppendTemplate<std::float_t>();
			ScopeAppendTemplate<vec4>();
			ScopeAppendTemplate<mat4x4>();
			ScopeAppendTemplate<string>();
		}

		/************************************************************************/
		TEST_METHOD(ScopeAppendScopeTest)
		{
			ScopeAppendScopeTemplate<int32_t>();
			ScopeAppendScopeTemplate<std::float_t>();
			ScopeAppendScopeTemplate<vec4>();
			ScopeAppendScopeTemplate<mat4x4>();
			ScopeAppendScopeTemplate<string>();
		}

		/************************************************************************/
		TEST_METHOD(ScopeAdoptTest)
		{
			ScopeAdoptTemplate<int32_t>();
			ScopeAdoptTemplate<std::float_t>();
			ScopeAdoptTemplate<vec4>();
			ScopeAdoptTemplate<mat4x4>();
			ScopeAdoptTemplate<string>();
		}

		/************************************************************************/
		TEST_METHOD(ScopeGetParentTest)
		{
			Scope scope1;
			Assert::IsNull(scope1.GetParent());
			Scope& scope2 = scope1.AppendScope("po");
			Assert::IsTrue(scope2.GetParent() == &scope1);
		}

		/************************************************************************/
		TEST_METHOD(ScopeOperatorBracketTest)
		{
			ScopeOperatorBracketTemplate<int32_t>();
			ScopeOperatorBracketTemplate<std::float_t>();
			ScopeOperatorBracketTemplate<vec4>();
			ScopeOperatorBracketTemplate<mat4x4>();
			ScopeOperatorBracketTemplate<string>();
		}

		/************************************************************************/
		TEST_METHOD(ScopeEqualityTest)
		{
			ScopeEqualityTemplate<int32_t>();
			ScopeEqualityTemplate<std::float_t>();
			ScopeEqualityTemplate<vec4>();
			ScopeEqualityTemplate<mat4x4>();
			ScopeEqualityTemplate<string>();
		}

		/************************************************************************/
		TEST_METHOD(ScopeFindNameTest)
		{
			ScopeFindNameTemplate<int32_t>();
			ScopeFindNameTemplate<std::float_t>();
			ScopeFindNameTemplate<vec4>();
			ScopeFindNameTemplate<mat4x4>();
			ScopeFindNameTemplate<string>();
		}

		/************************************************************************/
		TEST_METHOD(ScopeClearTest)
		{
			ScopeClearTemplate<int32_t>();
			ScopeClearTemplate<std::float_t>();
			ScopeClearTemplate<vec4>();
			ScopeClearTemplate<mat4x4>();
			ScopeClearTemplate<string>();
		}

		/************************************************************************/
		TEST_METHOD(ScopeToStringTest)
		{
			int32_t i1 = 5, i2 = 9;
			std::float_t f1 = 5.6f, f2 = 9.3f;
			vec4 v1(6), v2(9), v3(1, 2, 4, 3), v4(5, 6, 7, 8);
			mat4x4 m1(4), m2(0);
			mat4x4 m3(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
			mat4x4 m4(17, 27, 37, 47, 57, 67, 77, 87, 97, 107, 117, 127, 137, 147, 157, 187);
			string str1("yo"), str2("paul");

			ScopeToStringTemplate(i1, i2, "5", "9");
			ScopeToStringTemplate(f1, f2, "5.6", "9.3");
			ScopeToStringTemplate(v1, v2, "6", "9");
			ScopeToStringTemplate(v3, v4, "4", "8");
			ScopeToStringTemplate(m1, m2, "4", "0");
			ScopeToStringTemplate(m3, m4, "16", "187");
			ScopeToStringTemplate(str1, str2, "yo", "paul");
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState ScopeTest::sStartMemState;
}