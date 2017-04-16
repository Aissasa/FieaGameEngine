#include "pch.h"
#include "EventMessageAttributed.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(EventMessageAttributed);

	const string EventMessageAttributed::SUB_TYPE_KEY = "SubType";
	const string EventMessageAttributed::WORLD_KEY = "World";

	/************************************************************************/
	EventMessageAttributed::EventMessageAttributed(const std::string& subType, WorldState* worldState):
		mWorldState(worldState), mSubType(subType), Attributed()
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	std::string EventMessageAttributed::GetSubType() const
	{
		return mSubType;
	}

	/************************************************************************/
	void EventMessageAttributed::SetSubType(const std::string& subType)
	{
		mSubType = subType;
	}

	/************************************************************************/
	WorldState* EventMessageAttributed::GetWorldState() const
	{
		return mWorldState;
	}

	/************************************************************************/
	void EventMessageAttributed::SetWorldState(WorldState& worldState)
	{
		mWorldState = &worldState;
	}

	/************************************************************************/
	void EventMessageAttributed::InitPrescribedAttributes()
	{
		AddInternalAttribute("this", static_cast<RTTI*>(this));
		AddExternalAttribute(SUB_TYPE_KEY, &mSubType, 1);
	}
}
