#pragma once

#include "EventPublisher.h"
#include <mutex>

namespace Library
{
	/** Event is a templated class that holds a message inside of it, which the class to put as an argument for the template.
	* It derives from EventPublisher.
	* @see EventPublisher
	*/
	template <typename T>
	class Event final : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher);

	public:

		/** Event constructor.
		*/
		explicit Event(const T& message);

		/** Event destructor.
		*/
		~Event() = default;

		/** Event copy constructor.
		*/
		Event(const Event & rhs) = default;

		/** Event move constructor.
		*/
		Event(Event && rhs);

		/** Event copy assignment operator.
		*/
		Event& operator=(const Event& rhsS) = default;

		/** Event move assignment operator.
		*/
		Event& operator=(Event&& rhs);

		/** Adds an EventSubscriber to the list of EventSubscribers of this Event type.
		* @param eventSubscriber: EventSubscriber to add.
		*/
		static void Subscribe(EventSubscriber& eventSubscriber);

		/** Removes an EventSubscriber from the list of EventSubscribers of this Event type.
		* @param eventSubscriber: EventSubscriber to remove.
		*/
		static void Unsubscribe(EventSubscriber& eventSubscriber);

		/** Removes all of the EventSubscribers in the list of EventSubscribers of this Event type.
		*/
		static void UnsubscribeAll();

		/** Returns the payload of the event.
		* @return Payload contained by the event.
		*/
		const T& Message() const;

		/** Initializes the EventSubscribers List to a certain capacity.
		* @param capacity: Capacity for the event list.
		*/
		static void Init(std::uint32_t capacity);

	private:

		static Vector<EventSubscriber*> sSubscribers;
		static std::mutex sMutex;
		T mMessage;
	};
}

#include "Event.inl"
