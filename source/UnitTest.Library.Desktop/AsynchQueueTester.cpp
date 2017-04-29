#include "pch.h"
#include "AsynchQueueTester.h"
#include "Event.h"
#include "Bar.h"
#include "Product1.h"

using namespace std;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	/************************************************************************/
	AsynchQueueTester::AsynchQueueTester(const std::shared_ptr<Library::EventQueue>& queue, Type type, const std::shared_ptr<Library::GameTime>& gameTime):
		mType(type), mQueue(queue)
	{
		if (gameTime)
		{
			mGameTime = gameTime;
		}
		else
		{
			mGameTime = make_shared<GameTime>();
			mGameTime->SetCurrentTime(chrono::high_resolution_clock::now());
		}
	}

	/************************************************************************/
	void AsynchQueueTester::Notify(EventPublisher& eventPublisher)
	{
		UNREFERENCED_PARAMETER(eventPublisher);

		if (!mIsNotified)
		{
			switch (mType)
			{
				case Type::QueueBar:
					mQueue->Enqueue(make_shared<Event<Bar>>(Bar()), *mGameTime);
					mIsNotified = true;
					break;
					
				case Type::QueueProd:
					mQueue->Enqueue(make_shared<Event<Product1>>(Product1()), *mGameTime);
					mIsNotified = true;
					break;

				case Type::DequeueAll:
					mQueue->Clear();
					mIsNotified = true;
					break;
				
				default:
					mIsNotified = true;
					
			}
		}
	}

	/************************************************************************/
	AsynchQueueTester::Type AsynchQueueTester::GetType() const
	{
		return mType;
	}

	/************************************************************************/
	void AsynchQueueTester::SetType(const Type type)
	{
		mType = type;
	}

}
