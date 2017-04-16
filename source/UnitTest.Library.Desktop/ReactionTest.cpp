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
			unique_ptr<Bar> bar = make_unique<Bar>();
			shared_ptr<Event<Bar>> barEvent = make_shared<Event<Bar>>(*bar);

			unique_ptr<EventMessageAttributed> eventMsg = make_unique<EventMessageAttributed>();
			shared_ptr<Event<EventMessageAttributed>> event = make_shared<Event<EventMessageAttributed>>(*eventMsg);
			unique_ptr<ReactionAttributed> reactionAttr = make_unique<ReactionAttributed>("Type0");

			Assert::IsTrue(reactionAttr->GetSubType() == "Type0");

			barEvent->Message().SetFloat(2.0f);
			reactionAttr->Notify(*barEvent);
			Assert::IsFalse(reactionAttr->GetIsNotified());

			auto& message = event->Message();
			message.SetSubType("Type1");
			reactionAttr->Notify(*event);
			Assert::IsFalse(reactionAttr->GetIsNotified());

			auto& dat = message.AppendAuxiliaryAttribute("Float");
			dat.PushBack(5.6f);

			reactionAttr->SetSubType("Type1");
			reactionAttr->Notify(*event);
			Assert::IsFalse(reactionAttr->GetIsNotified());
			Assert::IsTrue(reactionAttr->IsAuxiliaryAttribute("Float"));
			Assert::IsTrue(reactionAttr->operator[]("Float") == 5.6f);

			shared_ptr<World> world = make_shared<World>();
			shared_ptr<WorldState> worldState = make_shared<WorldState>();
			worldState->SetWorld(world.get());
			message.SetWorldState(*worldState);
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
			Assert::IsNotNull(reaction->As<Attributed>());
			Assert::IsNull(reaction->As<EventMessageAttributed>());

			Assert::IsTrue(reaction->Is("ReactionAttributed"));
			Assert::IsTrue(reaction->Is(reaction->TypeIdInstance()));
			Assert::IsTrue(reaction->Is(reaction->As<ReactionAttributed>()->TypeIdClass()));
			Assert::IsTrue(reaction->As<ReactionAttributed>()->TypeName() == "ReactionAttributed");

			Assert::IsTrue(reaction->QueryInterface(reaction->TypeIdInstance())->ToString() != "RTTI");
			Assert::IsNotNull(reaction->As<ReactionAttributed>());

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

#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState ReactionTest::sStartMemState;
}