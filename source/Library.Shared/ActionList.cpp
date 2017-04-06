#include "pch.h"
#include "ActionList.h"
#include "Factory.h"
#include "WorldState.h"
#include "World.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(ActionList);

	/************************************************************************/
	ActionList::ActionList():
		mActionsDatumPtr(nullptr)
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	Datum& ActionList::Actions() const
	{
		assert(mActionsDatumPtr != nullptr);
		return *mActionsDatumPtr;
	}

	/************************************************************************/
	Action& ActionList::CreateAction(const std::string& actionClassName, const std::string& actionInstanceName)
	{
		Action* action = Factory<Action>::Create(actionClassName);
		if (!action)
		{
			throw exception("The correspondant factory needs to be initialized.");
		}
		action->SetName(actionInstanceName);
		Adopt(*action, ACTIONS_ENTRY_NAME);

		return *action;
	}

	/************************************************************************/
	bool ActionList::DestroyAction(const WorldState& worldState, const std::string& actionInstanceName) const
	{
		if (worldState.GetWorld())
		{
			uint32_t size = mActionsDatumPtr->Size();
			for (uint32_t i = 0; i < size; ++i)
			{
				assert((*mActionsDatumPtr)[i].Is(Action::TypeIdClass()));
				auto& action = static_cast<Action&>((*mActionsDatumPtr)[i]);
				if (action.Name() == actionInstanceName)
				{
					worldState.GetWorld()->AddActionToDestory(*mActionsDatumPtr, i);
					return true;
				}
			}
		}

		return false;
	}

	/************************************************************************/
	Action* ActionList::FindNestedActionByName(const std::string& actionInstanceName) const
	{
		uint32_t size = mActionsDatumPtr->Size();
		for (uint32_t i = 0; i < size; ++i)
		{
			assert((*mActionsDatumPtr)[i].Is(Action::TypeIdClass()));
			auto& action = static_cast<Action&>((*mActionsDatumPtr)[i]);
			if (action.Name() == actionInstanceName)
			{
				return &action;
			}
		}

		return nullptr;
	}

	/************************************************************************/
	void ActionList::Update(WorldState& worldState)
	{
		assert(mActionsDatumPtr != nullptr);
		worldState.SetAction(this);
		uint32_t size = mActionsDatumPtr->Size();
		for (uint32_t i = 0; i < size; ++i)
		{
			assert((*mActionsDatumPtr)[i].Is(Action::TypeIdClass()));
			static_cast<Action&>((*mActionsDatumPtr)[i]).Update(worldState);
		}
		worldState.SetAction(nullptr);
	}

	/************************************************************************/
	void ActionList::InitPrescribedAttributes()
	{
		Action::InitPrescribedAttributes();
		mActionsDatumPtr = &(AddEmptyNestedScopeAttribute(ACTIONS_ENTRY_NAME));
	}
}
