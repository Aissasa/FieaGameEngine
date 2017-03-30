#include "pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "AbstractProductTest.h"
#include "Product1.h"
#include "Product2.h"
#include "ConcreteFactoriesTest.h"
#include "TableSharedData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(FactoryTest)
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
		template <typename ConcreteFactory, typename Product>
		void ProductRTTITestTemplate(string className)
		{
			ConcreteFactory factory;
			RTTI* prod = factory.Create();

			Assert::IsNotNull(prod->As<Product>());
			Assert::IsNotNull(prod->As<AbstractProductTest>());
			Assert::IsNull(prod->As<TableSharedData>());

			Assert::IsTrue(prod->Is(className));
			Assert::IsTrue(prod->Is(prod->TypeIdInstance()));
			Assert::IsTrue(prod->Is(prod->As<AbstractProductTest>()->TypeIdClass()));
			Assert::IsTrue(prod->As<Product>()->TypeName() == className);

			Assert::IsTrue(prod->QueryInterface(prod->TypeIdInstance())->ToString() == "RTTI");
			Assert::IsNotNull(prod->As<Product>());

			Assert::IsTrue(prod->As<Product>()->Equals(prod));

			delete prod;
		}

#pragma endregion

#pragma region Test Methods

		/************************************************************************/
		TEST_METHOD(FactoriesTest)
		{
			// constructor
			Product1Factory factory1;
			Product2Factory factory2;
			BarFactory barFactory;

			// begin and end
			Assert::IsTrue(Factory<AbstractProductTest>::begin().operator*().second == &factory1);
			Assert::IsTrue(Factory<AbstractProductTest>::begin().operator++().operator*().second == &factory2);
			Assert::IsTrue(Factory<AbstractProductTest>::begin().operator++().operator++() == Factory<AbstractProductTest>::end());
			Assert::IsTrue(Factory<Bar>::begin().operator*().second == &barFactory);
			Assert::IsTrue(Factory<Bar>::begin().operator++() == Factory<Bar>::end());

			// classname
			Assert::IsTrue(factory1.ClassName() == "Product1");
			Assert::IsTrue(factory2.ClassName() == "Product2");
			Assert::IsTrue(barFactory.ClassName() == "Bar");

			// find
			Assert::IsTrue(Factory<AbstractProductTest>::Find("Product1") == &factory1);
			Assert::IsTrue(Factory<AbstractProductTest>::Find("Product2") == &factory2);
			Assert::IsTrue(Factory<Bar>::Find("Bar") == &barFactory);
			Assert::IsNull(Factory<AbstractProductTest>::Find("Product3"));
			Assert::IsNull(Factory<Bar>::Find("Product3"));

			// creates
			AbstractProductTest* product11 = factory1.Create();
			AbstractProductTest* product12 = Factory<AbstractProductTest>::Create("Product1");
			AbstractProductTest* product21 = factory2.Create();
			AbstractProductTest* product22 = Factory<AbstractProductTest>::Create("Product2");
			Bar* bar1 = barFactory.Create();
			Bar* bar2 = Factory<Bar>::Create("Bar");

			Assert::IsTrue(product11->Is(Product1::TypeName()));
			Assert::IsTrue(product12->Is(Product1::TypeName()));
			Assert::IsTrue(product21->Is(Product2::TypeName()));
			Assert::IsTrue(product22->Is(Product2::TypeName()));
			Assert::IsNull(Factory<AbstractProductTest>::Create("Product3"));

			delete bar1;
			delete bar2;
			delete product11;
			delete product12;
			delete product21;
			delete product22;
		}

		/************************************************************************/
		TEST_METHOD(ProductsTest)
		{
			Product1Factory factory1;
			Product2Factory factory2;
			BarFactory barFactory;

			AbstractProductTest* product1 = factory1.Create();
			AbstractProductTest* product2 = Factory<AbstractProductTest>::Create("Product2");
			Bar* bar = barFactory.Create();

			product1->As<Product1>()->SetNumber(2);
			product2->As<Product2>()->SetNumber(2);
			bar->SetFloat(2.5f);

			Assert::AreEqual(product1->GetNumber(), 2U);
			Assert::AreEqual(product2->GetNumber(), 4U);
			Assert::AreEqual(bar->GetFloat(), 2.5f);

			delete product1;
			delete product2;
			delete bar;
		}

		/************************************************************************/
		TEST_METHOD(ProductRTTITest)
		{
			ProductRTTITestTemplate<Product1Factory, Product1>("Product1");
			ProductRTTITestTemplate<Product2Factory, Product2>("Product2");
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState FactoryTest::sStartMemState;
}