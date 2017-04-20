#pragma once

#pragma once
#include "EventSubscriber.h"

namespace UnitTestLibraryDesktop
{
	class EventUnsubscriber final : public Library::EventSubscriber
	{

	public:
		EventUnsubscriber() = default;
		~EventUnsubscriber() = default;
		EventUnsubscriber(const EventUnsubscriber & rhs) = default;
		EventUnsubscriber& operator=(const EventUnsubscriber& rhs) = default;

		virtual void Notify(Library::EventPublisher& eventPublisher) override;
	};
}
