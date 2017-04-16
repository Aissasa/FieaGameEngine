#include "pch.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "EventMessageAttributed.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(ReactionAttributed);

	const string ReactionAttributed::SUB_TYPE_KEY = "SubType";

	/************************************************************************/
	ReactionAttributed::ReactionAttributed(const std::string& subType):
		mSubType(subType), Reaction()
	{
		InitPrescribedAttributes();
		Event<EventMessageAttributed>::Subscribe(*this);
	}

	/************************************************************************/
	void ReactionAttributed::Notify(EventPublisher& eventPublisher)
	{
		if (eventPublisher.Is(Event<EventMessageAttributed>::TypeIdClass()))
		{
			Event<EventMessageAttributed>& event = static_cast<Event<EventMessageAttributed>&>(eventPublisher);
			if (mSubType == event.Message().GetSubType())
			{
				CopyAuxiliaryAttributes(event.Message());
				WorldState* state = event.Message().GetWorldState();
				if (state)
				{
					Update(*state);
					mIsNotified = true;
				}
			}
		}
	}

	/************************************************************************/
	std::string ReactionAttributed::GetSubType() const
	{
		return mSubType;
	}

	/************************************************************************/
	void ReactionAttributed::SetSubType(const std::string& subType)
	{
		mSubType = subType;
	}

	/************************************************************************/
	void ReactionAttributed::InitPrescribedAttributes()
	{
		Reaction::InitPrescribedAttributes();
		AddExternalAttribute(SUB_TYPE_KEY, &mSubType, 1);
	}

	/************************************************************************/
	void ReactionAttributed::Update(WorldState& worldState)
	{
		ActionList::Update(worldState);
	}
}
