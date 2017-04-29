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
	/************************************************************************/
	EventSubscriberSub::EventSubscriberSub():
		mBarEventSubscriber(make_shared<BarEventSubscriber>())
	{
	}

	/************************************************************************/
	void EventSubscriberSub::Notify(EventPublisher& eventPublisher)
	{
		UNREFERENCED_PARAMETER(eventPublisher);

		if (!mIsNotified)
		{
			Event<Bar>::Subscribe(*mBarEventSubscriber);
			mIsNotified = true;
		}
	}
}
