#include "pch.h"
#include "ActionEvent.h"
#include "EventMessageAttributed.h"
#include "Event.h"
#include "WorldState.h"
#include "World.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(ActionEvent);

	const string ActionEvent::SUB_TYPE_KEY = "SubType";
	const string ActionEvent::DELAY_KEY = "Delay";

	/************************************************************************/
	ActionEvent::ActionEvent(const std::string& subType, const std::int32_t delay):
		mSubType(subType), mDelay(delay)
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	void ActionEvent::Update(WorldState& worldState)
	{
		World* world = worldState.GetWorld();
		if (world)
		{
			EventMessageAttributed message(mSubType, &worldState);
			message.CopyAuxiliaryAttributes(*this);
			shared_ptr<Event<EventMessageAttributed>> event = make_shared<Event<EventMessageAttributed>>(message);
			world->GetEventQueue().Enqueue(event, worldState.GetGameTime(), chrono::milliseconds(mDelay));
		}
	}

	/************************************************************************/
	std::string ActionEvent::GetSubType() const
	{
		return mSubType;
	}

	/************************************************************************/
	void ActionEvent::SetSubType(const std::string& subType)
	{
		mSubType = subType;
	}

	/************************************************************************/
	std::int32_t ActionEvent::GetDelay() const
	{
		return mDelay;
	}

	/************************************************************************/
	void ActionEvent::SetDelay(const std::int32_t delay)
	{
		mDelay = delay;
	}

	/************************************************************************/
	void ActionEvent::InitPrescribedAttributes()
	{
		Action::InitPrescribedAttributes();
		AddExternalAttribute(SUB_TYPE_KEY, &mSubType, 1);
		AddExternalAttribute(DELAY_KEY, &mDelay, 1);
	}
}
