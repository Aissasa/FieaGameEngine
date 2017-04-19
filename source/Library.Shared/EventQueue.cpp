#include "pch.h"
#include "EventQueue.h"

using namespace std;
using namespace std::chrono;

namespace Library
{
	/************************************************************************/
	EventQueue::EventQueue() :
		mEvents()
	{
	}

	/************************************************************************/
	void EventQueue::Enqueue(const shared_ptr<EventPublisher>& eventPublisher, const GameTime& gameTime, const milliseconds& delay)
	{
		eventPublisher->SetTime(gameTime.CurrentTime(), delay);
		{
			lock_guard<mutex> lock(mEventQueueMutex);
			mEvents.push_back(eventPublisher);
		}
	}

	/************************************************************************/
	void EventQueue::Dequeue(const std::shared_ptr<EventPublisher>& eventPublisher)
	{
		lock_guard<mutex> lock(mEventQueueMutex);
		auto it = find(mEvents.begin(), mEvents.end(), eventPublisher);
		if (it != mEvents.end())
		{
			mEvents.erase(it);
		}
	}

	/************************************************************************/
	void EventQueue::Send(const shared_ptr<EventPublisher>& eventPublisher)
	{
		lock_guard<mutex> lock(mEventQueueMutex);
		auto it = find(mEvents.begin(), mEvents.end(), eventPublisher);
		if (it != mEvents.end())
		{
			(*it)->Deliver();
			mEvents.erase(it);
		}
	}

	/************************************************************************/
	void EventQueue::Update(const GameTime& gameTime)
	{
		vector<shared_ptr<EventPublisher>> expiredEvents;
		{
			// lock the events list until it's done being modified
			lock_guard<mutex> lock(mEventQueueMutex);
			auto firstIt = partition(mEvents.begin(), mEvents.end(), [&](shared_ptr<EventPublisher> ptr) { return !ptr->IsExpired(gameTime.CurrentTime()); });
			if (firstIt != mEvents.end())
			{
				expiredEvents = vector<shared_ptr<EventPublisher>>(firstIt, mEvents.end());
				mEvents.erase(firstIt, mEvents.end());
			}
		}

		vector<future<void>> futures;
		for (auto& eventPublisher : expiredEvents)
		{
			futures.push_back(async(launch::async, [&eventPublisher]()
			{
				return eventPublisher->Deliver();
			}));
		}

		for (auto& future : futures)
		{
			future.get();
		}
	}

	/************************************************************************/
	void EventQueue::Clear()
	{
		lock_guard<mutex> lock(mEventQueueMutex);
		mEvents.clear();
	}

	/************************************************************************/
	bool EventQueue::IsEmpty() const
	{
		return mEvents.empty();
	}

	/************************************************************************/
	uint32_t EventQueue::Size() const
	{
		return mEvents.size();
	}
}
