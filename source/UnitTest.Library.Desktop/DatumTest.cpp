#include "pch.h"
#include "CppUnitTest.h"
#include "Datum.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace glm;

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(DatumTest)
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
		void ConstructorsTemplateMethod(T& t, Datum::DatumType type)
		{
			Datum dat;
			Assert::IsTrue(dat.IsEmpty());
			Assert::IsTrue(dat.Type() == Datum::DatumType::Unknown);
			dat.PushBack(t);
			Datum dat2(dat);
			dat.Clear();

			Assert::AreEqual(1u, dat2.Size());
			Assert::IsTrue(dat2.Type() == type);
		}

		/************************************************************************/
		template<typename T>
		void AssignmentTemplateMethod()
		{
			// 4 possibilities: lhs and rhs either or both external
			Datum dat1;
			dat1.PushBack(T());
			dat1.PushBack(T());

			Datum dat2;
			dat2.PushBack(T());
			Assert::IsFalse(dat2.HasExternalStorage());

			// internal = internal
			dat1 = dat2;
			Assert::AreEqual(1u, dat1.Size());
			Assert::IsFalse(dat1.HasExternalStorage());

			Datum dat3;
			T* ar3 = new T[2];
			dat3.SetStorage(ar3, 2u);
			Assert::IsTrue(dat3.HasExternalStorage());

			// internal = external
			dat1 = dat3;
			Assert::AreEqual(2u, dat1.Size());
			Assert::IsTrue(dat1.HasExternalStorage());

			// external = internal
			dat1 = dat2;
			Assert::AreEqual(1u, dat1.Size());
			Assert::IsFalse(dat1.HasExternalStorage());

			Datum dat4;
			T* ar4 = new T[5];
			dat4.SetStorage(ar4, 5u);

			// external = external
			dat3 = dat4;
			Assert::AreEqual(5u, dat3.Size());
			Assert::IsTrue(dat3.HasExternalStorage());

			delete[] ar3;
			delete[] ar4;
		}

		/************************************************************************/
		template<typename T, typename U>
		void ScalarAssignmentTemplateMethod(Datum::DatumType type)
		{
			Datum dat;
			dat = T();
			Assert::IsTrue(dat.Type() == type);

			auto func1 = [&dat] { dat = U(); };
			Assert::ExpectException<exception>(func1);
		}

		/************************************************************************/
		template<typename T, typename U>
		void EqualTemplateMethod()
		{
			Datum dat1, dat2, dat3;
			bool b;
			T t1, t2;
			U u;
			auto func1 = [&dat1, &dat2] { dat1 == dat2; };
			Assert::ExpectException<exception>(func1);

			dat1.PushBack(t1);
			auto func2 = [&dat1, &dat2] { dat1 == dat2; };
			Assert::ExpectException<exception>(func2);

			dat2.PushBack(u);
			b = dat1 == dat2;
			Assert::IsFalse(b);

			dat3.PushBack(t1);
			b = dat1 == dat3;
			Assert::IsTrue(b);

			dat1.PushBack(t2);
			b = dat1 == dat3;
			Assert::IsFalse(b);

			dat3.PushBack(t2);
			b = dat1 == dat3;
			Assert::IsTrue(b);
		}

		/************************************************************************/
		template<typename T, typename U>
		void NotEqualTemplateMethod()
		{
			Datum dat1, dat2, dat3;
			bool b;
			T t1, t2;
			U u;
			auto func1 = [&dat1, &dat2] { dat1 != dat2; };
			Assert::ExpectException<exception>(func1);

			dat1.PushBack(t1);
			auto func2 = [&dat1, &dat2] { dat1 != dat2; };
			Assert::ExpectException<exception>(func2);

			dat2.PushBack(u);
			b = dat1 != dat2;
			Assert::IsTrue(b);

			dat3.PushBack(t1);
			b = dat1 != dat3;
			Assert::IsFalse(b);

			dat1.PushBack(t2);
			b = dat1 != dat3;
			Assert::IsTrue(b);

			dat3.PushBack(t2);
			b = dat1 != dat3;
			Assert::IsFalse(b);
		}

		/************************************************************************/
		template<typename T>
		void TypeTemplateMethod(Datum::DatumType type1, Datum::DatumType type2)
		{
			Datum dat;
			Assert::IsTrue(dat.Type() == Datum::DatumType::Unknown);

			auto func1 = [&dat] { dat.SetType(Datum::DatumType::Unknown); };
			Assert::ExpectException<exception>(func1);

			dat.SetType(type1);
			Assert::IsTrue(dat.Type() == type1);

			auto func2 = [&dat, &type2] { dat.SetType(type2); };
			Assert::ExpectException<exception>(func2);
		}

		/************************************************************************/
		template<typename T>
		void SizeTemplateMethod()
		{
			Datum dat1, dat2;
			auto func1 = [&dat1] { dat1.SetSize(6u); };
			Assert::ExpectException<exception>(func1);

			dat1.PushBack(T());
			dat1.SetSize(4u);
			Assert::AreEqual(dat1.Size(), 4u);

			dat1.Get<T>(3u);

			dat1.SetSize(2u);
			Assert::AreEqual(dat1.Size(), 2u);

			auto func2 = [&dat1] { dat1.Get<T>(3u); };
			Assert::ExpectException<exception>(func2);

			T* ar = new T[6];
			dat2.SetStorage(ar, 6u);

			auto func3 = [&dat2] { dat2.SetSize(8u); };
			Assert::ExpectException<exception>(func3);

			delete[] ar;
		}

		/************************************************************************/
		template<typename T>
		void ClearAndPopBackTemplateMethod()
		{
			Datum dat1;
			dat1.PushBack(T());
			dat1.PushBack(T());
			dat1.PushBack(T());
			Assert::AreEqual(dat1.Size(), 3u);

			dat1.PopBack();
			Assert::AreEqual(dat1.Size(), 2u);
			
			dat1.Clear();
			Assert::IsTrue(dat1.IsEmpty());

			T* ar = new T[4];
			dat1.SetStorage(ar, 4);

			auto func1 = [&dat1] { dat1.PopBack(); };
			Assert::ExpectException<exception>(func1);
			
			auto func2 = [&dat1] { dat1.Clear(); };
			Assert::ExpectException<exception>(func2);

			delete[] ar;
		}

		/************************************************************************/
		template<typename T>
		void SetStorageTemplateMethod()
		{
			Datum dat1;
			dat1.PushBack(T());
			dat1.PushBack(T());
			dat1.PushBack(T());
			Assert::AreEqual(dat1.Size(), 3u);

			T* ar = new T[4];
			T t = ar[0];
			dat1.SetStorage(ar, 4);
			Assert::AreEqual(dat1.Size(), 4u);
			Assert::IsTrue(dat1.Get<T>(0u) == t);

			auto func1 = [&dat1] { dat1.PopBack(); };
			Assert::ExpectException<exception>(func1);

			delete[] ar;
		}

		/************************************************************************/
		template<typename T, typename U>
		void SetTemplateMethod(Datum::DatumType type)
		{
			Datum dat;
			dat.Set(T());
			Assert::IsTrue(dat.Type() == type);

			auto func1 = [&dat] { dat.Set(U()); };
			Assert::ExpectException<exception>(func1);
			T t1;
			dat.Set(t1);
			Assert::AreEqual(dat.Size(), 1u);

			T t2;
			dat.Set(t2, 1u);
			Assert::IsTrue(dat.Get<T>(1) == t2);

			auto func2 = [&dat] { dat.Set(T(), 6u); };
			Assert::ExpectException<exception>(func2);
		}

		/************************************************************************/
		template<typename T>
		void SetFromStringTemplateMethod(Datum::DatumType type ,string str, T& t, bool wrongFormat = false)
		{
			Datum dat;

			auto func1 = [&dat, &str] { dat.SetFromString(str); };
			Assert::ExpectException<exception>(func1);
			dat.SetType(type);

			if (wrongFormat)
			{
				auto func2 = [&dat, &str] { dat.SetFromString(str); };
				Assert::ExpectException<exception>(func2);
			}
			else
			{
				dat.SetFromString(str);
				Assert::AreEqual(dat.Size(), 1u);
				Assert::IsTrue(dat.Get<T>(0) == t);
			}
		}

		/************************************************************************/
		template<typename T>
		void ToStringTemplateMethod(T& t, string str1, string str2)
		{
			Datum dat;
			dat.PushBack(t);
			string s = dat.ToString(0);
			bool b = (s.find(str1) != string::npos);
			Assert::IsTrue(b);
			
			b = (s.find(str2) != string::npos);
			Assert::IsFalse(b);
		}

		/************************************************************************/
		template<typename T, typename U>
		void PushBackTemplateMethod(Datum::DatumType type)
		{
			Datum dat;
			dat.PushBack(T());
			Assert::IsTrue(dat.Type() == type);

			auto func1 = [&dat] { dat.PushBack(U()); };
			Assert::ExpectException<exception>(func1);
			T t1;
			dat.PushBack(t1);
			Assert::AreEqual(dat.Size(), 2u);
			Assert::AreEqual(dat.Capacity(), 2u);

			T t2;
			dat.PushBack(t2);
			Assert::IsTrue(dat.Get<T>(2) == t2);
			Assert::AreEqual(dat.Size(), 3u);
			Assert::AreEqual(dat.Capacity(), 4u);
		}

		/************************************************************************/
		template<typename T>
		void GetTemplateMethod()
		{
			Datum dat1;
			auto func1 = [&dat1] { dat1.Get<T>(0); };
			Assert::ExpectException<out_of_range>(func1);

			dat1.PushBack(T());
			T t;
			dat1.PushBack(t);
			Assert::IsTrue(dat1.Get<T>(1) == t);

			dat1.PushBack(T());
			auto func2 = [&dat1] { dat1.Get<T>(3); };
			Assert::ExpectException<out_of_range>(func2);

			const Datum dat2 = dat1;
			Assert::IsTrue(dat2.Get<T>(1) == t);
			auto func3 = [&dat2] { dat2.Get<T>(3); };
			Assert::ExpectException<out_of_range>(func3);
		}

		/************************************************************************/
		template<typename T>
		void CapacityTemplateMethod()
		{
			Datum dat;
			Assert::AreEqual(dat.Capacity(), 0u);
			dat.PushBack(T());
			Assert::AreEqual(dat.Capacity(), 1u);
			dat.PushBack(T());
			dat.PushBack(T());
			Assert::AreEqual(dat.Capacity(), 4u);
			dat.SetSize(6u);
			Assert::AreEqual(dat.Capacity(), 6u);
		}

		/************************************************************************/
		template<typename T>
		void IsEmptyTemplateMethod()
		{
			Datum dat;
			Assert::IsTrue(dat.IsEmpty());
			dat.PushBack(T());
			Assert::IsFalse(dat.IsEmpty());
			dat.Clear();
			Assert::IsTrue(dat.IsEmpty());
		}

		/************************************************************************/
		template<typename T>
		void HasExternalStorageTemplateMethod()
		{
			Datum dat;
			dat.PushBack(T());
			Assert::IsFalse(dat.HasExternalStorage());
			T* ar = new T[4];
			dat.SetStorage(ar, 4u);
			Assert::IsTrue(dat.HasExternalStorage());

			delete[] ar;
		}

