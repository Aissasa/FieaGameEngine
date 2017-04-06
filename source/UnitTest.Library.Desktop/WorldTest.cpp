#include "pch.h"
#include "CppUnitTest.h"
#include "World.h"
#include "WorldState.h"
#include "Sector.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "TestEntity.h"
#include "WorldSharedData.h"
#include "XmlParseHelperWorld.h"
#include "TestSharedData.h"
#include "XmlParseHelperNumber.h"
#include "XmlParseHelperSector.h"
#include "XmlParseHelperEntity.h"
#include "XmlParseHelperString.h"
#include "XmlParseHelperVector.h"
#include "XmlParseHelperMatrix.h"
#include "TestAction.h"
#include "ActionList.h"
#include "AttributedFoo.h"
#include "XmlParseHelperAction.h"
#include "ActionListIf.h"
#include "XmlParseHelperActionIf.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace glm;
using namespace Library;

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(WorldTest)
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
			World::ClearCashedPrescribedAttributes();
			Sector::ClearCashedPrescribedAttributes();
			Entity::ClearCashedPrescribedAttributes();

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

#pragma region TestMethods

#pragma region Action

		/************************************************************************/
		TEST_METHOD(ActionTest)
		{
			TestAction* action = new TestAction();

			Assert::IsTrue(action->IsPrescribedAttribute("Name"));
			Assert::IsTrue(action->IsPrescribedAttribute("Number"));
			Assert::IsTrue(action->Name() == "");
			action->SetName("Yo");
			Assert::IsTrue(action->Name() == "Yo");

			WorldState state;
			action->Update(state);
			Assert::IsTrue(action->GetNumber() == 1);

			Assert::IsNull(action->GetPredecessor());

			Attributed* attr = new AttributedFoo();
			auto func = [&action, &attr] { action->SetPredecessor(*attr); };
			Assert::ExpectException<exception>(func);

			ActionList* actionList = new ActionList();
			Assert::IsTrue(actionList->Actions().IsEmpty());
			action->SetPredecessor(*actionList);
			Assert::IsFalse(actionList->Actions().IsEmpty());
			Assert::IsTrue(action->GetPredecessor() == actionList);

			TestActionFactory* fact = new TestActionFactory();
			auto& newAction = actionList->CreateAction("TestAction", "TestAction1");
			Assert::IsTrue(&actionList->Actions()[1] == &newAction);

			WorldState worldState;
			actionList->Update(worldState);
			Assert::IsTrue(action->GetNumber() == 2);
			Assert::IsTrue(newAction.As<TestAction>()->GetNumber() == 1);
			
			delete fact;

			// test create action
			EntityFactory entityFact;
			World* world = new World();
			auto& sector = world->CreateSector("Sector");
			auto& entity = sector.CreateEntity("Entity", "Entity1");

			auto func1 = [&world] { world->CreateAction("TestAction", "WorldAction1"); };
			Assert::ExpectException<exception>(func1);

			auto func2 = [&sector] { sector.CreateAction("TestAction", "SectorAction1"); };
			Assert::ExpectException<exception>(func2);

			auto func3 = [&entity] { entity.CreateAction("TestAction", "EntityAction1"); };
			Assert::ExpectException<exception>(func3);

			TestActionFactory fact1;
			auto& worldAction1 = world->CreateAction("TestAction", "WorldAction1");
			auto& worldAction2 = world->CreateAction("TestAction", "WorldAction2");
			auto& worldAction3 = world->CreateAction("TestAction", "WorldAction3");
			auto& sectorAction1 = sector.CreateAction("TestAction", "SectorAction1");
			auto& sectorAction2 = sector.CreateAction("TestAction", "SectorAction2");
			auto& sectorAction3 = sector.CreateAction("TestAction", "SectorAction3");
			auto& entityAction1 = entity.CreateAction("TestAction", "EntityAction1");
			auto& entityAction2 = entity.CreateAction("TestAction", "EntityAction2");
			auto& entityAction3 = entity.CreateAction("TestAction", "EntityAction3");

			Assert::IsTrue(&world->Actions()[0] == &worldAction1);
			Assert::IsTrue(&world->Actions()[1] == &worldAction2);
			Assert::IsTrue(&world->Actions()[2] == &worldAction3);
			Assert::IsTrue(&sector.Actions()[0] == &sectorAction1);
			Assert::IsTrue(&sector.Actions()[1] == &sectorAction2);
			Assert::IsTrue(&sector.Actions()[2] == &sectorAction3);
			Assert::IsTrue(&entity.Actions()[0] == &entityAction1);
			Assert::IsTrue(&entity.Actions()[1] == &entityAction2);
			Assert::IsTrue(&entity.Actions()[2] == &entityAction3);

			// test set predecessor
			sectorAction2.SetPredecessor(*world);
			Assert::IsTrue(&world->Actions()[3] == &sectorAction2);
			Assert::IsTrue(sector.Actions().Size() == 2u);

			actionList->SetPredecessor(entity);
			Assert::IsTrue(&entity.Actions()[3] == actionList);

			// test destroy action
			world->DestroyAction(worldState, "WorldAction2");
			sector.DestroyAction(worldState, "SectorAction3");
			entity.DestroyAction(worldState, "EntityAction2");
			actionList->DestroyAction(worldState, "TestAction1");

			world->Update(worldState);
			Assert::IsTrue(world->Actions().Size() == 4);
			Assert::IsTrue(sector.Actions().Size() == 2);
			Assert::IsTrue(entity.Actions().Size() == 4);
			Assert::IsTrue(actionList->Actions().Size() == 2);

			world->DestroyAction(worldState, "WorldAction2");
			sector.DestroyAction(worldState, "SectorAction3");
			entity.DestroyAction(worldState, "EntityAction2");
			actionList->DestroyAction(worldState, "TestAction1");
			world->DestroyAction(worldState, "Test");
			sector.DestroyAction(worldState, "Test");
			entity.DestroyAction(worldState, "Test");
			actionList->DestroyAction(worldState, "Test");

			world->Update(worldState);
			Assert::IsTrue(world->Actions().Size() == 3);
			Assert::IsTrue(sector.Actions().Size() == 1);
			Assert::IsTrue(entity.Actions().Size() == 3);
			Assert::IsTrue(actionList->Actions().Size() == 1);

			Assert::IsTrue(worldAction1.As<TestAction>()->GetNumber() == 2);
			Assert::IsTrue(sectorAction1.As<TestAction>()->GetNumber() == 2);
			Assert::IsTrue(entityAction1.As<TestAction>()->GetNumber() == 2);
			Assert::IsTrue(action->GetNumber() == 4);

			delete attr;
			delete actionList;
			delete world;
		}

		/************************************************************************/
		TEST_METHOD(ActionListIfTest)
		{
			ActionListIf* actionListIf = new ActionListIf();
			actionListIf->SetName("TestIf");
			int32_t number = 5;
			Datum& dat = actionListIf->AppendAuxiliaryAttribute("Number");
			dat.PushBack(number);
			actionListIf->SetConditionIntegerName("Number");
			
			// test no actions
			WorldState world_state;
			auto func = [&actionListIf, &world_state] { actionListIf->Update(world_state); };
			Assert::ExpectException<exception>(func);

			TestActionFactory fact;
			Action& thenAction = actionListIf->CreateThenAction("TestAction");
			Action& elseAction = actionListIf->CreateElseAction("TestAction");

			// test no condition and wrong type
			actionListIf->SetConditionIntegerName("nope");
			actionListIf->Update(world_state);
			Assert::IsTrue(thenAction.As<TestAction>()->GetNumber() == 0);
			Assert::IsTrue(elseAction.As<TestAction>()->GetNumber() == 0);

			actionListIf->AppendAuxiliaryAttribute("Float").PushBack(5.6f);
			actionListIf->SetConditionIntegerName("Float");
			actionListIf->Update(world_state);
			Assert::IsTrue(thenAction.As<TestAction>()->GetNumber() == 0);
			Assert::IsTrue(elseAction.As<TestAction>()->GetNumber() == 0);

			// test then
			actionListIf->SetConditionIntegerName("Number");
			actionListIf->Update(world_state);
			Assert::IsTrue(thenAction.As<TestAction>()->GetNumber() == 1);
			Assert::IsTrue(elseAction.As<TestAction>()->GetNumber() == 0);

			// test else
			dat = 0;
			actionListIf->Update(world_state);
			Assert::IsTrue(thenAction.As<TestAction>()->GetNumber() == 1);
			Assert::IsTrue(elseAction.As<TestAction>()->GetNumber() == 1);

			actionListIf->DestroyElseAction(world_state);
			actionListIf->DestroyThenAction(world_state);

			delete actionListIf;
		}

		/************************************************************************/
		TEST_METHOD(ActionRTTITest)
		{
			RTTI* action = new TestAction();

			Assert::IsNotNull(action->As<Attributed>());
			Assert::IsNotNull(action->As<TestAction>());
			Assert::IsNull(action->As<Sector>());

			Assert::IsTrue(action->Is("Action"));
			Assert::IsTrue(action->Is(action->TypeIdInstance()));
			Assert::IsTrue(action->Is(action->As<Action>()->TypeIdClass()));
			Assert::IsTrue(action->As<Action>()->TypeName() == "Action");

			Assert::IsTrue(action->QueryInterface(action->TypeIdInstance())->ToString() != "RTTI");
			Assert::IsNotNull(action->As<Action>());

			Assert::IsTrue(action->As<Action>()->Equals(action));

			delete action;
		}

		/************************************************************************/
		TEST_METHOD(ActionListRTTITest)
		{
			RTTI* action = new ActionList();

			Assert::IsNotNull(action->As<Attributed>());
			Assert::IsNotNull(action->As<ActionList>());
			Assert::IsNull(action->As<Sector>());

			Assert::IsTrue(action->Is("ActionList"));
			Assert::IsTrue(action->Is(action->TypeIdInstance()));
			Assert::IsTrue(action->Is(action->As<ActionList>()->TypeIdClass()));
			Assert::IsTrue(action->As<ActionList>()->TypeName() == "ActionList");

			Assert::IsTrue(action->QueryInterface(action->TypeIdInstance())->ToString() != "RTTI");
			Assert::IsNotNull(action->As<Action>());

			Assert::IsTrue(action->As<ActionList>()->Equals(action));

			delete action;
		}

		/************************************************************************/
		TEST_METHOD(ActionListIfRTTITest)
		{
			RTTI* action = new ActionListIf();

			Assert::IsNotNull(action->As<Attributed>());
			Assert::IsNotNull(action->As<ActionListIf>());
			Assert::IsNull(action->As<Sector>());

			Assert::IsTrue(action->Is("ActionList"));
			Assert::IsTrue(action->Is(action->TypeIdInstance()));
			Assert::IsTrue(action->Is(action->As<ActionListIf>()->TypeIdClass()));
			Assert::IsTrue(action->As<ActionListIf>()->TypeName() == "ActionListIf");

			Assert::IsTrue(action->QueryInterface(action->TypeIdInstance())->ToString() != "RTTI");
			Assert::IsNotNull(action->As<ActionList>());

			Assert::IsTrue(action->As<ActionListIf>()->Equals(action));

			delete action;
		}

