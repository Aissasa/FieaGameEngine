#include "Event.h"

namespace Library
{
	template <typename T>
	RTTI_DEFINITIONS(Event<T>);

	template <typename T>
	Vector<EventSubscriber*> Event<T>::sSubscribers = Vector<EventSubscriber*>();
	
	template <typename T>
	std::mutex Event<T>::sMutex;

	/************************************************************************/
	template <typename T>
	Event<T>::Event(const T& message) :
		EventPublisher(sSubscribers, sMutex), mMessage(message)
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
		std::lock_guard<std::mutex> lock(sMutex);
		if (sSubscribers.Find(&eventSubscriber) == sSubscribers.end())
		{
			sSubscribers.PushBack(&eventSubscriber);
		}
	}

	/************************************************************************/
	template <typename T>
	void Event<T>::Unsubscribe(EventSubscriber& eventSubscriber)
	{
		std::lock_guard<std::mutex> lock(sMutex);
		sSubscribers.Remove(&eventSubscriber); 
	}

	/************************************************************************/
	template <typename T>
	void Event<T>::UnsubscribeAll()
	{
		std::lock_guard<std::mutex> lock(sMutex);
		sSubscribers.Empty();
	}

	/************************************************************************/
	template <typename T>
	const T& Event<T>::Message() const
	{
		return mMessage;
	}

	/************************************************************************/
	template <typename T>
	void Event<T>::Init(std::uint32_t capacity)
	{
		sSubscribers = Vector<EventSubscriber*>(capacity);
	}
}