#pragma endregion

#pragma region Test Methods

		/************************************************************************/
		TEST_METHOD(ConstructorsTestMethod)
		{
			int32_t i = 5;
			std::float_t f = 5.6f;
			vec4 v(6);
			mat4x4 m(9);
			string s("yo");

			ConstructorsTemplateMethod(i, Datum::DatumType::Integer);
			ConstructorsTemplateMethod(f, Datum::DatumType::Float);
			ConstructorsTemplateMethod(v, Datum::DatumType::Vector);
			ConstructorsTemplateMethod(m, Datum::DatumType::Matrix);
			ConstructorsTemplateMethod(s, Datum::DatumType::String);
		}

		/************************************************************************/
		TEST_METHOD(AssignmentTestMethod)
		{
			AssignmentTemplateMethod<int32_t>();
			AssignmentTemplateMethod<std::float_t>();
			AssignmentTemplateMethod<vec4>();
			AssignmentTemplateMethod<mat4x4>();
			AssignmentTemplateMethod<string>();
		}

		/************************************************************************/
		TEST_METHOD(ScalarAssignmentsTestMethod)
		{
			ScalarAssignmentTemplateMethod<int32_t, std::float_t>(Datum::DatumType::Integer);
			ScalarAssignmentTemplateMethod<std::float_t, int32_t>(Datum::DatumType::Float);
			ScalarAssignmentTemplateMethod<vec4, std::float_t>(Datum::DatumType::Vector);
			ScalarAssignmentTemplateMethod<mat4x4, std::float_t>(Datum::DatumType::Matrix);
			ScalarAssignmentTemplateMethod<string, std::float_t>(Datum::DatumType::String);
		}

		/************************************************************************/
		TEST_METHOD(EqualTestMethod)
		{
			EqualTemplateMethod<int32_t, std::float_t>();
			EqualTemplateMethod<std::float_t, int32_t>();
			EqualTemplateMethod<vec4, std::float_t>();
			EqualTemplateMethod<mat4x4, std::float_t>();
			EqualTemplateMethod<string, std::float_t>();
		}

		/************************************************************************/
		TEST_METHOD(NotEqualTestMethod)
		{
			NotEqualTemplateMethod <int32_t, std::float_t>();
			NotEqualTemplateMethod <std::float_t, int32_t>();
			NotEqualTemplateMethod <vec4, std::float_t>();
			NotEqualTemplateMethod <mat4x4, std::float_t>();
			NotEqualTemplateMethod <string, std::float_t>();
		}

		/************************************************************************/
		TEST_METHOD(TypeTestMethod)
		{
			TypeTemplateMethod <int32_t>(Datum::DatumType::Integer, Datum::DatumType::Float);
			TypeTemplateMethod <std::float_t>(Datum::DatumType::Float, Datum::DatumType::Integer);
			TypeTemplateMethod <vec4>(Datum::DatumType::Vector, Datum::DatumType::Float);
			TypeTemplateMethod <mat4x4>(Datum::DatumType::Matrix, Datum::DatumType::Float);
			TypeTemplateMethod <string>(Datum::DatumType::String, Datum::DatumType::Float);
		}

		/************************************************************************/
		TEST_METHOD(SizeTestMethod)
		{
			SizeTemplateMethod<int32_t>();
			SizeTemplateMethod<std::float_t>();
			SizeTemplateMethod<vec4>();
			SizeTemplateMethod<mat4x4>();
			SizeTemplateMethod<string>();
		}

		/************************************************************************/
		TEST_METHOD(ClearAndPopBackTestMethod)
		{
			ClearAndPopBackTemplateMethod<int32_t>();
			ClearAndPopBackTemplateMethod<std::float_t>();
			ClearAndPopBackTemplateMethod<vec4>();
			ClearAndPopBackTemplateMethod<mat4x4>();
			ClearAndPopBackTemplateMethod<string>();
		}

		/************************************************************************/
		TEST_METHOD(SetStorageTestMethod)
		{
			SetStorageTemplateMethod<int32_t>();
			SetStorageTemplateMethod<std::float_t>();
			SetStorageTemplateMethod<vec4>();
			SetStorageTemplateMethod<mat4x4>();
			SetStorageTemplateMethod<string>();
		}

		/************************************************************************/
		TEST_METHOD(SetTestMethod)
		{
			SetTemplateMethod<int32_t, std::float_t>(Datum::DatumType::Integer);
			SetTemplateMethod<std::float_t, int32_t>(Datum::DatumType::Float);
			SetTemplateMethod<vec4, std::float_t>(Datum::DatumType::Vector);
			SetTemplateMethod<mat4x4, std::float_t>(Datum::DatumType::Matrix);
			SetTemplateMethod<string, std::float_t>(Datum::DatumType::String);
		}

		/************************************************************************/
		TEST_METHOD(SetFromStringTestMethod)
		{
			int32_t i = 5;
			std::float_t f = 5.6f;
			vec4 v1(6);
			vec4 v2(6,5,4,3);
			mat4x4 m1(4);
			mat4x4 m2(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16);
			string str("yo");

			SetFromStringTemplateMethod<int32_t>(Datum::DatumType::Integer, "5", i);
			SetFromStringTemplateMethod<int32_t>(Datum::DatumType::Integer, "df5", i, true);
			SetFromStringTemplateMethod<std::float_t>(Datum::DatumType::Float, "5.6", f);
			SetFromStringTemplateMethod<std::float_t>(Datum::DatumType::Float, "adf5", f, true);
			SetFromStringTemplateMethod<vec4>(Datum::DatumType::Vector, "(6)", v1);
			SetFromStringTemplateMethod<vec4>(Datum::DatumType::Vector, "(6,5,4,3)", v2);
			SetFromStringTemplateMethod<vec4>(Datum::DatumType::Vector, "8d", v1, true);
			SetFromStringTemplateMethod<mat4x4>(Datum::DatumType::Matrix, "(4)", m1);
			SetFromStringTemplateMethod<mat4x4>(Datum::DatumType::Matrix, "(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16)", m2);
			SetFromStringTemplateMethod<mat4x4>(Datum::DatumType::Matrix, "sd", m1, true);
			SetFromStringTemplateMethod<string>(Datum::DatumType::String, "yo", str);
		}

		/************************************************************************/
		TEST_METHOD(ToStringTestMethod)
		{
			int32_t i = 5;
			std::float_t f = 5.6f;
			vec4 v1(6);
			vec4 v2(6, 5, 4, 3);
			mat4x4 m1(4);
			mat4x4 m2(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
			string str("yo");

			ToStringTemplateMethod<int32_t>(i, "5", "8");
			ToStringTemplateMethod<std::float_t>(f, "5.6", "9.5");
			ToStringTemplateMethod<vec4>(v1, "6", "9");
			ToStringTemplateMethod<vec4>(v2, "3", "65");
			ToStringTemplateMethod<mat4x4>(m1, "4", "6");
			ToStringTemplateMethod<mat4x4>(m2, "16", "17");
			ToStringTemplateMethod<string>(str, "yo", "rdf");
		}

		/************************************************************************/
		TEST_METHOD(PushBackTestMethod)
		{
			PushBackTemplateMethod<int32_t, std::float_t>(Datum::DatumType::Integer);
			PushBackTemplateMethod<std::float_t, int32_t>(Datum::DatumType::Float);
			PushBackTemplateMethod<vec4, std::float_t>(Datum::DatumType::Vector);
			PushBackTemplateMethod<mat4x4, std::float_t>(Datum::DatumType::Matrix);
			PushBackTemplateMethod<string, std::float_t>(Datum::DatumType::String);
		}

		/************************************************************************/
		TEST_METHOD(GetTestMethod)
		{
			GetTemplateMethod<int32_t>();
			GetTemplateMethod<std::float_t>();
			GetTemplateMethod<vec4>();
			GetTemplateMethod<mat4x4>();
			GetTemplateMethod<string>();
		}

		/************************************************************************/
		TEST_METHOD(CapacityTestMethod)
		{
			CapacityTemplateMethod<int32_t>();
			CapacityTemplateMethod<std::float_t>();
			CapacityTemplateMethod<vec4>();
			CapacityTemplateMethod<mat4x4>();
			CapacityTemplateMethod<string>();
		}

		/************************************************************************/
		TEST_METHOD(IsEmptyTestMethod)
		{
			IsEmptyTemplateMethod<int32_t>();
			IsEmptyTemplateMethod<std::float_t>();
			IsEmptyTemplateMethod<vec4>();
			IsEmptyTemplateMethod<mat4x4>();
			IsEmptyTemplateMethod<string>();
		}

		/************************************************************************/
		TEST_METHOD(HasExternalStorageTestMethod)
		{
			HasExternalStorageTemplateMethod<int32_t>();
			HasExternalStorageTemplateMethod<std::float_t>();
			HasExternalStorageTemplateMethod<vec4>();
			HasExternalStorageTemplateMethod<mat4x4>();
			HasExternalStorageTemplateMethod<string>();
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState DatumTest::sStartMemState;

}