#pragma endregion 

#pragma region Entity

		/************************************************************************/
		TEST_METHOD(EntityTest)
		{
			TestEntity* test_entity = new TestEntity();

			Assert::IsTrue(test_entity->IsPrescribedAttribute("Name"));
			Assert::IsTrue(test_entity->IsPrescribedAttribute("Number"));
			Assert::IsTrue(test_entity->Name() == "");
			test_entity->SetName("Yo");
			Assert::IsTrue(test_entity->Name() == "Yo");
			
			WorldState state;
			test_entity->Update(state);
			Assert::IsTrue(test_entity->GetNumber() == 1);

			Assert::IsTrue(test_entity->GetSector() == nullptr);
			Sector* sector = new Sector();
			test_entity->SetSector(*sector);
			Assert::IsTrue(test_entity->GetSector() == sector);

			delete sector;
		}

		/************************************************************************/
		TEST_METHOD(EntityRTTITest)
		{
			RTTI* entity = new Entity();

			Assert::IsNotNull(entity->As<Attributed>());
			Assert::IsNotNull(entity->As<Entity>());
			Assert::IsNull(entity->As<Sector>());

			Assert::IsTrue(entity->Is("Entity"));
			Assert::IsTrue(entity->Is(entity->TypeIdInstance()));
			Assert::IsTrue(entity->Is(entity->As<Entity>()->TypeIdClass()));
			Assert::IsTrue(entity->As<Entity>()->TypeName() == "Entity");

			Assert::IsTrue(entity->QueryInterface(entity->TypeIdInstance())->ToString() != "RTTI");
			Assert::IsNotNull(entity->As<Entity>());

			Assert::IsTrue(entity->As<Entity>()->Equals(entity));

			delete entity;
		}

