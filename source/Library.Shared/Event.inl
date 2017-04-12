#include "Event.h"

namespace Library
{
	template <typename T>
	RTTI_DEFINITIONS(Event<T>);

	/************************************************************************/
	template <typename T>
	Event<T>::Event(const T& message, bool deleteAfterPublish) :
		EventPublisher(sSubscribers, deleteAfterPublish), mMessage(message)
	{
	}

	/************************************************************************/
	template <typename T>
	Event<T>::Event(Event&& rhs) :
		EventPublisher(rhs)
	{
		sSubscribers = std::move(rhs.sSubscribers);
		mMessage = std::move(rhs.mMessage);
	}

	/************************************************************************/
	template <typename T>
	Event<T>& Event<T>::operator=(Event&& rhs)
	{
		if (this != &rhs)
		{
			EventPublisher::operator=(rhs);
			sSubscribers = std::move(rhs.sSubscribers);
			mMessage = std::move(rhs.mMessage);
		}

		return *this;
	}

	/************************************************************************/
	template <typename T>
	void Event<T>::Subscribe(const EventSubscriber& eventSubscriber)
	{
		sSubscribers.PushBack(&eventSubscriber);
	}

	/************************************************************************/
	template <typename T>
	void Event<T>::Unsubscribe(const EventSubscriber& eventSubscriber)
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