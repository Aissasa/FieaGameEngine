#include "pch.h"
#include "CppUnitTest.h"
#include "XmlParseMaster.h"
#include "XmlParseHelperTable.h"
#include "XmlParseHelperNumber.h"
#include "XmlParseHelperString.h"
#include "XmlParseHelperVector.h"
#include "XmlParseHelperMatrix.h"
#include "TableSharedData.h"
#include "TestSharedData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace glm;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeXmlParseTest)
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
		template <typename Helper>
		void HelperRTTITestTemplate(string className)
		{
			RTTI* helper = new Helper();

			Assert::IsNotNull(helper->As<Helper>());
			Assert::IsNotNull(helper->As<IXmlParseHelper>());
			Assert::IsNull(helper->As<TableSharedData>());

			Assert::IsTrue(helper->Is(className));
			Assert::IsTrue(helper->Is(helper->TypeIdInstance()));
			Assert::IsTrue(helper->Is(helper->As<IXmlParseHelper>()->TypeIdClass()));
			Assert::IsTrue(helper->As<Helper>()->TypeName() == className);

			Assert::IsTrue(helper->QueryInterface(helper->TypeIdInstance())->ToString() == "RTTI");
			Assert::IsNotNull(helper->As<Helper>());

			Assert::IsTrue(helper->As<Helper>()->Equals(helper));

			delete helper;
		}

#pragma endregion

