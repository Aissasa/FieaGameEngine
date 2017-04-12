#pragma once

namespace Library
{
	class EventPublisher;

	class EventSubscriber
	{

	public:

		EventSubscriber();
		virtual ~EventSubscriber() = default;
		EventSubscriber(const EventSubscriber & rhs) = default;
		EventSubscriber(EventSubscriber && rhs) = default;
		EventSubscriber& operator=(const EventSubscriber& rhs) = default;
		EventSubscriber& operator=(EventSubscriber&& rhs) = default;

		virtual void Notify(const EventPublisher& eventPublisher) = 0;

		void Reset();

	private:

		bool mIsNotified;
	};
}
