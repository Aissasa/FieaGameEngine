#include "pch.h"
#include "EventSubscriberSub.h"
#include "EventPublisher.h"
#include "Event.h"
#include "Bar.h"
#include "BarEventSubscriber.h"

using namespace std;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	EventSubscriberSub::EventSubscriberSub():
		mBarEventSubscriber(make_shared<BarEventSubscriber>())
	{
	}

	void EventSubscriberSub::Notify(EventPublisher& eventPublisher)
	{
		if (!mIsNotified)
		{
			assert(eventPublisher.Is(Event<Bar>::TypeIdClass()));
			Event<Bar>::Subscribe(*mBarEventSubscriber);
			mIsNotified = true;
		}
	}
}
