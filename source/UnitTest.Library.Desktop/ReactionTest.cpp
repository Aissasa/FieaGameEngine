#include "pch.h"
#include "CppUnitTest.h"
#include "Action.h"
#include "World.h"
#include "Entity.h"
#include "Sector.h"
#include "Reaction.h"
#include "EventMessageAttributed.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "Bar.h"
#include "ActionEvent.h"
#include "XmlParseHelperWorld.h"
#include "XmlParseHelperSector.h"
#include "XmlParseHelperAction.h"
#include "XmlParseHelperNumber.h"
#include "XmlParseHelperEntity.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace chrono;
using namespace Library;

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(ReactionTest)
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
			Action::ClearCashedPrescribedAttributes();
			Reaction::ClearCashedPrescribedAttributes();
			Event<EventMessageAttributed>::UnsubscribeAll();

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

		/************************************************************************/
		TEST_METHOD(EventMessageTest)
		{
			unique_ptr<EventMessageAttributed> event = make_unique<EventMessageAttributed>();
			Assert::IsTrue(event->GetSubType() == "");
			Assert::IsNull(event->GetWorldState());
			Assert::IsTrue(event->IsPrescribedAttribute("SubType"));
			Assert::IsTrue(event->operator[]("SubType") == "");

			shared_ptr<WorldState> state = make_shared<WorldState>();
			unique_ptr<World> world = make_unique<World>();
			state->SetWorld(world.get());
			event->SetSubType("Test");
			event->SetWorldState(*state);
			Assert::IsTrue(event->GetSubType() == "Test");
			Assert::IsTrue(event->GetWorldState() == state.get());
			Assert::IsTrue(event->operator[]("SubType") == "Test");
		}

		/************************************************************************/
		TEST_METHOD(EventMessageRTTITest)
		{
			unique_ptr<RTTI> eventMsg = make_unique<EventMessageAttributed>();

			Assert::IsNotNull(eventMsg->As<EventMessageAttributed>());
			Assert::IsNotNull(eventMsg->As<Attributed>());
			Assert::IsNull(eventMsg->As<Action>());

			Assert::IsTrue(eventMsg->Is("EventMessageAttributed"));
			Assert::IsTrue(eventMsg->Is(eventMsg->TypeIdInstance()));
			Assert::IsTrue(eventMsg->Is(eventMsg->As<EventMessageAttributed>()->TypeIdClass()));
			Assert::IsTrue(eventMsg->As<EventMessageAttributed>()->TypeName() == "EventMessageAttributed");

			Assert::IsTrue(eventMsg->QueryInterface(eventMsg->TypeIdInstance())->ToString() != "RTTI");
			Assert::IsNotNull(eventMsg->As<EventMessageAttributed>());

			Assert::IsTrue(eventMsg->As<EventMessageAttributed>()->Equals(eventMsg.get()));
		}

		/************************************************************************/
		TEST_METHOD(ReactionAttributedTest)
		{
			unique_ptr<Bar> bar = make_unique<Bar>(2.0f);
			shared_ptr<Event<Bar>> barEvent = make_shared<Event<Bar>>(*bar);

			unique_ptr<EventMessageAttributed> eventMsg = make_unique<EventMessageAttributed>();
			eventMsg->SetSubType("Type1");
			auto& dat = eventMsg->AppendAuxiliaryAttribute("Float");
			dat.PushBack(5.6f);
			shared_ptr<WorldState> worldState = make_shared<WorldState>();
			eventMsg->SetWorldState(*worldState);

			shared_ptr<Event<EventMessageAttributed>> event = make_shared<Event<EventMessageAttributed>>(*eventMsg);
			unique_ptr<ReactionAttributed> reactionAttr = make_unique<ReactionAttributed>("Type0");

			Assert::IsTrue(reactionAttr->GetSubType() == "Type0");

			reactionAttr->Notify(*barEvent);
			Assert::IsFalse(reactionAttr->GetIsNotified());

			reactionAttr->Notify(*event);
			Assert::IsFalse(reactionAttr->GetIsNotified());

			reactionAttr->SetSubType("Type1");
			reactionAttr->Notify(*event);
			Assert::IsTrue(reactionAttr->GetIsNotified());
			Assert::IsTrue(reactionAttr->IsAuxiliaryAttribute("Float"));
			Assert::IsTrue(reactionAttr->operator[]("Float") == 5.6f);

			shared_ptr<World> world = make_shared<World>();
			worldState->SetWorld(world.get());
			reactionAttr->Notify(*event);
			Assert::IsTrue(reactionAttr->GetIsNotified());

			reactionAttr->Reset();
			event->Deliver();
			Assert::IsTrue(reactionAttr->GetIsNotified());
		}

		/************************************************************************/
		TEST_METHOD(ReactionAttributedRTTITest)
		{
			unique_ptr<RTTI> reaction = make_unique<ReactionAttributed>();

			Assert::IsNotNull(reaction->As<ReactionAttributed>());
			Assert::IsNotNull(reaction->As<Reaction>());
			Assert::IsNotNull(reaction->As<Attributed>());
			Assert::IsNull(reaction->As<EventMessageAttributed>());

			Assert::IsTrue(reaction->Is("ReactionAttributed"));
			Assert::IsTrue(reaction->Is("Reaction"));
			Assert::IsTrue(reaction->Is(reaction->TypeIdInstance()));
			Assert::IsTrue(reaction->Is(reaction->As<ReactionAttributed>()->TypeIdClass()));
			Assert::IsTrue(reaction->Is(reaction->As<Reaction>()->TypeIdClass()));
			Assert::IsTrue(reaction->Is(ReactionAttributed::TypeIdClass()));
			Assert::IsTrue(reaction->Is(Reaction::TypeIdClass()));

			Assert::IsTrue(reaction->As<ReactionAttributed>()->TypeName() == "ReactionAttributed");
			Assert::IsTrue(reaction->As<Reaction>()->TypeName() == "Reaction");

			Assert::IsTrue(reaction->QueryInterface(reaction->TypeIdInstance())->ToString() != "RTTI");
			Assert::IsNotNull(reaction->As<ReactionAttributed>());
			Assert::IsNotNull(reaction->As<Reaction>());

			Assert::IsTrue(reaction->As<ReactionAttributed>()->Equals(reaction.get()));
		}

		/************************************************************************/
		TEST_METHOD(ActionEventTest)
		{
			unique_ptr<ReactionAttributed> reactionAttr = make_unique<ReactionAttributed>("Type");
			unique_ptr<ActionEvent> actionEvent = make_unique<ActionEvent>();

			Assert::IsTrue(actionEvent->GetSubType() == "");
			Assert::AreEqual(actionEvent->GetDelay(), 0);

			actionEvent->SetSubType("Type");
			actionEvent->SetDelay(50);
			auto& dat1 = actionEvent->AppendAuxiliaryAttribute("Float");
			dat1.PushBack(5.4f);
			auto& dat2 = actionEvent->AppendAuxiliaryAttribute("Integer");
			dat2.PushBack(9);

			shared_ptr<WorldState> worldState = make_shared<WorldState>();

			actionEvent->Update(*worldState);
			Assert::IsFalse(reactionAttr->GetIsNotified());

			shared_ptr<World> world = make_shared<World>();
			worldState->SetWorld(world.get());

			shared_ptr<GameTime> gameTime = make_shared<GameTime>();
			high_resolution_clock::time_point startPoint = high_resolution_clock::now();
			gameTime->SetCurrentTime(startPoint);
			worldState->SetGameTime(*gameTime);

			actionEvent->Update(*worldState);
			Assert::IsFalse(reactionAttr->GetIsNotified());
			worldState->GetGameTime().SetCurrentTime(startPoint + milliseconds(30000));

			world->Update(*worldState);
			Assert::IsTrue(reactionAttr->GetIsNotified());
		}

		/************************************************************************/
		TEST_METHOD(ActionEventRTTITest)
		{
			unique_ptr<RTTI> action = make_unique<ActionEvent>();

			Assert::IsNotNull(action->As<ActionEvent>());
			Assert::IsNotNull(action->As<Attributed>());
			Assert::IsNull(action->As<EventMessageAttributed>());

			Assert::IsTrue(action->Is("ActionEvent"));
			Assert::IsTrue(action->Is(action->TypeIdInstance()));
			Assert::IsTrue(action->Is(action->As<ActionEvent>()->TypeIdClass()));
			Assert::IsTrue(action->As<ActionEvent>()->TypeName() == "ActionEvent");

			Assert::IsTrue(action->QueryInterface(action->TypeIdInstance())->ToString() != "RTTI");
			Assert::IsNotNull(action->As<ActionEvent>());

			Assert::IsTrue(action->As<ActionEvent>()->Equals(action.get()));
		}

		/************************************************************************/
		TEST_METHOD(EventRTTITest)
		{
			unique_ptr<EventMessageAttributed> eventMessage = make_unique<EventMessageAttributed>();
			unique_ptr<RTTI> event = make_unique<Event<EventMessageAttributed>>(*eventMessage);

			Assert::IsNotNull(event->As<Event<EventMessageAttributed>>());
			Assert::IsNotNull(event->As<EventPublisher>());
			Assert::IsNull(event->As<TableSharedData>());

			Assert::IsTrue(event->Is(event->TypeIdInstance()));
			Assert::IsTrue(event->Is(Event<EventMessageAttributed>::TypeIdClass()));
			Assert::IsTrue(event->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(event->Is(event->As<EventPublisher>()->TypeIdClass()));

			Assert::IsTrue(event->QueryInterface(event->TypeIdInstance())->ToString() == "RTTI");
			Assert::IsNotNull(event->As<Event<EventMessageAttributed>>());

			Assert::IsTrue(event->As<Event<EventMessageAttributed>>()->Equals(event.get()));

		}

		/************************************************************************/
		TEST_METHOD(XmlParsingTest)
		{
			XmlParseHelperWorld* worldHelper = new XmlParseHelperWorld();
			XmlParseHelperSector* sectorHelper = new XmlParseHelperSector();
			XmlParseHelperEntity* entityHelper = new XmlParseHelperEntity();
			XmlParseHelperAction* actionHelper = new XmlParseHelperAction();
			XmlParseHelperNumber* numberHelper = new XmlParseHelperNumber();

			WorldSharedData* sharedData = new WorldSharedData();
			XmlParseMaster master(sharedData);
			master.AddHelper(*worldHelper);
			master.AddHelper(*sectorHelper);
			master.AddHelper(*entityHelper);
			master.AddHelper(*actionHelper);
			master.AddHelper(*numberHelper);

			EntityFactory entityFactory;
			ActionEventFactory actionEventFactory;

			// no names
			string xmlToParse = "<World Name=\"Lvl1\"><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/><ActionEvent NonInstanceName=\"ActionEvent\"/></Entity></Sector></World>";
			auto func0 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func0);
			// to actually delete the world
			sharedData->SetScope(*sharedData->GetScope()->As<Entity>()->GetSector()->GetWorld());

			xmlToParse = "<World Name=\"Lvl1\"><Reaction NonClass=\"ReactionAttributed\" InstanceName=\"Reaction\" Subtype=\"Type\" /><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/></Entity></Sector></World>";
			auto func1 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func1);

			xmlToParse = "<World Name=\"Lvl1\"><Reaction Class=\"ReactionAttributed\" NonInstanceName=\"Reaction\" Subtype=\"Type\" /><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/></Entity></Sector></World>";
			auto func2 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func2);

			// fact
			xmlToParse = "<World Name=\"Lvl1\"><Reaction Class=\"ReactionAttributed\" InstanceName=\"Reaction\" Subtype=\"Type\" /><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Float Name=\"Fl\" Value=\"5.6f\"/></Entity></Sector></World>";
			auto func3 = [&master, &xmlToParse] { master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length())); };
			Assert::ExpectException<exception>(func3);

			ReactionAttributedFactory reactionAttributedFactory;

			// normal case
			xmlToParse = "<World Name=\"Lvl1\"><ActionEvent InstanceName=\"ActionEvent\" Subtype=\"Type\" Delay=\"50\"><Float Name=\"Fl\" Value=\"5.6f\"/></ActionEvent><Sector Name=\"Sec1\"><Entity Class=\"Entity\" InstanceName=\"Ent\"><Reaction Class=\"ReactionAttributed\" InstanceName=\"Reaction\" Subtype=\"Type\" /></Entity></Sector></World>";
			master.Parse(xmlToParse.c_str(), static_cast<uint32_t>(xmlToParse.length()));
			
			World* world = sharedData->GetScope()->As<World>();
			Scope& action = world->Actions()[0];
			Assert::IsTrue(action.Is(ActionEvent::TypeIdClass()));
			ActionEvent& actionEvent = static_cast<ActionEvent&>(action);
			Assert::IsTrue(actionEvent.GetSubType() == "Type");
			Assert::AreEqual(actionEvent.GetDelay(), 50);
			Assert::IsTrue(actionEvent.IsAuxiliaryAttribute("Fl"));
			Assert::IsTrue(actionEvent.Find("Fl")->Get<float>(0) == 5.6f);

			Scope& scope = world->Sectors().operator[](0).As<Sector>()->Entities().operator[](0);
			Assert::IsTrue(scope.Is(Entity::TypeIdClass()));
			Entity& entity = static_cast<Entity&>(scope);
			Assert::IsTrue(entity.IsAuxiliaryAttribute("Reaction"));
			auto& scope1 = entity.Find("Reaction")[0][0];
			Assert::IsTrue(scope1.Is(ReactionAttributed::TypeIdClass()));
			ReactionAttributed& reactionAttributed = static_cast<ReactionAttributed&>(scope1);
			Assert::IsTrue(reactionAttributed.GetSubType() == "Type");

			shared_ptr<WorldState> worldState = make_shared<WorldState>();
			worldState->SetWorld(world);
			shared_ptr<GameTime> gameTime = make_shared<GameTime>();
			high_resolution_clock::time_point startPoint = high_resolution_clock::now();
			gameTime->SetCurrentTime(startPoint);
			worldState->SetGameTime(*gameTime);

			actionEvent.Update(*worldState);
			Assert::IsFalse(reactionAttributed.GetIsNotified());
			worldState->GetGameTime().SetCurrentTime(startPoint + milliseconds(30000));

			world->Update(*worldState);
			Assert::IsTrue(reactionAttributed.GetIsNotified());


			delete worldHelper;
			delete sectorHelper;
			delete entityHelper;
			delete actionHelper;
			delete numberHelper;
			delete sharedData;
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState ReactionTest::sStartMemState;
}