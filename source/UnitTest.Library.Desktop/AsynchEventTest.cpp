#include "pch.h"
#include "CppUnitTest.h"
#include "Event.h"
#include "Bar.h"
#include "BarEventSubscriber.h"
#include "EventSubscriberSub.h"
#include "EventUnsubscriber.h"
#include "AsynchQueueTester.h"
#include "Foo.h"
#include "Product1.h"
#include "EventSubExceptionThrower.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AsynchEventTest)
	{

	public:

#pragma region Memory leak check

		TEST_METHOD_INITIALIZE(Initialize)
		{
			Event<Bar>::Init(10000);
			Event<Product1>::Init(10000);

			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Event<Bar>::UnsubscribeAll();
			Event<Product1>::UnsubscribeAll();

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
		TEST_METHOD(EventSubscriberSubTest)
		{
			unique_ptr<Bar> bar1 = make_unique<Bar>(1.0f);
			unique_ptr<Event<Bar>> barEvent1 = make_unique<Event<Bar>>(*bar1);

			Vector<shared_ptr<EventSubscriberSub>> vector1;
			Vector<shared_ptr<BarEventSubscriber>> vector2;
			for (int i = 0; i < 3000; ++i)
			{
				vector1.PushBack(make_shared<EventSubscriberSub>());
				Event<Bar>::Subscribe(*vector1.Back());
				vector2.PushBack(make_shared<BarEventSubscriber>());
				Event<Bar>::Subscribe(*vector2.Back());
			}

			for (auto& eventSubscriberSub : vector1)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : vector2)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			barEvent1->Deliver();

			for (auto& eventSubscriberSub : vector1)
			{
				Assert::IsTrue(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : vector2)
			{
				Assert::IsTrue(eventSubscriberSub->GetIsNotified());
			}

			barEvent1->Deliver();
		}

		/************************************************************************/
		TEST_METHOD(EventUnsubscriberTest)
		{
			unique_ptr<Bar> bar1 = make_unique<Bar>(1.0f);
			unique_ptr<Event<Bar>> barEvent1 = make_unique<Event<Bar>>(*bar1);

			Vector<shared_ptr<EventUnsubscriber>> vector1;
			Vector<shared_ptr<BarEventSubscriber>> vector2;
			for (int i = 0; i < 3000; ++i)
			{
				vector1.PushBack(make_shared<EventUnsubscriber>());
				Event<Bar>::Subscribe(*vector1.Back());
				vector2.PushBack(make_shared<BarEventSubscriber>());
				Event<Bar>::Subscribe(*vector2.Back());
			}

			for (auto& eventSubscriberSub : vector1)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : vector2)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			barEvent1->Deliver();

			for (auto& eventSubscriberSub : vector1)
			{
				Assert::IsTrue(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : vector2)
			{
				Assert::IsTrue(eventSubscriberSub->GetIsNotified());
			}

			barEvent1->Deliver();
		}

		/************************************************************************/
		TEST_METHOD(EventSubExceptionThrowerTest)
		{
			unique_ptr<Bar> bar1 = make_unique<Bar>(1.0f);
			unique_ptr<Event<Bar>> barEvent1 = make_unique<Event<Bar>>(*bar1);

			Vector<shared_ptr<EventSubExceptionThrower>> vector1;
			Vector<shared_ptr<BarEventSubscriber>> vector2;
			for (int i = 0; i < 3000; ++i)
			{
				vector1.PushBack(make_shared<EventSubExceptionThrower>());
				Event<Bar>::Subscribe(*vector1.Back());
				vector2.PushBack(make_shared<BarEventSubscriber>());
				Event<Bar>::Subscribe(*vector2.Back());
			}

			for (auto& eventSubscriberSub : vector1)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : vector2)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			auto func = [&barEvent1]() { barEvent1->Deliver(); };
			Assert::ExpectException<runtime_error>(func);

			for (auto& eventSubscriberSub : vector1)
			{
				Assert::IsTrue(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : vector2)
			{
				Assert::IsTrue(eventSubscriberSub->GetIsNotified());
			}
		}

		/************************************************************************/
		TEST_METHOD(EventUnsubscriberSubscriberSubTest)
		{
			unique_ptr<Bar> bar1 = make_unique<Bar>(1.0f);
			unique_ptr<Event<Bar>> barEvent1 = make_unique<Event<Bar>>(*bar1);

			Vector<shared_ptr<EventSubscriberSub>> vector0;
			Vector<shared_ptr<EventUnsubscriber>> vector1;
			Vector<shared_ptr<BarEventSubscriber>> vector2;
			for (int i = 0; i < 2000; ++i)
			{
				vector1.PushBack(make_shared<EventUnsubscriber>());
				Event<Bar>::Subscribe(*vector1.Back());
				vector2.PushBack(make_shared<BarEventSubscriber>());
				Event<Bar>::Subscribe(*vector2.Back());
				vector0.PushBack(make_shared<EventSubscriberSub>());
				Event<Bar>::Subscribe(*vector0.Back());
			}

			for (auto& eventSubscriberSub : vector0)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : vector1)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : vector2)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			barEvent1->Deliver();

			for (auto& eventSubscriberSub : vector0)
			{
				Assert::IsTrue(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : vector1)
			{
				Assert::IsTrue(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : vector2)
			{
				Assert::IsTrue(eventSubscriberSub->GetIsNotified());
			}

			barEvent1->Deliver();
		}

		/************************************************************************/
		TEST_METHOD(EventQueueAddTest)
		{
			// prepare the events
			shared_ptr<EventQueue> queue = make_shared<EventQueue>();
			unique_ptr<Bar> bar1 = make_unique<Bar>(1.0f);
			unique_ptr<Product1> product1 = make_unique<Product1>();

			shared_ptr<GameTime> gameTime = make_shared<GameTime>();
			gameTime->SetCurrentTime(chrono::high_resolution_clock::now());

			Vector<shared_ptr<Event<Bar>>> barEventVect;
			Vector<shared_ptr<Event<Product1>>> prodEventVect;
			for (uint32_t i = 0; i < 100; i++)
			{
				barEventVect.PushBack(make_shared<Event<Bar>>(*bar1));
				queue->Enqueue(barEventVect.Back(), *gameTime);
				prodEventVect.PushBack(make_shared<Event<Product1>>(*product1));
				queue->Enqueue(prodEventVect.Back(), *gameTime);
			}

			// prepare the subs
			Vector<shared_ptr<AsynchQueueTester>> asynchQueueTestersVect;
			Vector<shared_ptr<BarEventSubscriber>> barEventSubsVect;

			for (uint32_t i = 0; i < 3000; ++i)
			{
				if (i % 2 == 0)
				{
					asynchQueueTestersVect.PushBack(make_shared<AsynchQueueTester>(queue, AsynchQueueTester::Type::QueueBar, gameTime));
					Event<Bar>::Subscribe(*asynchQueueTestersVect.Back());
					barEventSubsVect.PushBack(make_shared<BarEventSubscriber>());
					Event<Bar>::Subscribe(*barEventSubsVect.Back());
				}
				else
				{
					asynchQueueTestersVect.PushBack(make_shared<AsynchQueueTester>(queue, AsynchQueueTester::Type::QueueProd, gameTime));
					Event<Product1>::Subscribe(*asynchQueueTestersVect.Back());
				}
			}

			for (auto& eventSubscriberSub : asynchQueueTestersVect)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : barEventSubsVect)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			Assert::AreEqual(200U, queue->Size());

			GameTime gameTimeTemp = *gameTime;
			gameTimeTemp.SetCurrentTime(chrono::high_resolution_clock::now());
			queue->Update(gameTimeTemp);

			for (auto& eventSubscriberSub : barEventSubsVect)
			{
				Assert::IsTrue(eventSubscriberSub->GetIsNotified());
			}

			for (auto& asynchQueueTester : asynchQueueTestersVect)
			{
				Assert::IsTrue(asynchQueueTester->GetIsNotified());
			}

			Assert::AreEqual(3000U, queue->Size());
		}

		/************************************************************************/
		TEST_METHOD(EventQueueClearTest)
		{
			// prepare the events
			shared_ptr<EventQueue> queue = make_shared<EventQueue>();
			unique_ptr<Bar> bar1 = make_unique<Bar>(1.0f);
			unique_ptr<Product1> product1 = make_unique<Product1>();

			shared_ptr<GameTime> gameTime = make_shared<GameTime>();
			gameTime->SetCurrentTime(chrono::high_resolution_clock::now());

			Vector<shared_ptr<Event<Bar>>> barEventVect;
			Vector<shared_ptr<Event<Product1>>> prodEventVect;
			for (uint32_t i = 0; i < 100; i++)
			{
				barEventVect.PushBack(make_shared<Event<Bar>>(*bar1));
				queue->Enqueue(barEventVect.Back(), *gameTime);
				prodEventVect.PushBack(make_shared<Event<Product1>>(*product1));
				queue->Enqueue(prodEventVect.Back(), *gameTime);
			}

			// prepare the subs
			Vector<shared_ptr<AsynchQueueTester>> asynchQueueTestersVect;
			Vector<shared_ptr<BarEventSubscriber>> barEventSubsVect;

			for (uint32_t i = 0; i < 3000; ++i)
			{
				if (i % 2 == 0)
				{
					asynchQueueTestersVect.PushBack(make_shared<AsynchQueueTester>(queue, AsynchQueueTester::Type::QueueBar, gameTime));
					Event<Bar>::Subscribe(*asynchQueueTestersVect.Back());
					barEventSubsVect.PushBack(make_shared<BarEventSubscriber>());
					Event<Bar>::Subscribe(*barEventSubsVect.Back());
				}
				else
				{
					asynchQueueTestersVect.PushBack(make_shared<AsynchQueueTester>(queue, AsynchQueueTester::Type::DequeueAll, gameTime));
					Event<Product1>::Subscribe(*asynchQueueTestersVect.Back());
				}
			}

			for (auto& eventSubscriberSub : asynchQueueTestersVect)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : barEventSubsVect)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			Assert::AreEqual(200U, queue->Size());

			GameTime gameTimeTemp = *gameTime;
			gameTimeTemp.SetCurrentTime(chrono::high_resolution_clock::now());
			queue->Update(gameTimeTemp);

			for (auto& eventSubscriberSub : barEventSubsVect)
			{
				Assert::IsTrue(eventSubscriberSub->GetIsNotified());
			}

			for (auto& asynchQueueTester : asynchQueueTestersVect)
			{
				Assert::IsTrue(asynchQueueTester->GetIsNotified());
			}

			Assert::IsTrue(queue->Size() < 3000U);
		}

		/************************************************************************/
		TEST_METHOD(EventQueueExceptionTest)
		{
			// prepare the events
			shared_ptr<EventQueue> queue = make_shared<EventQueue>();
			unique_ptr<Bar> bar1 = make_unique<Bar>(1.0f);
			unique_ptr<Product1> product1 = make_unique<Product1>();

			shared_ptr<GameTime> gameTime = make_shared<GameTime>();
			gameTime->SetCurrentTime(chrono::high_resolution_clock::now());

			Vector<shared_ptr<Event<Bar>>> barEventVect;
			Vector<shared_ptr<Event<Product1>>> prodEventVect;
			for (uint32_t i = 0; i < 100; i++)
			{
				barEventVect.PushBack(make_shared<Event<Bar>>(*bar1));
				queue->Enqueue(barEventVect.Back(), *gameTime);
				prodEventVect.PushBack(make_shared<Event<Product1>>(*product1));
				queue->Enqueue(prodEventVect.Back(), *gameTime);
			}

			// prepare the subs
			Vector<shared_ptr<EventSubExceptionThrower>> eventSubExceptionThrowersVect;
			Vector<shared_ptr<BarEventSubscriber>> barEventSubsVect;

			for (uint32_t i = 0; i < 3000; ++i)
			{
				eventSubExceptionThrowersVect.PushBack(make_shared<EventSubExceptionThrower>());
				Event<Bar>::Subscribe(*eventSubExceptionThrowersVect.Back());
				barEventSubsVect.PushBack(make_shared<BarEventSubscriber>());
				Event<Bar>::Subscribe(*barEventSubsVect.Back());
			}

			for (auto& eventSubscriberSub : eventSubExceptionThrowersVect)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			for (auto& eventSubscriberSub : barEventSubsVect)
			{
				Assert::IsFalse(eventSubscriberSub->GetIsNotified());
			}

			Assert::AreEqual(200U, queue->Size());

			GameTime gameTimeTemp = *gameTime;
			gameTimeTemp.SetCurrentTime(chrono::high_resolution_clock::now());
			auto func = [&queue, &gameTimeTemp]() { queue->Update(gameTimeTemp); };
			Assert::ExpectException<runtime_error>(func);

			for (auto& eventSubscriberSub : barEventSubsVect)
			{
				Assert::IsTrue(eventSubscriberSub->GetIsNotified());
			}

			for (auto& asynchQueueTester : eventSubExceptionThrowersVect)
			{
				Assert::IsTrue(asynchQueueTester->GetIsNotified());
			}

			Assert::AreEqual(0U, queue->Size());
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState AsynchEventTest::sStartMemState;
}