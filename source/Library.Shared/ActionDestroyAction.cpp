#include "pch.h"
#include "ActionDestroyAction.h"
#include "WorldState.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(ActionDestroyAction);

	const string ActionDestroyAction::INSTANCE_NAME_ENTRY_NAME = "InstanceName";

	/************************************************************************/
	ActionDestroyAction::ActionDestroyAction()
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	bool ActionDestroyAction::DestroyAction(const WorldState& worldState, const std::string& actionInstanceName, Attributed& context) const
	{
		if (worldState.GetWorld())
		{
			Datum* dat = context.Find(ACTIONS_ENTRY_NAME);
			assert(dat->Type() == Datum::DatumType::Table);

			uint32_t size = dat->Size();
			for (uint32_t i = 0; i < size; ++i)
			{
				assert((*dat)[i].Is(Action::TypeIdClass()));
				auto& action = static_cast<Action&>((*dat)[i]);
				if (action.Name() == actionInstanceName)
				{
					worldState.GetWorld()->AddActionToDestroy(action);
					return true;
				}
			}
		}

		return false;
	}

	/************************************************************************/
	std::string ActionDestroyAction::GetInstanceName() const
	{
		return mInstanceName;
	}

	/************************************************************************/
	void ActionDestroyAction::SetInstanceName(const std::string& instance_name)
	{
		mInstanceName = instance_name;
	}

	/************************************************************************/
	void ActionDestroyAction::Update(WorldState& worldState)
	{
		Attributed* context = GetPredecessor();
		if (context)
		{
			DestroyAction(worldState, mInstanceName, *context);
		}
	}

	/************************************************************************/
	void ActionDestroyAction::InitPrescribedAttributes()
	{
		AddExternalAttribute(INSTANCE_NAME_ENTRY_NAME, &mInstanceName, 1);
	}
}
