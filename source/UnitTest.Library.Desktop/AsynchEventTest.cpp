#include "pch.h"
#include "CppUnitTest.h"
#include "Event.h"
#include "Bar.h"
#include "BarEventSubscriber.h"
#include "EventSubscriberSub.h"
#include "EventUnsubscriber.h"

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
		TEST_METHOD(EventSubscriberSubTest)
		{
			unique_ptr<Bar> bar1 = make_unique<Bar>(1.0f);
			unique_ptr<Event<Bar>> barEvent1 = make_unique<Event<Bar>>(*bar1);

			Vector<shared_ptr<EventSubscriberSub>> vector1;
			Vector<shared_ptr<BarEventSubscriber>> vector2;
			for (int i = 0; i < 3000; ++i)
			{
				vector1.PushBack(make_unique<EventSubscriberSub>());
				Event<Bar>::Subscribe(*vector1.Back());
				vector2.PushBack(make_shared<BarEventSubscriber>());
				Event<Bar>::Subscribe(*vector2.Back());
			}

			barEvent1->Deliver();
			barEvent1->Deliver();

			Event<Bar>::UnsubscribeAll();

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
				vector1.PushBack(make_unique<EventUnsubscriber>());
				Event<Bar>::Subscribe(*vector1.Back());
				vector2.PushBack(make_shared<BarEventSubscriber>());
				Event<Bar>::Subscribe(*vector2.Back());
			}

			barEvent1->Deliver();
			barEvent1->Deliver();

			Event<Bar>::UnsubscribeAll();
		}

#pragma endregion

	private:
		static _CrtMemState sStartMemState;

	};

	_CrtMemState AsynchEventTest::sStartMemState;
}