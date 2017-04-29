#include "pch.h"
#include "EventSubExceptionThrower.h"
#include "EventPublisher.h"

using namespace std;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	/************************************************************************/
	void EventSubExceptionThrower::Notify(EventPublisher& eventPublisher)
	{
		UNREFERENCED_PARAMETER(eventPublisher);

		if (!mIsNotified)
		{
			mIsNotified = true;
			throw runtime_error("Hihi");
		}
	}
}
