#include "pch.h"
#include "EventUnsubscriber.h"
#include "EventPublisher.h"
#include "Event.h"
#include "Bar.h"

using namespace std;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	/************************************************************************/
	void EventUnsubscriber::Notify(EventPublisher& eventPublisher)
	{
		UNREFERENCED_PARAMETER(eventPublisher);

		if (!mIsNotified)
		{
			Event<Bar>::UnsubscribeAll();
			mIsNotified = true;
		}
	}
}
