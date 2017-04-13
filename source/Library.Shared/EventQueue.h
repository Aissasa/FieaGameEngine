#pragma once
#include "EventPublisher.h"
#include "GameTime.h"

namespace Library
{
	/** EventQueue is a queue that contains events that wait to be fired.
	* @see EventPublisher
	* @see Event
	* @see GameTime
	*/
	class EventQueue final
	{

	public:

		/** EventQueue constructor.
		*/
		EventQueue(std::uint32_t length = QUEUE_LENGTH);

		/** EventQueue destructor.
		*/
		~EventQueue() = default;

		/** EventQueue copy constructor.
		*/
		EventQueue(const EventQueue & rhs) = default;

		/** EventQueue copy assignment operator.
		*/
		EventQueue& operator=(const EventQueue& rhs)= default;

		/** Adds an Event to the queue and sets its enqueue time and the its delay.
		* @param eventPublisher: Event to enqueue.
		* @param gameTime: GameTime to get the current time from.
		* @param delay: Delay for the event. Defaulted to zero.
		* @see GameTime
		* @see EventPublisher
		*/
		void Enqueue(const std::shared_ptr<EventPublisher>& eventPublisher, const GameTime& gameTime, const std::chrono::milliseconds& delay = std::chrono::milliseconds::zero());
		
		/** Delivers an Event on the queue and removes it from the latter.
		* @param eventPublisher: Event to deliver.
		* @see EventPublisher
		*/
		void Send(const std::shared_ptr<EventPublisher>& eventPublisher);

		/** Updates the queue by delivering and removing the expired events, based on the passed GameTime.
		* @param gameTime: GameTime to get the current time from.
		* @see GameTime
		* @see EventPublisher
		*/
		void Update(const GameTime& gameTime);

		/** Clears the queue from events.
		*/
		void Clear();

		/** Checks if the queue is empty.
		* @return Boolean representing if the queue is empty.
		*/
		bool IsEmpty() const;

		/** Returns the size of the queue.
		* @return Size of the queue.
		*/
		std::uint32_t Size() const;

	private:

		const static std::uint32_t QUEUE_LENGTH = 4;
		
		Vector<std::shared_ptr<EventPublisher>> mEvents;
	};
}
