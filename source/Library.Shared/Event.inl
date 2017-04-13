#include "Event.h"

namespace Library
{
	template <typename T>
	RTTI_DEFINITIONS(Event<T>);

	template <typename T>
	Vector<EventSubscriber*> Event<T>::sSubscribers = Vector<EventSubscriber*>();

	/************************************************************************/
	template <typename T>
	Event<T>::Event(const T& message) :
		EventPublisher(sSubscribers), mMessage(message)
	{
	}

	/************************************************************************/
	template <typename T>
	Event<T>::Event(Event&& rhs) :
		EventPublisher(rhs)
	{
		mMessage = std::move(rhs.mMessage);
	}

	/************************************************************************/
	template <typename T>
	Event<T>& Event<T>::operator=(Event&& rhs)
	{
		if (this != &rhs)
		{
			EventPublisher::operator=(rhs);
			mMessage = std::move(rhs.mMessage);
		}

		return *this;
	}

	/************************************************************************/
	template <typename T>
	void Event<T>::Subscribe(EventSubscriber& eventSubscriber)
	{
		sSubscribers.PushBack(&eventSubscriber);
	}

	/************************************************************************/
	template <typename T>
	void Event<T>::Unsubscribe(EventSubscriber& eventSubscriber)
	{
		sSubscribers.Remove(&eventSubscriber); 
	}

	/************************************************************************/
	template <typename T>
	void Event<T>::UnsubscribeAll()
	{
		sSubscribers.Empty();
	}

	/************************************************************************/
	template <typename T>
	T& Event<T>::Message()
	{
		return mMessage;
	}
}
