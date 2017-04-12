#pragma once
#include "EventPublisher.h"
#include "GameTime.h"

namespace Library
{
	class EventQueue final
	{

	public:

		EventQueue(std::uint32_t length = QUEUE_LENGTH);
		~EventQueue() = default;
		EventQueue(const EventQueue & rhs) = default;
		EventQueue& operator=(const EventQueue& rhs)= default;

		void Enqueue(const std::shared_ptr<EventPublisher>& eventPublisher, const GameTime& gameTime, const std::chrono::milliseconds& delay = std::chrono::milliseconds::zero());
		void Send(const std::shared_ptr<EventPublisher>& eventPublisher);
		void Update(const GameTime& gameTime);
		void Clear();
		bool IsEmpty() const;
		std::uint32_t Size() const;

	private:

		const static std::uint32_t QUEUE_LENGTH = 4;
		
		Vector<std::shared_ptr<EventPublisher>> mEvents;
	};
}