#pragma endregion 

#pragma region Sector

		/************************************************************************/
		TEST_METHOD(SectorTest)
		{
			Sector* sector = new Sector();

			Assert::IsTrue(sector->IsPrescribedAttribute("Name"));
			Assert::IsTrue(sector->IsPrescribedAttribute("Entities"));
			Assert::IsTrue(sector->Name() == "");
			sector->SetName("Yo");
			Assert::IsTrue(sector->Name() == "Yo");

			Assert::IsTrue(sector->Entities().IsEmpty());

			auto func = [&sector] { sector->CreateEntity("Entity", "Entity1"); };
			Assert::ExpectException<exception>(func);

			EntityFactory factory;
			auto& entity = sector->CreateEntity("Entity", "Entity1");
			Assert::IsTrue(entity.Is(Entity::TypeIdClass()));
			Assert::IsTrue(entity.Name() == "Entity1");
			Assert::IsTrue(entity.GetSector() == sector);
			Assert::IsFalse(sector->Entities().IsEmpty());

			TestEntityFactory test_entity_factory;
			auto& testEntity = sector->CreateEntity("TestEntity", "Ent");
			Assert::IsTrue(testEntity.Is(TestEntity::TypeIdClass()));
			Assert::IsTrue(testEntity.Name() == "Ent");
			Assert::IsTrue(testEntity.GetSector() == sector);

			WorldState state;
			sector->Update(state);
			Assert::IsTrue(testEntity.As<TestEntity>()->GetNumber() == 1);

			Assert::IsNull(sector->GetWorld());
			World* world = new World();
			sector->SetWorld(*world);
			Assert::IsTrue(sector->GetWorld() == world);

			delete world;
		}

		/************************************************************************/
		TEST_METHOD(SectorRTTITest)
		{
			RTTI* sector = new Sector();

			Assert::IsNotNull(sector->As<Attributed>());
			Assert::IsNotNull(sector->As<Sector>());
			Assert::IsNull(sector->As<Entity>());

			Assert::IsTrue(sector->Is("Sector"));
			Assert::IsTrue(sector->Is(sector->TypeIdInstance()));
			Assert::IsTrue(sector->Is(sector->As<Sector>()->TypeIdClass()));
			Assert::IsTrue(sector->As<Sector>()->TypeName() == "Sector");

			Assert::IsTrue(sector->QueryInterface(sector->TypeIdInstance())->ToString() != "RTTI");
			Assert::IsNotNull(sector->As<Sector>());

			Assert::IsTrue(sector->As<Sector>()->Equals(sector));

			delete sector;
		}

#pragma endregion 

#pragma region World

		/************************************************************************/
		TEST_METHOD(WorldTestMeth)
		{
			World* world = new World();

			Assert::IsTrue(world->IsPrescribedAttribute("Name"));
			Assert::IsTrue(world->IsPrescribedAttribute("Sectors"));
			Assert::IsTrue(world->Name() == "");
			world->SetName("Yo");
			Assert::IsTrue(world->Name() == "Yo");

			Assert::IsTrue(world->Sectors().IsEmpty());

			auto& sector1 = world->CreateSector("Sect1");
			Assert::IsTrue(sector1.Is(Sector::TypeIdClass()));
			Assert::IsTrue(sector1.Name() == "Sect1");
			Assert::IsTrue(sector1.GetWorld() == world);
			Assert::IsFalse(world->Sectors().IsEmpty());

			auto& sector2 = world->CreateSector("Sect2");
			Assert::IsTrue(sector2.Is(Sector::TypeIdClass()));
			Assert::IsTrue(sector2.Name() == "Sect2");
			Assert::IsTrue(sector2.GetWorld() == world);

			EntityFactory fact1;
			TestEntityFactory fact2;
			sector1.CreateEntity("Entity", "ent1");
			auto& ent1 = sector1.CreateEntity("TestEntity", "Testent1");
			sector1.CreateEntity("Entity", "ent2");
			auto& ent2 = sector1.CreateEntity("TestEntity", "Testent2");

			WorldState state;
			world->Update(state);
			Assert::IsTrue(state.GetWorld() == world);
			Assert::IsNull(state.GetSector());
			Assert::IsNull(state.GetEntity());
			world->Update(state);
			Assert::IsTrue(ent1.As<TestEntity>()->GetNumber() == 2);
			Assert::IsTrue(ent2.As<TestEntity>()->GetNumber() == 2);

			delete world;
		}

		/************************************************************************/
		TEST_METHOD(WorldRTTITest)
		{
			RTTI* world = new World();

			Assert::IsNotNull(world->As<Attributed>());
			Assert::IsNotNull(world->As<World>());
			Assert::IsNull(world->As<Entity>());

			Assert::IsTrue(world->Is("World"));
			Assert::IsTrue(world->Is(world->TypeIdInstance()));
			Assert::IsTrue(world->Is(world->As<World>()->TypeIdClass()));
			Assert::IsTrue(world->As<World>()->TypeName() == "World");

			Assert::IsTrue(world->QueryInterface(world->TypeIdInstance())->ToString() != "RTTI");
			Assert::IsNotNull(world->As<World>());

			Assert::IsTrue(world->As<World>()->Equals(world));

			delete world;
		}

#pragma endregion 

