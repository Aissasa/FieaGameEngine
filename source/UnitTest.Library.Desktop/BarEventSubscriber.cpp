#include "pch.h"
#include "BarEventSubscriber.h"
#include "EventPublisher.h"
#include "Bar.h"
#include "Event.h"

using namespace std;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	/************************************************************************/
	BarEventSubscriber::BarEventSubscriber(): 
		mFloat(0)
	{
	}

	/************************************************************************/
	void BarEventSubscriber::Notify(const Library::EventPublisher& eventPublisher)
	{
		if (!mIsNotified)
		{
			assert(eventPublisher.Is(Event<Bar>::TypeIdClass()));
			Bar& bar = eventPublisher.As<Event<Bar>>()->Message();
			mFloat += bar.GetFloat();
			mIsNotified = true;
		}
	}

	/************************************************************************/
	std::float_t BarEventSubscriber::GetFloat() const
	{
		return mFloat;
	}

	/************************************************************************/
	void BarEventSubscriber::SetFloat(const std::float_t float_)
	{
		mFloat = float_;
	}
}
