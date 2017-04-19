#include "pch.h"
#include "CppUnitTest.h"
#include "BarEventSubscriber.h"
#include "Bar.h"
#include "Event.h"
#include "EventQueue.h"
#include "TableSharedData.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace std;
using namespace std::chrono;

namespace UnitTestLibraryDesktop
{		
	TEST_CLASS(EventSystemTest)
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
			Event<Bar>::UnsubscribeAll();

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
		TEST_METHOD(SubscriberTest)
		{
			unique_ptr<Bar> bar = make_unique<Bar>(2.0f);
			unique_ptr<Event<Bar>> barEvent = make_unique<Event<Bar>>(*bar);
			unique_ptr<BarEventSubscriber> barEventSubscriber = make_unique<BarEventSubscriber>();

			Assert::AreEqual(bar->GetFloat(), 2.0f);
			Assert::AreEqual(barEvent->Message().GetFloat(), 2.0f);
			Assert::AreEqual(barEventSubscriber->GetFloat(), 0.0f);

			barEventSubscriber->Notify(*barEvent);
			Assert::AreEqual(bar->GetFloat(), 2.0f);
			Assert::AreEqual(barEvent->Message().GetFloat(), 2.0f);
			Assert::AreEqual(barEventSubscriber->GetFloat(), 2.0f);

			barEventSubscriber->Notify(*barEvent);
			Assert::AreEqual(barEventSubscriber->GetFloat(), 2.0f);

			barEventSubscriber->Reset();
			barEventSubscriber->Notify(*barEvent);
			Assert::AreEqual(barEventSubscriber->GetFloat(), 4.0f);
		}

		/************************************************************************/
		TEST_METHOD(EventBarTest)
		{
			unique_ptr<Bar> bar1 = make_unique<Bar>(1.0f);
			unique_ptr<Bar> bar2 = make_unique<Bar>(2.0f);
			unique_ptr<Event<Bar>> barEvent1 = make_unique<Event<Bar>>(*bar1);
			unique_ptr<Event<Bar>> barEvent2 = make_unique<Event<Bar>>(*bar2);
			unique_ptr<BarEventSubscriber> barEventSubscriber1 = make_unique<BarEventSubscriber>();
			unique_ptr<BarEventSubscriber> barEventSubscriber2 = make_unique<BarEventSubscriber>();

			barEventSubscriber1->SetFloat(1);
			barEventSubscriber2->SetFloat(5);

			Event<Bar>::Subscribe(*barEventSubscriber1);
			Event<Bar>::Subscribe(*barEventSubscriber2);

			barEvent1->Deliver();
			Assert::AreEqual(barEvent1->Message().GetFloat(), 1.0f);
			Assert::AreEqual(barEventSubscriber1->GetFloat(), 2.0f);
			Assert::AreEqual(barEventSubscriber2->GetFloat(), 6.0f);

			barEventSubscriber1->Reset();
			barEventSubscriber2->Reset();

			barEvent2->Deliver();
			Assert::AreEqual(barEvent2->Message().GetFloat(), 2.0f);
			Assert::AreEqual(barEventSubscriber1->GetFloat(), 4.0f);
			Assert::AreEqual(barEventSubscriber2->GetFloat(), 8.0f);

			barEventSubscriber1->Reset();
			barEventSubscriber2->Reset();
			Event<Bar>::Unsubscribe(*barEventSubscriber2);

			barEvent1->Deliver();
			Assert::AreEqual(barEventSubscriber1->GetFloat(), 5.0f);
			Assert::AreEqual(barEventSubscriber2->GetFloat(), 8.0f);

			barEventSubscriber1->Reset();
			barEventSubscriber2->Reset();
			Event<Bar>::UnsubscribeAll();

			barEvent2->Deliver();
			Assert::AreEqual(barEventSubscriber1->GetFloat(), 5.0f);
			Assert::AreEqual(barEventSubscriber2->GetFloat(), 8.0f);

			Event<Bar>::Subscribe(*barEventSubscriber2);
			barEvent1->Deliver();
			barEventSubscriber1->Reset();
			barEventSubscriber2->Reset();
			barEvent2->Deliver();
			Assert::AreEqual(barEventSubscriber1->GetFloat(), 5.0f);
			Assert::AreEqual(barEventSubscriber2->GetFloat(), 11.0f);		
		}

