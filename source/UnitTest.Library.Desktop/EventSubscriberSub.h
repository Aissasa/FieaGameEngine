#pragma once

#pragma once
#include "EventSubscriber.h"
#include <memory>
#include "BarEventSubscriber.h"

namespace UnitTestLibraryDesktop
{
	class EventSubscriberSub final : public Library::EventSubscriber
	{

	public:

		EventSubscriberSub();
		~EventSubscriberSub() = default;
		EventSubscriberSub(const EventSubscriberSub & rhs) = default;
		EventSubscriberSub& operator=(const EventSubscriberSub& rhs) = default;

		virtual void Notify(Library::EventPublisher& eventPublisher) override;

	private:

		std::shared_ptr<BarEventSubscriber> mBarEventSubscriber;
	};
}
