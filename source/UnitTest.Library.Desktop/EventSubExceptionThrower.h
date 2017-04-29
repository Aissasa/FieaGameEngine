#pragma once

#include "EventSubscriber.h"

namespace UnitTestLibraryDesktop
{
	class EventSubExceptionThrower final : public Library::EventSubscriber
	{
	public:

		EventSubExceptionThrower() = default;
		~EventSubExceptionThrower() = default;
		EventSubExceptionThrower(const EventSubExceptionThrower & rhs) = default;
		EventSubExceptionThrower& operator=(const EventSubExceptionThrower& rhs) = default;

		virtual void Notify(Library::EventPublisher& eventPublisher) override;

	};
}
