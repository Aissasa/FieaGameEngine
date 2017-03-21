#include "pch.h"
#include "CppUnitTest.h"
#include "TestXmlParseHelper.h"
#include "TestSharedData.h"
#include "XmlParseMaster.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Library;

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(XmlParseTest)
	{
	public:

		typedef XmlParseMaster::SharedData SharedDataC;

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

#pragma region Test Methods

#pragma region XmlParseMaster

		/************************************************************************/
		TEST_METHOD(XmlParseMasterCtorTest)
		{
			TestSharedData* sharedData = new TestSharedData();
			XmlParseMaster master(sharedData);
			Assert::IsTrue(sharedData == master.GetSharedData());
			
			TestSharedData* sharedData2 = new TestSharedData();
			master.SetSharedData(sharedData2);
			Assert::IsTrue(sharedData2 == master.GetSharedData());

			TestXmlParseHelper* helper = new TestXmlParseHelper();
			master.AddHelper(*helper);

			delete sharedData;
			delete sharedData2;
			delete helper;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseMasterCloneTest)
		{


			TestSharedData* sharedData = new TestSharedData();
			XmlParseMaster master(sharedData);

			TestXmlParseHelper* helper = new TestXmlParseHelper();
			master.AddHelper(*helper);

			XmlParseMaster* master2 = master.Clone();
			Assert::IsTrue(sharedData != master2->GetSharedData());

			XmlParseMaster master3;
			XmlParseMaster* master4 = master3.Clone();
			Assert::IsNull(master4->GetSharedData());

			delete master2;
			delete master4;
			delete sharedData;
			delete helper;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseMasterAddAndRemoveHelpersTest)
		{
			TestSharedData* sharedData = new TestSharedData();
			XmlParseMaster master(sharedData);

			TestXmlParseHelper* helper1 = new TestXmlParseHelper();
			TestXmlParseHelper* helper2 = new TestXmlParseHelper();
			master.AddHelper(*helper1);

			XmlParseMaster* master2 = master.Clone();
			
			auto func1 = [&master2, &helper2] { master2->AddHelper(*helper2); };
			Assert::ExpectException<exception>(func1);

			auto func2 = [&master2, &helper1] { master2->RemoveHelper(*helper1); };
			Assert::ExpectException<exception>(func2);
			
			master.RemoveHelper(*helper1);

			delete master2;
			delete sharedData;
			delete helper1;
			delete helper2;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseMasterParseTest)
		{
			TestSharedData* sharedData = new TestSharedData();
			XmlParseMaster master(sharedData);

			auto func1 = [&master] { master.Parse("<Weapon Damage=\"5\" />", 22); };
			Assert::ExpectException<exception>(func1);

			TestXmlParseHelper* helper = new TestXmlParseHelper();
			master.AddHelper(*helper);
			
			master.Parse("<Untreated Health=\"5\" />", 22);
			Assert::IsTrue(sharedData->mDamage == 0);
			Assert::IsTrue(sharedData->GetMaxDepth() == 0);
			Assert::IsTrue(helper->mStartElementHandlerCount == 0);
			Assert::IsTrue(helper->mEndElementHandlerCount == 0);
			Assert::IsTrue(helper->mCharDataHandlerCount == 0);

			master.Parse("<Weapon Damage=\"5\" />", 22);
			Assert::IsTrue(sharedData->mDamage == 5);
			Assert::IsTrue(sharedData->GetMaxDepth() == 1);
			Assert::IsTrue(helper->mStartElementHandlerCount == 1);
			Assert::IsTrue(helper->mEndElementHandlerCount == 1);
			Assert::IsTrue(helper->mCharDataHandlerCount == 0);

			master.Parse("<Weapon><Damage><Value>9</Value></Damage></Weapon>", 51);
			Assert::IsTrue(sharedData->mDamage == 9);
			Assert::IsTrue(sharedData->GetMaxDepth() == 3);
			Assert::IsTrue(helper->mStartElementHandlerCount == 3);
			Assert::IsTrue(helper->mEndElementHandlerCount == 3);
			Assert::IsTrue(helper->mCharDataHandlerCount == 1);

			master.Parse("<Weapon><Damage Value=\"23\"></Damage></Weapon>", 46);
			Assert::IsTrue(sharedData->mDamage == 23);
			Assert::IsTrue(sharedData->GetMaxDepth() == 2);
			Assert::IsTrue(helper->mStartElementHandlerCount == 2);
			Assert::IsTrue(helper->mEndElementHandlerCount == 2);
			Assert::IsTrue(helper->mCharDataHandlerCount == 0);

			delete sharedData;
			delete helper;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseMasterParseFromFileTest)
		{
			TestSharedData* sharedData = new TestSharedData();
			XmlParseMaster master(sharedData);

			TestXmlParseHelper* helper = new TestXmlParseHelper();
			master.AddHelper(*helper);

			ofstream file;
			string fileName("ParseTestFile.xml");
			file.open(fileName);
			file << "<Weapon Damage=\"7\" />";
			file.close();

			master.ParseFromFile(fileName);
			Assert::IsTrue(sharedData->mDamage == 7);
			Assert::IsTrue(sharedData->GetMaxDepth() == 1);
			Assert::IsTrue(helper->mStartElementHandlerCount == 1);
			Assert::IsTrue(helper->mEndElementHandlerCount == 1);
			Assert::IsTrue(helper->mCharDataHandlerCount == 0);


			file.open(fileName);
			file << "<Weapon><Damage><Value>0</Value></Damage></Weapon>";
			file.close();

			master.ParseFromFile(fileName);
			Assert::IsTrue(sharedData->mDamage == 0);
			Assert::IsTrue(sharedData->GetMaxDepth() == 3);
			Assert::IsTrue(helper->mStartElementHandlerCount == 3);
			Assert::IsTrue(helper->mEndElementHandlerCount == 3);
			Assert::IsTrue(helper->mCharDataHandlerCount == 1);


			file.open(fileName);
			file << "<Weapon><Damage Value=\"98\"></Damage></Weapon>";
			file.close();

			master.ParseFromFile(fileName);
			Assert::IsTrue(sharedData->mDamage == 98);
			Assert::IsTrue(sharedData->GetMaxDepth() == 2);
			Assert::IsTrue(helper->mStartElementHandlerCount == 2);
			Assert::IsTrue(helper->mEndElementHandlerCount == 2);
			Assert::IsTrue(helper->mCharDataHandlerCount == 0);

			Assert::IsTrue(master.GetFileName() == fileName);

			delete sharedData;
			delete helper;

			remove("ParseTestFile.xml");
		}

#pragma endregion

#pragma region SharedData

		/************************************************************************/
		TEST_METHOD(SharedDataTest)
		{
			TestSharedData sharedData;
			Assert::IsTrue(sharedData.Depth() == 0);
			Assert::IsTrue(sharedData.GetMaxDepth() == 0);
			Assert::IsTrue(sharedData.GetXmlParseMaster() == nullptr);

			sharedData.IncrementDepth();
			Assert::IsTrue(sharedData.Depth() == 1);
			Assert::IsTrue(sharedData.GetMaxDepth() == 0);
			sharedData.UpdateMaxDepth();
			Assert::IsTrue(sharedData.GetMaxDepth() == 1);
			
			SharedDataC* clone = sharedData.Clone();
			Assert::IsTrue(clone->As<TestSharedData>()->GetMaxDepth() == 1);
			Assert::IsTrue(clone->Depth() == 1);

			clone->DecrementDepth();
			clone->DecrementDepth();
			clone->As<TestSharedData>()->UpdateMaxDepth();
			Assert::IsTrue(clone->As<TestSharedData>()->GetMaxDepth() == 1);
			Assert::IsTrue(clone->Depth() == 0);
			Assert::IsTrue(sharedData.Depth() == 1);

			XmlParseMaster master;
			TestSharedData sharedData2(&master);
			Assert::IsTrue(sharedData2.GetXmlParseMaster() == &master);
			SharedDataC* clone2 = sharedData2.Clone();
			Assert::IsTrue(clone2->As<TestSharedData>()->GetXmlParseMaster() == &master);
			
			sharedData2.SetXmlParseMaster(nullptr);
			Assert::IsTrue(clone2->As<TestSharedData>()->GetXmlParseMaster() != sharedData2.GetXmlParseMaster());

			delete clone;
			delete clone2;
		}


#pragma endregion


#pragma endregion


	private:
		static _CrtMemState sStartMemState;


	};

	_CrtMemState XmlParseTest::sStartMemState;
}