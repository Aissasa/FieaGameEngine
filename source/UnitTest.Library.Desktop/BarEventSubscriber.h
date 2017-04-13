#pragma once

#pragma once
#include "EventSubscriber.h"

namespace UnitTestLibraryDesktop
{
	class BarEventSubscriber final : public Library::EventSubscriber
	{

	public:
		BarEventSubscriber();
		~BarEventSubscriber() = default;
		BarEventSubscriber(const BarEventSubscriber & rhs) = default;
		BarEventSubscriber& operator=(const BarEventSubscriber& rhs) = default;

		virtual void Notify(const Library::EventPublisher& eventPublisher) override;

		std::float_t GetFloat() const;

		void SetFloat(const std::float_t float_);

	private:

		std::float_t mFloat;

	};
}
