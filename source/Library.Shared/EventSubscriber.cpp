#include "pch.h"
#include "EventSubscriber.h"

namespace Library
{
	/************************************************************************/
	EventSubscriber::EventSubscriber(): 
		mIsNotified(false)
	{
	}

	/************************************************************************/
	void EventSubscriber::Reset()
	{
		mIsNotified = false;
	}

	/************************************************************************/
	bool EventSubscriber::GetIsNotified() const
	{
		return mIsNotified;
	}
}
