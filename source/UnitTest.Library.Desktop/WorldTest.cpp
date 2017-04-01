#include "pch.h"
#include "CppUnitTest.h"
#include "World.h"
#include "WorldState.h"
#include "Sector.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "TestEntity.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
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

#pragma region TestMethods

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

#pragma endregion 

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState WorldTest::sStartMemState;
}