#pragma region Test Methods

		/************************************************************************/
		TEST_METHOD(XmlParseHelperTableTest)
		{
			TestSharedData* testSharedData = new TestSharedData();
			XmlParseMaster master1(testSharedData);

			XmlParseHelperTable* scopeHelper = new XmlParseHelperTable();
			master1.AddHelper(*scopeHelper);

			string xmlToParse("<Scope Name=\"Lvl1\"/>");
			master1.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(testSharedData->GetMaxDepth() == 0);
			Assert::IsTrue(scopeHelper->mStartElementHandlerCount == 0);
			Assert::IsTrue(scopeHelper->mEndElementHandlerCount == 0);

			TableSharedData* sharedData = new TableSharedData();
			XmlParseMaster master(sharedData);
			master.AddHelper(*scopeHelper);

			xmlToParse = "<Scope Test=\"Yo\"/>";

			auto func1 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func1);

			xmlToParse = "<Scope Test=\"Yo\" Name=\"Lvl1\">bla</Scope>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(&sharedData->GetScope());
			Assert::IsNotNull(sharedData->GetScope().Find("Name"));
			Assert::IsTrue(sharedData->GetScope().Find("Name")->Get<string>(0) == "Lvl1");

			XmlParseHelperNumber* numberHelper = new XmlParseHelperNumber();
			master.AddHelper(*numberHelper);

			xmlToParse = "<Scope Name=\"Lvl1\"><Integer Name=\"Integer1\" Value=\"2\"/></Scope>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(sharedData->GetScope().Find("Integer1"));
			Assert::IsTrue(sharedData->GetScope().Find("Integer1")->Get<int32_t>(0) == 2);


			XmlParseHelperString* stringHelper = new XmlParseHelperString();
			master.AddHelper(*stringHelper);

			xmlToParse = "<Scope Name=\"Lvl1\"><String Name=\"String1\" Value=\"Yo\"/></Scope>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(sharedData->GetScope().Find("String1"));
			Assert::IsTrue(sharedData->GetScope().Find("String1")->Get<string>(0) == "Yo");


			xmlToParse = "<Scope Name=\"Lvl1\"><Scope Name=\"Lvl2\"><Integer Name=\"Integer1\" Value=\"2\"/></Scope><String Name=\"String1\" Value=\"Yo\"/></Scope>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));

			Assert::IsNotNull(sharedData->GetScope().Find("Name"));
			Assert::IsTrue(sharedData->GetScope().Find("Name")->Get<string>(0) == "Lvl1");
			Assert::IsNotNull(sharedData->GetScope().Find("String1"));
			Assert::IsTrue(sharedData->GetScope().Find("String1")->Get<string>(0) == "Yo");

			Scope* childScope = sharedData->GetScope().Find("Lvl2")->Get<Scope*>(0);
			Assert::IsNotNull(childScope);
			Assert::IsNotNull(childScope->Find("Name"));
			Assert::IsTrue(childScope->Find("Name")->Get<string>(0) == "Lvl2");
			Assert::IsNotNull(childScope->Find("Integer1"));
			Assert::IsTrue(childScope->Find("Integer1")->Get<int32_t>(0) == 2);

			XmlParseMaster* cloneMaster = master.Clone();
			xmlToParse = "<Scope Name=\"Lvl1\"><Integer Name=\"Integer1\" Value=\"2\"/></Scope>";
			cloneMaster->Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("Integer1"));
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("Integer1")->Get<int32_t>(0) == 2);


			delete cloneMaster;
			delete scopeHelper;
			delete numberHelper;
			delete stringHelper;
			delete sharedData;
			delete testSharedData;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseHelperNumberTest)
		{
			TestSharedData* testSharedData = new TestSharedData();
			XmlParseMaster master1(testSharedData);

			XmlParseHelperTable* scopeHelper = new XmlParseHelperTable();
			XmlParseHelperNumber* numberHelper = new XmlParseHelperNumber();
			master1.AddHelper(*scopeHelper);
			master1.AddHelper(*numberHelper);

			string xmlToParse("<Scope Name=\"Lvl1\"><Integer Name=\"Integer1\" Value=\"2\"/></Scope>");
			master1.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(testSharedData->GetMaxDepth() == 0);
			Assert::IsTrue(numberHelper->mStartElementHandlerCount == 0);

			TableSharedData* sharedData = new TableSharedData();
			XmlParseMaster master(sharedData);
			master.AddHelper(*scopeHelper);
			master.AddHelper(*numberHelper);
			XmlParseHelperString* stringHelper = new XmlParseHelperString();
			master.AddHelper(*stringHelper);

			xmlToParse = "<Scope Name=\"Lvl1\"><Integer NotName=\"Integer1\" Value=\"2\"/></Scope>";
			auto func1 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func1);

			xmlToParse = "<Scope Name=\"Lvl1\"><Integer Name=\"Integer1\" Value=\"2\"><Float Name=\"Float1\" Value=\"2.5f\"/></Integer></Scope>";
			auto func2 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func2);

			xmlToParse = "<Scope Name=\"Lvl1\"><Integer Name=\"Integer1\" Value=\"2\"><String Name=\"Str1\" Value=\"2.5f\"/></Integer></Scope>";
			auto func3 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func3);

			xmlToParse = "<Scope Name=\"Lvl1\"> <Integer Name=\"Integer1\" Value=\"2\"/> <Integer Name=\"Integer1\">5</Integer> <Float Name=\"Float1\" Value=\"2.5f\"/> </Scope>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(sharedData->GetScope().Find("Integer1"));
			Assert::IsTrue(sharedData->GetScope().Find("Integer1")->Get<int32_t>(0) == 2);
			Assert::IsTrue(sharedData->GetScope().Find("Integer1")->Get<int32_t>(1) == 5);
			Assert::IsTrue(sharedData->GetScope().Find("Float1")->Get<std::float_t>(0) == 2.5f);

			XmlParseMaster* cloneMaster = master.Clone();
			xmlToParse = "<Scope Name=\"Lvl1\"> <Integer Name=\"Integer1\" Value=\"2\"/> <Integer Name=\"Integer1\">5</Integer> <Float Name=\"Float1\" Value=\"2.5f\"/> </Scope>";
			cloneMaster->Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("Integer1"));
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("Integer1")->Get<int32_t>(0) == 2);
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("Integer1")->Get<int32_t>(1) == 5);
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("Float1")->Get<std::float_t>(0) == 2.5f);

			delete cloneMaster;
			delete scopeHelper;
			delete numberHelper;
			delete stringHelper;
			delete sharedData;
			delete testSharedData;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseHelperStringTest)
		{
			TestSharedData* testSharedData = new TestSharedData();
			XmlParseMaster master1(testSharedData);

			XmlParseHelperTable* scopeHelper = new XmlParseHelperTable();
			master1.AddHelper(*scopeHelper);
			XmlParseHelperString* stringHelper = new XmlParseHelperString();
			master1.AddHelper(*stringHelper);

			string xmlToParse("<Scope Name=\"Lvl1\"><String Name=\"String1\" Value=\"2\"/></Scope>");
			master1.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(testSharedData->GetMaxDepth() == 0);
			Assert::IsTrue(stringHelper->mStartElementHandlerCount == 0);

			TableSharedData* sharedData = new TableSharedData();
			XmlParseMaster master(sharedData);
			master.AddHelper(*scopeHelper);
			master.AddHelper(*stringHelper);
			XmlParseHelperNumber* numberHelper = new XmlParseHelperNumber();
			master.AddHelper(*numberHelper);

			xmlToParse = "<Scope Name=\"Lvl1\"><String NotName=\"String1\" Value=\"ha\"/></Scope>";
			auto func1 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func1);

			xmlToParse = "<Scope Name=\"Lvl1\"><String Name=\"String1\" Value=\"ha\"><String Name=\"String2\" Value=\"haha\"/></String></Scope>";
			auto func2 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func2);

			xmlToParse = "<Scope Name=\"Lvl1\"><String Name=\"String1\" Value=\"ha\"><Integer Name=\"Integer1\" Value=\"2\"/></String></Scope>";
			auto func3 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func3);

			xmlToParse = "<Scope Name=\"Lvl1\"> <String Name=\"String1\" Value=\"ha\"/> <String Name=\"String1\">yep</String> <String Name=\"String2\" Value=\"hahaha\"/> </Scope>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(sharedData->GetScope().Find("String1"));
			Assert::IsTrue(sharedData->GetScope().Find("String1")->Get<string>(0) == "ha");
			Assert::IsTrue(sharedData->GetScope().Find("String1")->Get<string>(1) == "yep");
			Assert::IsTrue(sharedData->GetScope().Find("String2")->Get<string>(0) == "hahaha");


			XmlParseMaster* cloneMaster = master.Clone();
			xmlToParse = "<Scope Name=\"Lvl1\"> <String Name=\"String1\" Value=\"ha\"/> <String Name=\"String1\">yep</String> <String Name=\"String2\" Value=\"hahaha\"/> </Scope>";
			cloneMaster->Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("String1"));
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("String1")->Get<string>(0) == "ha");
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("String1")->Get<string>(1) == "yep");
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("String2")->Get<string>(0) == "hahaha");

			delete cloneMaster;
			delete scopeHelper;
			delete numberHelper;
			delete stringHelper;
			delete sharedData;
			delete testSharedData;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseHelperVectorTest)
		{
			TestSharedData* testSharedData = new TestSharedData();
			XmlParseMaster master1(testSharedData);

			XmlParseHelperTable* scopeHelper = new XmlParseHelperTable();
			master1.AddHelper(*scopeHelper);
			XmlParseHelperVector* vectorHelper = new XmlParseHelperVector();
			master1.AddHelper(*vectorHelper);

			string xmlToParse("<Scope Name=\"Lvl1\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/></Scope>");
			master1.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(testSharedData->GetMaxDepth() == 0);
			Assert::IsTrue(vectorHelper->mStartElementHandlerCount == 0);

			TableSharedData* sharedData = new TableSharedData();
			XmlParseMaster master(sharedData);
			master.AddHelper(*scopeHelper);
			master.AddHelper(*vectorHelper);
			XmlParseHelperNumber* numberHelper = new XmlParseHelperNumber();
			master.AddHelper(*numberHelper);

			xmlToParse = "<Scope Name=\"Lvl1\"><Vector NotName=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\" /> </Scope>";
			auto func1 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func1);

			xmlToParse = "<Scope Name=\"Lvl1\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"><Vector Name=\"Vector2\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/></Vector></Scope>";
			auto func2 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func2);

			xmlToParse = "<Scope Name=\"Lvl1\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"><Integer Name=\"Integer1\" Value=\"2\"/></Vector></Scope>";
			auto func3 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func3);

			xmlToParse = "<Scope Name=\"Lvl1\"><Vector Name=\"Vector1\" x=\"2\" z=\"0\" w=\"0.3f\" /> </Scope>";
			auto func4 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func4);

			xmlToParse = "<Scope Name=\"Lvl1\"><Integer Name=\"Int1\" Value=\"3\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\" /> </Scope>";
			auto func5 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func5);


			xmlToParse = "<Scope Name=\"Lvl1\"> <Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/> <Vector Name=\"Vector2\" x=\"5\" y=\"9.6f\" z=\"8\" w=\"7.3f\"/> <Vector Name=\"Vector1\" x=\"7\" y=\"9.8f\" z=\"4\" w=\"1.6f\"/> </Scope>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(sharedData->GetScope().Find("Vector1")->Get<vec4>(0) == vec4(2, 5.6f, 0, 0.3f));
			Assert::IsTrue(sharedData->GetScope().Find("Vector1")->Get<vec4>(1) == vec4(7, 9.8f, 4, 1.6f));
			Assert::IsTrue(sharedData->GetScope().Find("Vector2")->Get<vec4>(0) == vec4(5, 9.6f, 8, 7.3f));

			XmlParseMaster* cloneMaster = master.Clone();
			xmlToParse = "<Scope Name=\"Lvl1\"> <Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/> <Vector Name=\"Vector2\" x=\"5\" y=\"9.6f\" z=\"8\" w=\"7.3f\"/> <Vector Name=\"Vector1\" x=\"7\" y=\"9.8f\" z=\"4\" w=\"1.6f\"/> </Scope>";
			cloneMaster->Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("Vector1")->Get<vec4>(0) == vec4(2, 5.6f, 0, 0.3f));
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("Vector1")->Get<vec4>(1) == vec4(7, 9.8f, 4, 1.6f));
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("Vector2")->Get<vec4>(0) == vec4(5, 9.6f, 8, 7.3f));

			delete cloneMaster;
			delete scopeHelper;
			delete numberHelper;
			delete vectorHelper;
			delete sharedData;
			delete testSharedData;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseHelperMatrixTest)
		{
			TestSharedData* testSharedData = new TestSharedData();
			XmlParseMaster master1(testSharedData);

			XmlParseHelperTable* scopeHelper = new XmlParseHelperTable();
			master1.AddHelper(*scopeHelper);
			XmlParseHelperMatrix* matrixHelper = new XmlParseHelperMatrix();
			master1.AddHelper(*matrixHelper);

			string xmlToParse("<Scope Name=\"Lvl1\"> <Matrix Name=\"Matrix1\"> <Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/> <Vector Name=\"Vector2\" x=\"5\" y=\"9.6f\" z=\"8\" w=\"7.3f\"/> <Vector Name=\"Vector3\" x=\"7\" y=\"9.8f\" z=\"4\" w=\"1.6f\"/> <Vector Name=\"Vector4\" x=\"9\" y=\"5.8f\" z=\"4\" w=\"1.6f\"/> </Matrix> </Scope>");
			master1.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(testSharedData->GetMaxDepth() == 0);
			Assert::IsTrue(matrixHelper->mStartElementHandlerCount == 0);

			TableSharedData* sharedData = new TableSharedData();
			XmlParseMaster master(sharedData);
			master.AddHelper(*scopeHelper);
			master.AddHelper(*matrixHelper);
			XmlParseHelperVector* vectorHelper = new XmlParseHelperVector();
			master.AddHelper(*vectorHelper);
			XmlParseHelperNumber* numberHelper = new XmlParseHelperNumber();
			master.AddHelper(*numberHelper);

			xmlToParse = "<Scope Name=\"Lvl1\"> <Matrix NotName=\"Matrix1\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/> <Vector Name=\"Vector2\" x=\"5\" y=\"9.6f\" z=\"8\" w=\"7.3f\"/> <Vector Name=\"Vector3\" x=\"7\" y=\"9.8f\" z=\"4\" w=\"1.6f\"/> <Vector Name=\"Vector4\" x=\"9\" y=\"5.8f\" z=\"4\" w=\"1.6f\"/>  </Matrix> </Scope>";
			auto func1 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func1);

			xmlToParse = "<Scope Name=\"Lvl1\"><Matrix Name=\"Matrix1\"> <Matrix Name=\"Matrix2\"></Matrix> </Matrix> </Scope>";
			auto func2 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func2);

			xmlToParse = "<Scope Name=\"Lvl1\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"><Matrix Name=\"Matrix1\"/></Vector></Scope>";
			auto func3 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func3);

			xmlToParse = "<Scope Name=\"Lvl1\"> <Matrix Name=\"Matrix1\"> <Vector Name=\"Vector2\" x=\"5\" y=\"9.6f\" z=\"8\" w=\"7.3f\"/> <Vector Name=\"Vector3\" x=\"7\" y=\"9.8f\" z=\"4\" w=\"1.6f\"/> <Vector Name=\"Vector4\" x=\"9\" y=\"5.8f\" z=\"4\" w=\"1.6f\"/>  </Matrix> </Scope>";
			auto func4 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func4);

			xmlToParse = "<Scope Name=\"Lvl1\"> <Matrix Name=\"Matrix1\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/> <Vector Name=\"Vector2\" x=\"5\" y=\"9.6f\" z=\"8\" w=\"7.3f\"/> <Vector Name=\"Vector3\" x=\"7\" y=\"9.8f\" z=\"4\" w=\"1.6f\"/> <Vector Name=\"Vector4\" x=\"9\" y=\"5.8f\" z=\"4\" w=\"1.6f\"/>  </Matrix> <Matrix Name=\"Matrix1\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/> <Vector Name=\"Vector2\" x=\"5\" y=\"9.6f\" z=\"8\" w=\"7.3f\"/> <Vector Name=\"Vector3\" x=\"7\" y=\"9.8f\" z=\"4\" w=\"1.6f\"/> <Vector Name=\"Vector4\" x=\"9\" y=\"5.8f\" z=\"4\" w=\"0.6f\"/>  </Matrix> <Matrix Name=\"Matrix2\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/> <Vector Name=\"Vector2\" x=\"5\" y=\"9.6f\" z=\"8\" w=\"7.3f\"/> <Vector Name=\"Vector3\" x=\"7\" y=\"9.8f\" z=\"4\" w=\"1.6f\"/> <Vector Name=\"Vector4\" x=\"9\" y=\"5.8f\" z=\"4\" w=\"1.9f\"/>  </Matrix></Scope>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(sharedData->GetScope().Find("Matrix1")->Get<mat4x4>(0) == mat4x4(vec4(2, 5.6f, 0, 0.3f), vec4(5, 9.6f, 8, 7.3f), vec4(7, 9.8, 4, 1.6f), vec4(9, 5.8f, 4, 1.6f)));
			Assert::IsTrue(sharedData->GetScope().Find("Matrix1")->Get<mat4x4>(1) == mat4x4(vec4(2, 5.6f, 0, 0.3f), vec4(5, 9.6f, 8, 7.3f), vec4(7, 9.8, 4, 1.6f), vec4(9, 5.8f, 4, 0.6f)));
			Assert::IsTrue(sharedData->GetScope().Find("Matrix2")->Get<mat4x4>(0) == mat4x4(vec4(2, 5.6f, 0, 0.3f), vec4(5, 9.6f, 8, 7.3f), vec4(7, 9.8, 4, 1.6f), vec4(9, 5.8f, 4, 1.9f)));

			XmlParseMaster* cloneMaster = master.Clone();
			xmlToParse = "<Scope Name=\"Lvl1\"> <Matrix Name=\"Matrix1\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/> <Vector Name=\"Vector2\" x=\"5\" y=\"9.6f\" z=\"8\" w=\"7.3f\"/> <Vector Name=\"Vector3\" x=\"7\" y=\"9.8f\" z=\"4\" w=\"1.6f\"/> <Vector Name=\"Vector4\" x=\"9\" y=\"5.8f\" z=\"4\" w=\"1.6f\"/>  </Matrix> <Matrix Name=\"Matrix1\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/> <Vector Name=\"Vector2\" x=\"5\" y=\"9.6f\" z=\"8\" w=\"7.3f\"/> <Vector Name=\"Vector3\" x=\"7\" y=\"9.8f\" z=\"4\" w=\"1.6f\"/> <Vector Name=\"Vector4\" x=\"9\" y=\"5.8f\" z=\"4\" w=\"0.6f\"/>  </Matrix> <Matrix Name=\"Matrix2\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/> <Vector Name=\"Vector2\" x=\"5\" y=\"9.6f\" z=\"8\" w=\"7.3f\"/> <Vector Name=\"Vector3\" x=\"7\" y=\"9.8f\" z=\"4\" w=\"1.6f\"/> <Vector Name=\"Vector4\" x=\"9\" y=\"5.8f\" z=\"4\" w=\"1.9f\"/>  </Matrix></Scope>";
			cloneMaster->Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("Matrix1")->Get<mat4x4>(0) == mat4x4(vec4(2, 5.6f, 0, 0.3f), vec4(5, 9.6f, 8, 7.3f), vec4(7, 9.8, 4, 1.6f), vec4(9, 5.8f, 4, 1.6f)));
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("Matrix1")->Get<mat4x4>(1) == mat4x4(vec4(2, 5.6f, 0, 0.3f), vec4(5, 9.6f, 8, 7.3f), vec4(7, 9.8, 4, 1.6f), vec4(9, 5.8f, 4, 0.6f)));
			Assert::IsTrue(cloneMaster->GetSharedData().As<TableSharedData>()->GetScope().Find("Matrix2")->Get<mat4x4>(0) == mat4x4(vec4(2, 5.6f, 0, 0.3f), vec4(5, 9.6f, 8, 7.3f), vec4(7, 9.8, 4, 1.6f), vec4(9, 5.8f, 4, 1.9f)));

			delete cloneMaster;
			delete scopeHelper;
			delete numberHelper;
			delete vectorHelper;
			delete matrixHelper;
			delete sharedData;
			delete testSharedData;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseHelpersRTTITest)
		{
			HelperRTTITestTemplate<XmlParseHelperTable>("XmlParseHelperTable");
			HelperRTTITestTemplate<XmlParseHelperNumber>("XmlParseHelperNumber");
			HelperRTTITestTemplate<XmlParseHelperString>("XmlParseHelperString");
			HelperRTTITestTemplate<XmlParseHelperVector>("XmlParseHelperVector");
			HelperRTTITestTemplate<XmlParseHelperMatrix>("XmlParseHelperMatrix");
		}

		/************************************************************************/
		TEST_METHOD(TableSharedDataRTTITest)
		{
			RTTI* sharedData = new TableSharedData();

			Assert::IsNotNull(sharedData->As<TableSharedData>());
			Assert::IsNotNull(sharedData->As<XmlParseMaster::SharedData>());
			Assert::IsNull(sharedData->As<XmlParseHelperTable>());

			Assert::IsTrue(sharedData->Is("TableSharedData"));
			Assert::IsTrue(sharedData->Is(sharedData->TypeIdInstance()));
			Assert::IsTrue(sharedData->Is(sharedData->As<TableSharedData>()->TypeIdClass()));
			Assert::IsTrue(sharedData->As<TableSharedData>()->TypeName() == "TableSharedData");

			Assert::IsTrue(sharedData->QueryInterface(sharedData->TypeIdInstance())->ToString() == "RTTI");
			Assert::IsNotNull(sharedData->As<TableSharedData>());

			Assert::IsTrue(sharedData->As<TableSharedData>()->Equals(sharedData));

			delete sharedData;
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState ScopeXmlParseTest::sStartMemState;
}