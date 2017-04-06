#include "pch.h"
#include "ActionListIf.h"
#include "WorldState.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(ActionListIf);

	const string ActionListIf::CONDITION_INTEGER_NAME = "Condition";
	const string ActionListIf::THEN_ACTION_NAME = "ThenAction";
	const string ActionListIf::ELSE_ACTION_NAME = "ElseAction";

	/************************************************************************/
	ActionListIf::ActionListIf()
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	std::string ActionListIf::GetConditionIntegerName() const
	{
		return mConditionIntegerName;
	}

	/************************************************************************/
	void ActionListIf::SetConditionIntegerName(const std::string& condition_integer_name)
	{
		mConditionIntegerName = condition_integer_name;
	}

	/************************************************************************/
	Action& ActionListIf::CreateThenAction(const std::string& actionClassName)
	{
		return CreateAction(actionClassName, THEN_ACTION_NAME);
	}

	/************************************************************************/
	bool ActionListIf::DestroyThenAction(const WorldState& worldState) const
	{
		return DestroyAction(worldState, THEN_ACTION_NAME);
	}

	/************************************************************************/
	Action& ActionListIf::CreateElseAction(const std::string& actionClassName)
	{
		return CreateAction(actionClassName, ELSE_ACTION_NAME);
	}

	/************************************************************************/
	bool ActionListIf::DestroyElseAction(const WorldState& worldState) const
	{
		return DestroyAction(worldState, ELSE_ACTION_NAME);
	}

	/************************************************************************/
	void ActionListIf::Update(WorldState& worldState)
	{
		assert(mActionsDatumPtr != nullptr);
		worldState.SetAction(this);

		Datum* valueDatum = Search(mConditionIntegerName);
		if (valueDatum && valueDatum->Type() == Datum::DatumType::Integer)
		{
			int32_t value = valueDatum->Get<int32_t>(0);
			Action* actionPtr;
			if (value)
			{
				actionPtr = FindNestedActionByName(THEN_ACTION_NAME);
			}
			else
			{
				actionPtr = FindNestedActionByName(ELSE_ACTION_NAME);
			}

			if (!actionPtr)
			{
				throw exception("Both the then and the else actions need to exist.");
			}

			actionPtr->Update(worldState);
		}
		worldState.SetAction(nullptr);
	}

	/************************************************************************/
	void ActionListIf::InitPrescribedAttributes()
	{
		ActionList::InitPrescribedAttributes();
		AddExternalAttribute(CONDITION_INTEGER_NAME, &mConditionIntegerName, 1);
	}
}
