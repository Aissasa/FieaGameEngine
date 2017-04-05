#include "pch.h"
#include "TestAction.h"
#include "WorldState.h"

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(TestAction);

	/************************************************************************/
	TestAction::TestAction(): 
		mNumber(0)
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	void TestAction::Update(Library::WorldState& worldState)
	{
		worldState.SetAction(this);
		++mNumber;
		worldState.SetAction(nullptr);
	}

	/************************************************************************/
	std::int32_t TestAction::GetNumber() const
	{
		return mNumber;
	}

	/************************************************************************/
	void TestAction::SetNumber(const std::int32_t number)
	{
		mNumber = number;
	}

	/************************************************************************/
	void TestAction::InitPrescribedAttributes()
	{
		Action::InitPrescribedAttributes();
		AddExternalAttribute("Number", &mNumber, 1);
	}
}
