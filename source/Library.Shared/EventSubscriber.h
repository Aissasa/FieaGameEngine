#pragma once

namespace Library
{
	class EventPublisher;

	/** EventSubscriber is an abstract base class for subscribers to events.
	* A subscriber will be tied to one or different kinds of events, depending on the implementation of notify.
	* @see EventPublisher
	* @see Event
	*/
	class EventSubscriber
	{

	public:

		/** EventSubscriber constructor.
		*/
		EventSubscriber();

		/** EventSubscriber destructor.
		*/
		virtual ~EventSubscriber() = default;

		/** EventSubscriber copy constructor.
		*/
		EventSubscriber(const EventSubscriber & rhs) = default;

		/** EventSubscriber move constructor.
		*/
		EventSubscriber(EventSubscriber && rhs) = default;

		/** EventSubscriber copy assignment operator.
		*/
		EventSubscriber& operator=(const EventSubscriber& rhs) = default;

		/** EventSubscriber move assignment operator.
		*/
		EventSubscriber& operator=(EventSubscriber&& rhs) = default;

		/** Notify executes a certain logic when the Event it is subscribed to is fired.
		* This method is pure virtual.
		* @param eventPublisher: The event that got fired.
		* @see EventPublisher
		*/
		virtual void Notify(EventPublisher& eventPublisher) = 0;

		/** Returns if the subscriber is notified.
		* @return Bool representing if the subscriber is notified.
		*/
		bool GetIsNotified() const;

		/** Resets the subscriber as not notified.
		*/
		void Reset();

	protected:

		bool mIsNotified;
	};
}
