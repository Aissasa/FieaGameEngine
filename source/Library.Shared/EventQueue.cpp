#include "pch.h"
#include "EventQueue.h"

using namespace std;
using namespace std::chrono;

namespace Library
{
	/************************************************************************/
	EventQueue::EventQueue(/*uint32_t length*/) :
		mEvents(/*length*/)
	{
	}

	/************************************************************************/
	void EventQueue::Enqueue(const shared_ptr<EventPublisher>& eventPublisher, const GameTime& gameTime, const milliseconds& delay)
	{
		eventPublisher->SetTime(gameTime.CurrentTime(), delay);
		mEvents.push_back(eventPublisher);
	}

	/************************************************************************/
	void EventQueue::Dequeue(const std::shared_ptr<EventPublisher>& eventPublisher)
	{
		auto it = find(mEvents.begin(), mEvents.end(), eventPublisher);
		if (it != mEvents.end())
		{
			mEvents.erase(it);
		}
	}

	/************************************************************************/
	void EventQueue::Send(const shared_ptr<EventPublisher>& eventPublisher)
	{
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
		// urgent lock the mutex here
		//auto eventsCopy = mEvents;

		//Vector<shared_ptr<EventPublisher>> eventsToDelete;
		//for (auto& eventPublisher : eventsCopy)
		//{
		//	if (eventPublisher->IsExpired(gameTime.CurrentTime()))
		//	{
		//		eventPublisher->Deliver();
		//		eventsToDelete.PushBack(eventPublisher);
		//	}
		//}

		//for (auto& eventToDelete : eventsToDelete)
		//{
		//	mEvents.Remove(eventToDelete);
		//}

		auto firstIt = partition(mEvents.begin(), mEvents.end(), [&](shared_ptr<EventPublisher> ptr) { return !ptr->IsExpired(gameTime.CurrentTime()); });
		if (firstIt != mEvents.end())
		{
			vector<shared_ptr<EventPublisher>> subVect(firstIt, mEvents.end());
			mEvents.erase(firstIt, mEvents.end());

			for (auto& eventPublisher : subVect)
			{
				eventPublisher->Deliver();
			}
		}
	}

	/************************************************************************/
	void EventQueue::Clear()
	{
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
