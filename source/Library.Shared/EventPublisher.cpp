#include "pch.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"

using namespace std;
using namespace chrono;

namespace Library
{
	RTTI_DEFINITIONS(EventPublisher);

	/************************************************************************/
	EventPublisher::EventPublisher(Vector<EventSubscriber*>& subscribers, bool deleteAfterPublish) :
		mSubscribersListPtr(&subscribers), mEnqueuedTime(high_resolution_clock::now()), mDeleteAfterPublish(deleteAfterPublish)
	{
	}

	/************************************************************************/
	EventPublisher::EventPublisher(EventPublisher&& rhs)
	{
		mSubscribersListPtr = rhs.mSubscribersListPtr;
		rhs.mSubscribersListPtr = nullptr;
	}

	/************************************************************************/
	EventPublisher& EventPublisher::operator=(EventPublisher&& rhs)
	{
		if (this != &rhs)
		{
			mSubscribersListPtr = rhs.mSubscribersListPtr;
			rhs.mSubscribersListPtr = nullptr;
		}

		return *this;
	}

	/************************************************************************/
	void EventPublisher::SetTime(const high_resolution_clock::time_point& currentTime, const milliseconds& delay)
	{
		mEnqueuedTime = currentTime;
		mDelay = delay;
	}

	/************************************************************************/
	high_resolution_clock::time_point EventPublisher::TimeEnqueued()
	{
		return mEnqueuedTime;
	}

	/************************************************************************/
	milliseconds EventPublisher::Delay()
	{
		return mDelay;
	}

	/************************************************************************/
	bool EventPublisher::IsExpired(const high_resolution_clock::time_point& currentTime)
	{
		return currentTime > (mEnqueuedTime + mDelay);
	}

	/************************************************************************/
	void EventPublisher::Deliver()
	{
		for (auto& subscriber : *mSubscribersListPtr)
		{
			subscriber->Notify(*this);
		}
	}
}
