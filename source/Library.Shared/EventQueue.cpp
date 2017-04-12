#include "pch.h"
#include "EventQueue.h"

using namespace std;
using namespace std::chrono;

namespace Library
{
	/************************************************************************/
	EventQueue::EventQueue(uint32_t length) :
		mEvents(length)
	{
	}

	/************************************************************************/
	void EventQueue::Enqueue(const shared_ptr<EventPublisher>& eventPublisher, const GameTime& gameTime, const milliseconds& delay)
	{
		eventPublisher->SetTime(gameTime.CurrentTime(), delay);
		mEvents.PushBack(eventPublisher);
	}

	/************************************************************************/
	void EventQueue::Send(const shared_ptr<EventPublisher>& eventPublisher)
	{
		auto it = mEvents.Find(eventPublisher);
		if (it != mEvents.end())
		{
			(*it)->Deliver();
			mEvents.Remove(eventPublisher);
		}
	}

	/************************************************************************/
	void EventQueue::Update(const GameTime& gameTime)
	{
		Vector<std::shared_ptr<EventPublisher>> eventsToDelete;

		for (auto& eventPublisher : mEvents)
		{
			if (eventPublisher->IsExpired(gameTime.CurrentTime()))
			{
				eventPublisher->Deliver();
				eventsToDelete.PushBack(eventPublisher);
			}
		}

		for (auto& eventToDelete : eventsToDelete)
		{
			mEvents.Remove(eventToDelete);
		}
	}

	/************************************************************************/
	void EventQueue::Clear()
	{
		mEvents.Empty();
	}

	/************************************************************************/
	bool EventQueue::IsEmpty() const
	{
		return mEvents.IsEmpty();
	}

	/************************************************************************/
	uint32_t EventQueue::Size() const
	{
		return mEvents.Size();
	}
}