		/************************************************************************/
		TEST_METHOD(EventQueueTest)
		{
			unique_ptr<Bar> bar1 = make_unique<Bar>(1.0f);
			unique_ptr<Bar> bar2 = make_unique<Bar>(2.0f);
			unique_ptr<Bar> bar3 = make_unique<Bar>(3.0f);
			shared_ptr<Event<Bar>> barEvent1 = make_shared<Event<Bar>>(*bar1);
			shared_ptr<Event<Bar>> barEvent2 = make_shared<Event<Bar>>(*bar2);
			shared_ptr<Event<Bar>> barEvent3 = make_shared<Event<Bar>>(*bar3);
			unique_ptr<BarEventSubscriber> barEventSubscriber1 = make_unique<BarEventSubscriber>();
			unique_ptr<BarEventSubscriber> barEventSubscriber2 = make_unique<BarEventSubscriber>();
			unique_ptr<EventQueue> eventQueue = make_unique<EventQueue>();

			barEventSubscriber1->SetFloat(1);
			barEventSubscriber2->SetFloat(5);

			Event<Bar>::Subscribe(*barEventSubscriber1);
			Event<Bar>::Subscribe(*barEventSubscriber2);

			unique_ptr<GameTime> gameTime = make_unique<GameTime>();
			high_resolution_clock::time_point startPoint = high_resolution_clock::now();
			gameTime->SetCurrentTime(startPoint);

			eventQueue->Enqueue(barEvent1, *gameTime);
			eventQueue->Enqueue(barEvent2, *gameTime, milliseconds(200));
			Assert::IsFalse(eventQueue->IsEmpty());
			Assert::IsTrue(eventQueue->Size() == 2u);

			eventQueue->Send(barEvent3);
			Assert::IsTrue(barEventSubscriber1->GetFloat() == 1.0f);
			Assert::IsTrue(barEventSubscriber2->GetFloat() == 5.0f);
			Assert::IsTrue(eventQueue->Size() == 2u);

			eventQueue->Enqueue(barEvent3, *gameTime, milliseconds(3000));

			eventQueue->Send(barEvent2);
			Assert::AreEqual(barEventSubscriber1->GetFloat(), 3.0f);
			Assert::AreEqual(barEventSubscriber2->GetFloat(), 7.0f);
			Assert::AreEqual(eventQueue->Size(), 2u);

			barEventSubscriber1->Reset();
			barEventSubscriber2->Reset();

			eventQueue->Enqueue(barEvent2, *gameTime, milliseconds(2500));
			gameTime->SetCurrentTime(startPoint + milliseconds(50));

			eventQueue->Update(*gameTime);
			Assert::AreEqual(barEventSubscriber1->GetFloat(), 4.0f);
			Assert::AreEqual(barEventSubscriber2->GetFloat(), 8.0f);
			Assert::AreEqual(eventQueue->Size(), 2u);

			barEventSubscriber1->Reset();
			barEventSubscriber2->Reset();

			barEvent2->SetTime(startPoint, milliseconds(2600));
			Assert::IsTrue(barEvent2->Delay() == milliseconds(2600));
			Assert::IsTrue(barEvent2->TimeEnqueued() == startPoint);

			gameTime->SetCurrentTime(startPoint + milliseconds(2605));
			eventQueue->Update(*gameTime);
			Assert::IsTrue(barEventSubscriber1->GetFloat() == 6.0f);
			Assert::IsTrue(barEventSubscriber2->GetFloat() == 10.0f);
			Assert::IsTrue(eventQueue->Size() == 1u);

			barEventSubscriber1->Reset();
			barEventSubscriber2->Reset();

			eventQueue->Clear();
			gameTime->SetCurrentTime(startPoint + milliseconds(3500));
			eventQueue->Update(*gameTime);
			Assert::IsTrue(barEventSubscriber1->GetFloat() == 6.0f);
			Assert::IsTrue(barEventSubscriber2->GetFloat() == 10.0f);
			Assert::IsTrue(eventQueue->Size() == 0u);
		}

		/************************************************************************/
		TEST_METHOD(EventMoveSemanticsTest)
		{
			unique_ptr<Bar> bar1 = make_unique<Bar>(1.0f);
			unique_ptr<Bar> bar2 = make_unique<Bar>(2.0f);
			unique_ptr<Event<Bar>> barEvent1 = make_unique<Event<Bar>>(*bar1);
			unique_ptr<Event<Bar>> barEvent2 = make_unique<Event<Bar>>(*bar2);
			unique_ptr<Event<Bar>> barEvent3 = make_unique<Event<Bar>>(*bar2);
			unique_ptr<BarEventSubscriber> barEventSubscriber1 = make_unique<BarEventSubscriber>();
			unique_ptr<BarEventSubscriber> barEventSubscriber2 = make_unique<BarEventSubscriber>();

			barEventSubscriber1->SetFloat(1);
			barEventSubscriber2->SetFloat(5);

			Event<Bar>::Subscribe(*barEventSubscriber1);
			Event<Bar>::Subscribe(*barEventSubscriber2);

			*barEvent3 = *barEvent1;
			Assert::IsTrue(barEvent3->Message().GetFloat() == 1.0f);

			unique_ptr<Event<Bar>> barEvent4 = make_unique<Event<Bar>>(*barEvent2);
			Assert::IsTrue(barEvent4->Message().GetFloat() == 2.0f);

			unique_ptr<Event<Bar>> barEvent5 = make_unique<Event<Bar>>(move(*barEvent1));
			Assert::IsTrue(barEvent5->Message().GetFloat() == 1.0f);

			unique_ptr<Event<Bar>> barEvent6 = make_unique<Event<Bar>>(*bar1);
			*barEvent6 = move(*barEvent2);
			Assert::IsTrue(barEvent6->Message().GetFloat() == 2.0f);
		}

		/************************************************************************/
		TEST_METHOD(EventRTTITest)
		{
			unique_ptr<Bar> bar = make_unique<Bar>();
			unique_ptr<RTTI> barEvent = make_unique<Event<Bar>>(*bar);

			Assert::IsNotNull(barEvent->As<Event<Bar>>());
			Assert::IsNotNull(barEvent->As<EventPublisher>());
			Assert::IsNull(barEvent->As<TableSharedData>());

			Assert::IsTrue(barEvent->Is(barEvent->TypeIdInstance()));
			Assert::IsTrue(barEvent->Is(Event<Bar>::TypeIdClass()));
			Assert::IsTrue(barEvent->Is(EventPublisher::TypeIdClass()));
			Assert::IsTrue(barEvent->Is(barEvent->As<EventPublisher>()->TypeIdClass()));

			Assert::IsTrue(barEvent->QueryInterface(barEvent->TypeIdInstance())->ToString() == "RTTI");
			Assert::IsNotNull(barEvent->As<Event<Bar>>());

			Assert::IsTrue(barEvent->As<Event<Bar>>()->Equals(barEvent.get()));
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState EventSystemTest::sStartMemState;
}