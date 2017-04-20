#include "pch.h"
#include "EventUnsubscriber.h"
#include "EventPublisher.h"
#include "Event.h"
#include "Bar.h"

using namespace std;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	void EventUnsubscriber::Notify(EventPublisher& eventPublisher)
	{
		if (!mIsNotified)
		{
			assert(eventPublisher.Is(Event<Bar>::TypeIdClass()));
			Event<Bar>::UnsubscribeAll();
			mIsNotified = true;
		}
	}
}
