#include "pch.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"

using namespace std;
using namespace chrono;

namespace Library
{
	RTTI_DEFINITIONS(EventPublisher);

	/************************************************************************/
	EventPublisher::EventPublisher(Vector<EventSubscriber*>& subscribers) :
		mSubscribersListPtr(&subscribers), mEnqueuedTime(high_resolution_clock::now())
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
	high_resolution_clock::time_point EventPublisher::TimeEnqueued() const
	{
		return mEnqueuedTime;
	}

	/************************************************************************/
	milliseconds EventPublisher::Delay() const
	{
		return mDelay;
	}

	/************************************************************************/
	bool EventPublisher::IsExpired(const high_resolution_clock::time_point& currentTime) const
	{
		return currentTime > (mEnqueuedTime + mDelay);
	}

	/************************************************************************/
	void EventPublisher::Deliver() const
	{
		for (auto& subscriber : *mSubscribersListPtr)
		{
			subscriber->Notify(*this);
		}
	}

}
