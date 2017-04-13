#pragma once

#include "RTTI.h"
#include "Vector.h"
#include <chrono>
#include <memory>

namespace Library
{
	class EventSubscriber;

	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI);

	public:

		virtual ~EventPublisher() = default;
		EventPublisher(const EventPublisher & rhs) = default;
		EventPublisher(EventPublisher && rhs);
		EventPublisher& operator=(const EventPublisher& rhs) = default;
		EventPublisher& operator=(EventPublisher&& rhs);

		void SetTime(const std::chrono::high_resolution_clock::time_point& currentTime, const std::chrono::milliseconds& delay = std::chrono::milliseconds::zero());
		std::chrono::high_resolution_clock::time_point TimeEnqueued() const;

		std::chrono::milliseconds Delay() const;
		bool IsExpired(const std::chrono::high_resolution_clock::time_point& currentTime) const;

		void Deliver() const;

	protected:

		EventPublisher(Vector<EventSubscriber*>& subscribers);

	private:

		Vector<EventSubscriber*>* mSubscribersListPtr;
		std::chrono::high_resolution_clock::time_point mEnqueuedTime;
		std::chrono::milliseconds mDelay;
	};
}
