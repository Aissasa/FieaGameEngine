#include "pch.h"
#include "EventPublisher.h"
#include "EventSubscriber.h"

using namespace std;
using namespace chrono;

namespace Library
{
	RTTI_DEFINITIONS(EventPublisher);

	/************************************************************************/
	EventPublisher::EventPublisher(Vector<EventSubscriber*>& subscribers, std::mutex& mutex) :
		mSubscribersListPtr(&subscribers), mMutexPtr(&mutex), mEnqueuedTime(high_resolution_clock::now())
	{
	}

	/************************************************************************/
	EventPublisher::EventPublisher(EventPublisher&& rhs)
	{
		mSubscribersListPtr = rhs.mSubscribersListPtr;
		mMutexPtr = rhs.mMutexPtr;
		rhs.mSubscribersListPtr = nullptr;
		rhs.mMutexPtr = nullptr;
	}

	/************************************************************************/
	EventPublisher& EventPublisher::operator=(EventPublisher&& rhs)
	{
		if (this != &rhs)
		{
			mSubscribersListPtr = rhs.mSubscribersListPtr;
			mMutexPtr = rhs.mMutexPtr;
			rhs.mSubscribersListPtr = nullptr;
			rhs.mMutexPtr = nullptr;
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
	void EventPublisher::Deliver()
	{
		//auto copyVect = *mSubscribersListPtr;

		vector<future<void>> futures;

		{
			lock_guard<std::mutex> lock(*mMutexPtr);
			for (auto& subscriber : *mSubscribersListPtr)
			{
				futures.push_back(async(launch::async, [&subscriber, this]()
				{
					return subscriber->Notify(*this);
				}));
			}
		}

		for (auto& future : futures)
		{
			future.get();
		}
	}

}
