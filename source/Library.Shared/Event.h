#pragma once

#include "EventPublisher.h"

namespace Library
{
	template <typename T>
	class Event final : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher);

	public:

		explicit Event(const T& message, bool deleteAfterPublish = true);
		~Event() = default;
		Event(const Event & rhs) = default;
		Event(Event && rhs);
		Event& operator=(const Event& rhsS) = default;
		Event& operator=(Event&& rhs);

		static void Subscribe(const EventSubscriber& eventSubscriber);
		static void Unsubscribe(const EventSubscriber& eventSubscriber);
		static void UnsubscribeAll();

		T& Message();

	private:

		static Vector<EventSubscriber*> sSubscribers;
		T mMessage;
	};
}

#include "Event.inl"
