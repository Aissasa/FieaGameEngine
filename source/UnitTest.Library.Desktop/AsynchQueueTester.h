#pragma once
#include "EventSubscriber.h"
#include "GameTime.h"
#include "EventQueue.h"

namespace UnitTestLibraryDesktop
{
	class AsynchQueueTester final : public Library::EventSubscriber
	{
	public:

		enum class Type
		{
			None,
			QueueBar,
			QueueProd,
			DequeueAll
		};

		explicit AsynchQueueTester(const std::shared_ptr<Library::EventQueue>& queue, Type type = Type::None, const std::shared_ptr<Library::GameTime>& gameTime = nullptr);
		~AsynchQueueTester() = default;
		AsynchQueueTester(const AsynchQueueTester & rhs) = default;
		AsynchQueueTester& operator=(const AsynchQueueTester& rhs) = default;

		virtual void Notify(Library::EventPublisher& eventPublisher) override;
		
		Type GetType() const;
		void SetType(const Type type);

	private:

		Type mType;
		std::shared_ptr<Library::GameTime> mGameTime;
		std::shared_ptr<Library::EventQueue> mQueue;
	};
}
