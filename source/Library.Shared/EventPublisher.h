#pragma once

#include "RTTI.h"
#include "Vector.h"
#include <chrono>
#include <memory>

namespace Library
{
	class EventSubscriber;

	/** EventPublisher is an abstract base class that Events inherit from.
	* It inherits from RTTI for type checking for Events.
	* @see Event
	* @see RTTI
	*/
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI);

	public:

		/** EventPublisher destructor.
		*/
		virtual ~EventPublisher() = default;

		/** EventPublisher copy constructor.
		*/
		EventPublisher(const EventPublisher & rhs) = default;

		/** EventPublisher move constructor.
		*/
		EventPublisher(EventPublisher && rhs);

		/** EventPublisher copy assignment operator.
		*/
		EventPublisher& operator=(const EventPublisher& rhs) = default;

		/** EventPublisher move assignment operator.
		*/
		EventPublisher& operator=(EventPublisher&& rhs);

		/** Sets the time enqueued time and the delay for the event.
		* @param currentTime: Represents the enqueued time of the event.
		* @param delay: Represents the delay before the event is fired. Defaulted to 0 milliseconds.
		*/
		void SetTime(const std::chrono::high_resolution_clock::time_point& currentTime, const std::chrono::milliseconds& delay = std::chrono::milliseconds::zero());
		
		/** Returns the enqueued time of the event.
		* @return Enqueued time of the event.
		*/
		std::chrono::high_resolution_clock::time_point TimeEnqueued() const;

		/** Returns the delay of the event.
		* @return Delay of the event.
		*/
		std::chrono::milliseconds Delay() const;

		/** Checks if the event has expired or not.
		* @param currentTime: current time to test against.
		* @return Boolean representing if the event expired..
		*/
		bool IsExpired(const std::chrono::high_resolution_clock::time_point& currentTime) const;

		/** Notifies all of the subscribers of the Event.
		* @see EventSubscriber
		*/
		void Deliver();

	protected:

		EventPublisher(Vector<EventSubscriber*>& subscribers);

	private:

		Vector<EventSubscriber*>* mSubscribersListPtr;
		std::chrono::high_resolution_clock::time_point mEnqueuedTime;
		std::chrono::milliseconds mDelay;
	};
}