#pragma region XmlParsing

		/************************************************************************/
		TEST_METHOD(XmlParseHelperWorldTest)
		{
			// non working shared data
			TestSharedData* testSharedData = new TestSharedData();
			XmlParseMaster master1(testSharedData);

			XmlParseHelperWorld* worldHelper = new XmlParseHelperWorld();
			master1.AddHelper(*worldHelper);

			string xmlToParse("<World Name=\"Lvl1\"/>");
			master1.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(worldHelper->mStartElementHandlerCount == 0);
			Assert::IsTrue(worldHelper->mEndElementHandlerCount == 0);

			WorldSharedData* sharedData = new WorldSharedData();
			XmlParseMaster master(sharedData);
			master.AddHelper(*worldHelper);

			// no name
			xmlToParse = "<World Test=\"Yo\"/>";
			auto func1 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func1);

			// nested world
			xmlToParse = "<World Name=\"Yo\"><World Name=\"Yo2\"/></World>";
			auto func2 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func2);

			// simple world
			xmlToParse = "<World Test=\"Yo\" Name=\"Lvl1\">bla</World>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(sharedData->GetScope());
			Assert::IsTrue(sharedData->GetScope()->Is(World::TypeIdClass()));
			Assert::IsTrue(sharedData->GetScope()->As<World>()->Sectors().IsEmpty());
			Assert::IsTrue(sharedData->GetScope()->As<World>()->Name() == "Lvl1");

			// clone helper
			XmlParseMaster* cloneMaster = master.Clone();
			cloneMaster->Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			auto& cloneSharedData = cloneMaster->GetSharedData();
			Assert::IsTrue(cloneSharedData.Is(WorldSharedData::TypeIdClass()));
			WorldSharedData& worldCloneSharedData = static_cast<WorldSharedData&>(cloneSharedData);
			Assert::IsNotNull(worldCloneSharedData.GetScope());
			Assert::IsTrue(worldCloneSharedData.GetScope()->Is(World::TypeIdClass()));
			Assert::IsTrue(worldCloneSharedData.GetScope()->As<World>()->Sectors().IsEmpty());
			Assert::IsTrue(worldCloneSharedData.GetScope()->As<World>()->Name() == "Lvl1");

			delete cloneMaster;
			delete worldHelper;
			delete sharedData;
			delete testSharedData;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseHelperSectorTest)
		{
			// non working shared data
			TestSharedData* testSharedData = new TestSharedData();
			XmlParseMaster master1(testSharedData);

			XmlParseHelperWorld* worldHelper = new XmlParseHelperWorld();
			XmlParseHelperSector* sectorHelper = new XmlParseHelperSector();
			master1.AddHelper(*worldHelper);
			master1.AddHelper(*sectorHelper);

			string xmlToParse("<World Name=\"Lvl1\"><Sector Name=\"Sec1\"></Sector></World>");
			master1.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(worldHelper->mStartElementHandlerCount == 0);
			Assert::IsTrue(worldHelper->mEndElementHandlerCount == 0);

			WorldSharedData* sharedData = new WorldSharedData();
			XmlParseMaster master(sharedData);
			master.AddHelper(*worldHelper);
			master.AddHelper(*sectorHelper);

			// no name
			xmlToParse = "<World Name=\"Lvl1\"><Sector NonName=\"Sec1\"></Sector></World>";
			auto func1 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func1);

			// not under world
			xmlToParse = "<Sector Name=\"Sec2\"></Sector>";
			auto func2 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func2);

			// nested sector
			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Sector Name=\"Sec2\"></Sector></Sector></World>";
			auto func3 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func3);
			// to actually delete the world
			sharedData->SetScope(*sharedData->GetScope()->As<Sector>()->GetWorld());

			// simple sector
			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"></Sector></World>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(sharedData->GetScope());
			Assert::IsTrue(sharedData->GetScope()->Is(World::TypeIdClass()));
			Assert::IsFalse(sharedData->GetScope()->As<World>()->Sectors().IsEmpty());
			Assert::IsTrue(sharedData->GetScope()->As<World>()->Sectors().operator[](0).As<Sector>()->Name() == "Sec1");

			// two sectors
			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"></Sector><Sector Name=\"Sec2\"></Sector></World>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(sharedData->GetScope());
			Assert::IsTrue(sharedData->GetScope()->As<World>()->Sectors().operator[](1).As<Sector>()->Name() == "Sec2");

			// clone helper
			XmlParseMaster* cloneMaster = master.Clone();
			cloneMaster->Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			auto& cloneSharedData = cloneMaster->GetSharedData();
			Assert::IsTrue(cloneSharedData.Is(WorldSharedData::TypeIdClass()));
			WorldSharedData& worldCloneSharedData = static_cast<WorldSharedData&>(cloneSharedData);
			Assert::IsNotNull(worldCloneSharedData.GetScope());
			Assert::IsTrue(worldCloneSharedData.GetScope()->Is(World::TypeIdClass()));
			Assert::IsFalse(worldCloneSharedData.GetScope()->As<World>()->Sectors().IsEmpty());
			Assert::IsTrue(worldCloneSharedData.GetScope()->As<World>()->Sectors().operator[](0).As<Sector>()->Name() == "Sec1");
			Assert::IsTrue(worldCloneSharedData.GetScope()->As<World>()->Sectors().operator[](1).As<Sector>()->Name() == "Sec2");

			delete cloneMaster;
			delete worldHelper;
			delete sectorHelper;
			delete sharedData;
			delete testSharedData;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseHelperEntityTest)
		{
			// non working shared data
			TestSharedData* testSharedData = new TestSharedData();
			XmlParseMaster master1(testSharedData);

			XmlParseHelperWorld* worldHelper = new XmlParseHelperWorld();
			XmlParseHelperSector* sectorHelper = new XmlParseHelperSector();
			XmlParseHelperEntity* entityHelper = new XmlParseHelperEntity();
			XmlParseHelperNumber* numberHelper = new XmlParseHelperNumber();
			XmlParseHelperString* stringHelper = new XmlParseHelperString();
			XmlParseHelperVector* vectorHelper = new XmlParseHelperVector();
			XmlParseHelperMatrix* matrixHelper = new XmlParseHelperMatrix();
			master1.AddHelper(*worldHelper);
			master1.AddHelper(*sectorHelper);
			master1.AddHelper(*entityHelper);
			master1.AddHelper(*numberHelper);
			master1.AddHelper(*stringHelper);
			master1.AddHelper(*vectorHelper);
			master1.AddHelper(*matrixHelper);

			string xmlToParse("<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/></Entity></Sector></World>");
			master1.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(worldHelper->mStartElementHandlerCount == 0);
			Assert::IsTrue(worldHelper->mEndElementHandlerCount == 0);

			WorldSharedData* sharedData = new WorldSharedData();
			XmlParseMaster master(sharedData);
			master.AddHelper(*worldHelper);
			master.AddHelper(*sectorHelper);
			master.AddHelper(*entityHelper);
			master.AddHelper(*numberHelper);
			master.AddHelper(*stringHelper);
			master.AddHelper(*vectorHelper);
			master.AddHelper(*matrixHelper);

			EntityFactory fact;

			// not under sector
			xmlToParse = "<Entity Class=\"Entity\" InstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/></Entity>";
			auto func2 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func2);

			// no names
			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" NonInstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/></Entity></Sector></World>";
			auto func0 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func0);
			// to actually delete the world
			sharedData->SetScope(*sharedData->GetScope()->As<Sector>()->GetWorld());

			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity NonClass=\"Entity\" InstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/></Entity></Sector></World>";
			auto func1 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func1);
			// to actually delete the world
			sharedData->SetScope(*sharedData->GetScope()->As<Sector>()->GetWorld());

			// nested entity
			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Entity Class=\"Entity\" InstanceName=\"Ent2\"></Entity></Entity></Sector></Sector></World>";
			auto func3 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func3);
			// to actually delete the world
			sharedData->SetScope(*sharedData->GetScope()->As<Entity>()->GetSector()->GetWorld());

			// simple entity
			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/></Entity></Sector></World>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(sharedData->GetScope());
			Assert::IsTrue(sharedData->GetScope()->Is(World::TypeIdClass()));
			Assert::IsFalse(sharedData->GetScope()->As<World>()->Sectors().IsEmpty());
			Scope& ent = sharedData->GetScope()->As<World>()->Sectors().operator[](0).As<Sector>()->Entities().operator[](0);
			Assert::IsTrue(ent.As<Entity>()->Name() == "Ent");
			Assert::IsTrue(ent.As<Entity>()->IsAuxiliaryAttribute("Fl"));
			Assert::IsTrue(ent.As<Entity>()->Find("Fl")->Get<float>() == 5.6f);

			// simple entity with vector and matrix
			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/><Matrix Name=\"Matrix1\"> <Vector Name=\"Vector1\" x=\"2\" y=\"5.6f\" z=\"0\" w=\"0.3f\"/> <Vector Name=\"Vector2\" x=\"5\" y=\"9.6f\" z=\"8\" w=\"7.3f\"/> <Vector Name=\"Vector3\" x=\"7\" y=\"9.8f\" z=\"4\" w=\"1.6f\"/> <Vector Name=\"Vector4\" x=\"9\" y=\"5.8f\" z=\"4\" w=\"1.6f\"/> </Matrix></Entity></Sector></World>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(sharedData->GetScope());
			Assert::IsTrue(sharedData->GetScope()->Is(World::TypeIdClass()));
			Assert::IsFalse(sharedData->GetScope()->As<World>()->Sectors().IsEmpty());
			Scope& ent10 = sharedData->GetScope()->As<World>()->Sectors().operator[](0).As<Sector>()->Entities().operator[](0);
			Assert::IsTrue(ent10.As<Entity>()->Name() == "Ent");
			Assert::IsTrue(ent10.As<Entity>()->IsAuxiliaryAttribute("Vector1"));
			Assert::IsTrue(ent10.As<Entity>()->Find("Vector1")->Get<vec4>() == vec4(2, 5.6f, 0, 0.3f));
			Assert::IsTrue(ent10.As<Entity>()->IsAuxiliaryAttribute("Matrix1"));
			Assert::IsTrue(ent10.As<Entity>()->Find("Matrix1")->Get<mat4x4>() == mat4x4(2, 5.6f, 0, 0.3f, 5, 9.6f, 8, 7.3f, 7, 9.8f, 4, 1.6f, 9, 5.8f, 4, 1.6f));

			// two entities
			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><String Name=\"Str1\" Value=\"yo\"/></Entity><Entity Class=\"Entity\" InstanceName=\"Ent2\"><Integer Name=\"Int1\" Value=\"5\"/></Entity></Sector></World>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Scope& ent1 = sharedData->GetScope()->As<World>()->Sectors().operator[](0).As<Sector>()->Entities().operator[](0);
			Scope& ent2 = sharedData->GetScope()->As<World>()->Sectors().operator[](0).As<Sector>()->Entities().operator[](1);
			Assert::IsTrue(ent1.As<Entity>()->Name() == "Ent");
			Assert::IsTrue(ent1.As<Entity>()->IsAuxiliaryAttribute("Str1"));
			Assert::IsTrue(ent1.As<Entity>()->Find("Str1")->Get<string>() == "yo");
			Assert::IsTrue(ent2.As<Entity>()->Name() == "Ent2");
			Assert::IsTrue(ent2.As<Entity>()->IsAuxiliaryAttribute("Int1"));
			Assert::IsTrue(ent2.As<Entity>()->Find("Int1")->Get<int32_t>() == 5);

			// clone helper
			XmlParseMaster* cloneMaster = master.Clone();
			cloneMaster->Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			auto& cloneSharedData = cloneMaster->GetSharedData();
			Assert::IsTrue(cloneSharedData.Is(WorldSharedData::TypeIdClass()));
			WorldSharedData& worldCloneSharedData = static_cast<WorldSharedData&>(cloneSharedData);
			Assert::IsNotNull(worldCloneSharedData.GetScope());
			Assert::IsTrue(worldCloneSharedData.GetScope()->Is(World::TypeIdClass()));
			Assert::IsFalse(worldCloneSharedData.GetScope()->As<World>()->Sectors().IsEmpty());
			Scope& ent3 = worldCloneSharedData.GetScope()->As<World>()->Sectors().operator[](0).As<Sector>()->Entities().operator[](0);
			Assert::IsTrue(ent3.As<Entity>()->Name() == "Ent");
			Assert::IsTrue(ent3.As<Entity>()->IsAuxiliaryAttribute("Str1"));
			Assert::IsTrue(ent3.As<Entity>()->Find("Str1")->Get<string>() == "yo");

			// todo fix this case, where the shared data now points to the cloned master
			// simple entity
			//xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/></Entity></Sector></World>";
			//master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			//Assert::IsNotNull(sharedData->GetScope());
			//Assert::IsTrue(sharedData->GetScope()->Is(World::TypeIdClass()));
			//Assert::IsFalse(sharedData->GetScope()->As<World>()->Sectors().IsEmpty());
			//Scope& ent0 = sharedData->GetScope()->As<World>()->Sectors().operator[](0).As<Sector>()->Entities().operator[](0);
			//Assert::IsTrue(ent0.As<Entity>()->Name() == "Ent");
			//Assert::IsTrue(ent0.As<Entity>()->IsAuxiliaryAttribute("Fl"));
			//Assert::IsTrue(ent0.As<Entity>()->Find("Fl")->Get<float>() == 5.6f);


			delete cloneMaster;
			delete worldHelper;
			delete sectorHelper;
			delete entityHelper;
			delete numberHelper;
			delete stringHelper;
			delete vectorHelper;
			delete matrixHelper;
			delete sharedData;
			delete testSharedData;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseHelperActionTest)
		{
			// non working shared data
			TestSharedData* testSharedData = new TestSharedData();
			XmlParseMaster master1(testSharedData);

			XmlParseHelperWorld* worldHelper = new XmlParseHelperWorld();
			XmlParseHelperSector* sectorHelper = new XmlParseHelperSector();
			XmlParseHelperEntity* entityHelper = new XmlParseHelperEntity();
			XmlParseHelperAction* actionHelper = new XmlParseHelperAction();
			XmlParseHelperNumber* numberHelper = new XmlParseHelperNumber();
			XmlParseHelperString* stringHelper = new XmlParseHelperString();
			XmlParseHelperVector* vectorHelper = new XmlParseHelperVector();
			XmlParseHelperMatrix* matrixHelper = new XmlParseHelperMatrix();
			master1.AddHelper(*worldHelper);
			master1.AddHelper(*sectorHelper);
			master1.AddHelper(*entityHelper);
			master1.AddHelper(*actionHelper);
			master1.AddHelper(*numberHelper);
			master1.AddHelper(*stringHelper);
			master1.AddHelper(*vectorHelper);
			master1.AddHelper(*matrixHelper);

			string xmlToParse("<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/><Action Class=\"TestAction\" InstanceName=\"Action\"/></Entity></Sector></World>");
			master1.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(worldHelper->mStartElementHandlerCount == 0);
			Assert::IsTrue(worldHelper->mEndElementHandlerCount == 0);

			WorldSharedData* sharedData = new WorldSharedData();
			XmlParseMaster master(sharedData);
			master.AddHelper(*worldHelper);
			master.AddHelper(*sectorHelper);
			master.AddHelper(*entityHelper);
			master.AddHelper(*actionHelper);
			master.AddHelper(*numberHelper);
			master.AddHelper(*stringHelper);
			master.AddHelper(*vectorHelper);
			master.AddHelper(*matrixHelper);

			EntityFactory fact;
			TestActionFactory testActionFact;
			ActionListFactory actionListFact;

			// not nested
			xmlToParse = "<Action Class=\"TestAction\" InstanceName=\"Action\"/>";
			auto func2 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func2);

			// no names
			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/><Action NonClass=\"TestAction\" InstanceName=\"Action\"/></Entity></Sector></World>";
			auto func0 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func0);
			// to actually delete the world
			sharedData->SetScope(*sharedData->GetScope()->As<Entity>()->GetSector()->GetWorld());

			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/><Action Class=\"TestAction\" NonInstanceName=\"Action\"/></Entity></Sector></World>";
			auto func1 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func1);
			// to actually delete the world
			sharedData->SetScope(*sharedData->GetScope()->As<Entity>()->GetSector()->GetWorld());

			// simple action
			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Action Class=\"TestAction\" InstanceName=\"Action\"><Float Name=\"Fl\" Value=\"5.6f\"/></Action></Entity></Sector></World>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsNotNull(sharedData->GetScope());
			Assert::IsTrue(sharedData->GetScope()->Is(World::TypeIdClass()));
			Assert::IsFalse(sharedData->GetScope()->As<World>()->Sectors().IsEmpty());
			Scope& ent = sharedData->GetScope()->As<World>()->Sectors().operator[](0).As<Sector>()->Entities().operator[](0);
			Assert::IsTrue(ent.As<Entity>()->Name() == "Ent");
			Scope& action = ent.As<Entity>()->Actions()[0];
			Assert::IsTrue(action.As<Action>()->Name() == "Action");
			Assert::IsTrue(action.As<Action>()->IsAuxiliaryAttribute("Fl"));
			Assert::IsTrue(action.As<Action>()->Find("Fl")->Get<float>() == 5.6f);

			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Action Class=\"TestAction\" InstanceName=\"Action\"><Float Name=\"Fl\" Value=\"5.6f\"/></Action><Entity Class=\"Entity\" InstanceName=\"Ent\"></Entity></Sector></World>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Scope& sect = sharedData->GetScope()->As<World>()->Sectors().operator[](0);
			Assert::IsTrue(sect.As<Sector>()->Name() == "Sec1");
			Scope& action1 = sect.As<Sector>()->Actions()[0];
			Assert::IsTrue(action1.As<Action>()->Name() == "Action");
			Assert::IsTrue(action1.As<Action>()->IsAuxiliaryAttribute("Fl"));
			Assert::IsTrue(action1.As<Action>()->Find("Fl")->Get<float>() == 5.6f);

			xmlToParse = "<World Name=\"Lvl1\"><Action Class=\"ActionList\" InstanceName=\"ActionList\"><Float Name=\"Fl\" Value=\"5.6f\"/><Action Class=\"TestAction\" InstanceName=\"Action\"><String Name=\"Str\" Value=\"Yo\"/></Action></Action><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"></Entity></Sector></World>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Scope& world = *sharedData->GetScope();
			Scope& actionList = world.As<World>()->Actions()[0];
			Assert::IsTrue(actionList.As<Action>()->Name() == "ActionList");
			Assert::IsTrue(actionList.As<Action>()->IsAuxiliaryAttribute("Fl"));
			Assert::IsTrue(actionList.As<Action>()->Find("Fl")->Get<float>() == 5.6f);
			Scope& action2 = actionList.As<ActionList>()->Actions()[0];
			Assert::IsTrue(action2.As<Action>()->Name() == "Action");
			Assert::IsTrue(action2.As<Action>()->IsAuxiliaryAttribute("Str"));
			Assert::IsTrue(action2.As<Action>()->Find("Str")->Get<string>() == "Yo");

			// clone helper
			XmlParseMaster* cloneMaster = master.Clone();
			cloneMaster->Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			auto& cloneSharedData = cloneMaster->GetSharedData();
			Assert::IsTrue(cloneSharedData.Is(WorldSharedData::TypeIdClass()));
			WorldSharedData& worldCloneSharedData = static_cast<WorldSharedData&>(cloneSharedData);
			Assert::IsNotNull(worldCloneSharedData.GetScope());
			Assert::IsTrue(worldCloneSharedData.GetScope()->Is(World::TypeIdClass()));
			Assert::IsFalse(worldCloneSharedData.GetScope()->As<World>()->Sectors().IsEmpty());
			Scope& cloneWorld = *worldCloneSharedData.GetScope();
			Assert::IsTrue(cloneWorld.As<World>()->Actions()[0].As<Action>()->Name() == "ActionList");
			Assert::IsTrue(cloneWorld.As<World>()->Actions()[0].As<Attributed>()->IsAuxiliaryAttribute("Fl"));
			Assert::IsTrue(cloneWorld.As<World>()->Actions()[0].As<ActionList>()->Actions()[0].As<Action>()->Name() == "Action");
			Assert::IsTrue(cloneWorld.As<World>()->Actions()[0].As<ActionList>()->Actions()[0].As<Action>()->IsAuxiliaryAttribute("Str"));


			delete cloneMaster;
			delete worldHelper;
			delete sectorHelper;
			delete entityHelper;
			delete actionHelper;
			delete numberHelper;
			delete stringHelper;
			delete vectorHelper;
			delete matrixHelper;
			delete sharedData;
			delete testSharedData;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseHelperActionListIfTest)
		{
			// non working shared data
			TestSharedData* testSharedData = new TestSharedData();
			XmlParseMaster master1(testSharedData);

			XmlParseHelperWorld* worldHelper = new XmlParseHelperWorld();
			XmlParseHelperSector* sectorHelper = new XmlParseHelperSector();
			XmlParseHelperEntity* entityHelper = new XmlParseHelperEntity();
			XmlParseHelperAction* actionHelper = new XmlParseHelperAction();
			XmlParseHelperActionIf* actionListIfHelper = new XmlParseHelperActionIf();
			XmlParseHelperNumber* numberHelper = new XmlParseHelperNumber();
			XmlParseHelperString* stringHelper = new XmlParseHelperString();
			XmlParseHelperVector* vectorHelper = new XmlParseHelperVector();
			XmlParseHelperMatrix* matrixHelper = new XmlParseHelperMatrix();
			master1.AddHelper(*worldHelper);
			master1.AddHelper(*sectorHelper);
			master1.AddHelper(*entityHelper);
			master1.AddHelper(*actionHelper);
			master1.AddHelper(*actionListIfHelper);
			master1.AddHelper(*numberHelper);
			master1.AddHelper(*stringHelper);
			master1.AddHelper(*vectorHelper);
			master1.AddHelper(*matrixHelper);

			string xmlToParse("<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Integer Name=\"Int\" Value=\"5\"/><If InstanceName=\"If1\"><Condition Name=\"Int\"/><Then ClassName=\"TestAction\"/><Else ClassName=\"TestAction\"/></If></Entity></Sector></World>");
			master1.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Assert::IsTrue(actionListIfHelper->mStartElementHandlerCount == 0);
			Assert::IsTrue(actionListIfHelper->mEndElementHandlerCount == 0);

			WorldSharedData* sharedData = new WorldSharedData();
			XmlParseMaster master(sharedData);
			master.AddHelper(*worldHelper);
			master.AddHelper(*sectorHelper);
			master.AddHelper(*entityHelper);
			master.AddHelper(*actionHelper);
			master.AddHelper(*actionListIfHelper);
			master.AddHelper(*numberHelper);
			master.AddHelper(*stringHelper);
			master.AddHelper(*vectorHelper);
			master.AddHelper(*matrixHelper);

			EntityFactory fact;
			TestActionFactory testActionFact;
			ActionListFactory actionListFact;
			ActionListIfFactory actionListIfFact;

			// not nested
			xmlToParse = "<If InstanceName=\"If1\"><Condition Name=\"Int\"/><Then ClassName=\"TestAction\"/><Else ClassName=\"TestAction\"/></If>";
			auto func0 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func0);

			// no names
			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Integer Name=\"Int\" Value=\"5\"/><If Instance=\"If1\"><Condition Name=\"Int\"/><Then ClassName=\"TestAction\"/><Else ClassName=\"TestAction\"/></If></Entity></Sector></World>";
			auto func1 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func1);
			// to actually delete the world
			sharedData->SetScope(*sharedData->GetScope()->As<Entity>()->GetSector()->GetWorld());

			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"></Entity><Integer Name=\"Int\" Value=\"5\"/><If InstanceName=\"If1\"><Condition NotName=\"Int\"/><Then ClassName=\"TestAction\"/><Else ClassName=\"TestAction\"/></If></Sector></World>";
			auto func2 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func2);
			// to actually delete the world
			sharedData->SetScope(*sharedData->GetScope()->As<Action>()->GetPredecessor()->As<Sector>()->GetWorld());

			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"></Entity></Sector><Integer Name=\"Int\" Value=\"5\"/><If InstanceName=\"If1\"><Condition Name=\"Int\"/><Then NotClassName=\"TestAction\"/><Else ClassName=\"TestAction\"/></If></World>";
			auto func3 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func3);
			// to actually delete the world
			sharedData->SetScope(*sharedData->GetScope()->As<Action>()->GetPredecessor());

			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Integer Name=\"Int\" Value=\"5\"/><If InstanceName=\"If1\"><Condition Name=\"Int\"/><Then ClassName=\"TestAction\"/><Else NotClassName=\"TestAction\"/></If></Entity></Sector></World>";
			auto func4 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func4);
			// to actually delete the world
			sharedData->SetScope(*sharedData->GetScope()->As<Action>()->GetPredecessor()->As<Entity>()->GetSector()->GetWorld());

			// simple if action
			xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Integer Name=\"Int\" Value=\"5\"/><If InstanceName=\"If1\"><Condition Name=\"Int\"/><Then ClassName=\"TestAction\"/><Else ClassName=\"TestAction\"/></If></Entity></Sector></World>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			Scope& ent = sharedData->GetScope()->As<World>()->Sectors().operator[](0).As<Sector>()->Entities().operator[](0);
			Scope& action = ent.As<Entity>()->Actions()[0];
			Assert::IsTrue(action.As<Action>()->Name() == "If1");
			Assert::IsTrue(action.Is(ActionListIf::TypeIdClass()));
			Assert::IsTrue(action.As<ActionListIf>()->GetConditionIntegerName() == "Int");
			Assert::IsTrue(action.As<ActionListIf>()->Actions()[0].Is(TestAction::TypeIdClass()));
			Assert::IsTrue(action.As<ActionListIf>()->Actions()[0].As<TestAction>()->Name() == "ThenAction");
			Assert::IsTrue(action.As<ActionListIf>()->Actions()[1].Is(TestAction::TypeIdClass()));
			Assert::IsTrue(action.As<ActionListIf>()->Actions()[1].As<TestAction>()->Name() == "ElseAction");

			WorldState world_state;
			sharedData->GetScope()->As<World>()->Update(world_state);
			Assert::IsTrue(action.As<ActionListIf>()->Actions()[0].As<TestAction>()->GetNumber() == 1);
			Assert::IsTrue(action.As<ActionListIf>()->Actions()[1].As<TestAction>()->GetNumber() == 0);

			// clone helper
			XmlParseMaster* cloneMaster = master.Clone();
			cloneMaster->Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			auto& cloneSharedData = cloneMaster->GetSharedData();
			Assert::IsTrue(cloneSharedData.Is(WorldSharedData::TypeIdClass()));
			WorldSharedData& worldCloneSharedData = static_cast<WorldSharedData&>(cloneSharedData);
			Assert::IsNotNull(worldCloneSharedData.GetScope());
			Assert::IsTrue(worldCloneSharedData.GetScope()->Is(World::TypeIdClass()));
			Assert::IsFalse(worldCloneSharedData.GetScope()->As<World>()->Sectors().IsEmpty());
			Scope& cloneWorld = *worldCloneSharedData.GetScope();
			Scope& ent1 = cloneWorld.As<World>()->Sectors().operator[](0).As<Sector>()->Entities().operator[](0);
			Scope& action1 = ent1.As<Entity>()->Actions()[0];
			Assert::IsTrue(action1.As<Action>()->Name() == "If1");
			Assert::IsTrue(action1.Is(ActionListIf::TypeIdClass()));
			Assert::IsTrue(action1.As<ActionListIf>()->GetConditionIntegerName() == "Int");
			Assert::IsTrue(action1.As<ActionListIf>()->Actions()[0].Is(TestAction::TypeIdClass()));
			Assert::IsTrue(action1.As<ActionListIf>()->Actions()[0].As<TestAction>()->Name() == "ThenAction");
			Assert::IsTrue(action1.As<ActionListIf>()->Actions()[1].Is(TestAction::TypeIdClass()));
			Assert::IsTrue(action1.As<ActionListIf>()->Actions()[1].As<TestAction>()->Name() == "ElseAction");

			delete cloneMaster;
			delete worldHelper;
			delete sectorHelper;
			delete entityHelper;
			delete actionHelper;
			delete actionListIfHelper;
			delete numberHelper;
			delete stringHelper;
			delete vectorHelper;
			delete matrixHelper;
			delete sharedData;
			delete testSharedData;
		}

		/************************************************************************/
		TEST_METHOD(TableSharedDataRTTITest)
		{
			RTTI* sharedData = new WorldSharedData();

			Assert::IsNotNull(sharedData->As<WorldSharedData>());
			Assert::IsNotNull(sharedData->As<TableSharedData>());
			Assert::IsNull(sharedData->As<Entity>());

			Assert::IsTrue(sharedData->Is("WorldSharedData"));
			Assert::IsTrue(sharedData->Is(sharedData->TypeIdInstance()));
			Assert::IsTrue(sharedData->Is(sharedData->As<WorldSharedData>()->TypeIdClass()));
			Assert::IsTrue(sharedData->As<WorldSharedData>()->TypeName() == "WorldSharedData");

			Assert::IsTrue(sharedData->QueryInterface(sharedData->TypeIdInstance())->ToString() == "RTTI");
			Assert::IsNotNull(sharedData->As<WorldSharedData>());

			Assert::IsTrue(sharedData->As<WorldSharedData>()->Equals(sharedData));

			delete sharedData;
		}

		/************************************************************************/
		TEST_METHOD(XmlParseHelpersRTTITest)
		{
			HelperRTTITestTemplate<XmlParseHelperWorld>("XmlParseHelperWorld");
			HelperRTTITestTemplate<XmlParseHelperSector>("XmlParseHelperSector");
			HelperRTTITestTemplate<XmlParseHelperEntity>("XmlParseHelperEntity");
			HelperRTTITestTemplate<XmlParseHelperAction>("XmlParseHelperAction");
			HelperRTTITestTemplate<XmlParseHelperActionIf>("XmlParseHelperActionIf");
		}

#pragma endregion 

#pragma endregion 

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState WorldTest::sStartMemState;
}