#pragma once

#include "RTTI.h"
#include "Vector.h"

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
		std::chrono::high_resolution_clock::time_point TimeEnqueued();

		std::chrono::milliseconds Delay();
		bool IsExpired(const std::chrono::high_resolution_clock::time_point& currentTime);

		void Deliver();

	protected:

		EventPublisher(Vector<EventSubscriber*>& subscribers, bool deleteAfterPublish = true);
		bool mDeleteAfterPublish;

	private:

		Vector<EventSubscriber*>* mSubscribersListPtr;
		std::chrono::high_resolution_clock::time_point mEnqueuedTime;
		std::chrono::milliseconds mDelay;
	};
}
