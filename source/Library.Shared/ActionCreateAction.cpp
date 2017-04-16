#include "pch.h"
#include "ActionCreateAction.h"
#include "WorldState.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(ActionCreateAction);

	const string ActionCreateAction::CLASS_NAME_ENTRY_NAME = "ClassName";
	const string ActionCreateAction::INSTANCE_NAME_ENTRY_NAME = "InstanceName";

	/************************************************************************/
	ActionCreateAction::ActionCreateAction()
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	Action& ActionCreateAction::CreateAction(const std::string& actionClassName, const std::string& actionInstanceName, Attributed& context) const
	{
		Action* action = Factory<Action>::Create(actionClassName);
		if (!action)
		{
			throw exception("The correspondant factory needs to be initialized.");
		}
		action->SetName(actionInstanceName);
		context.Adopt(*action, ACTIONS_ENTRY_NAME);

		return *action;
	}

	/************************************************************************/
	std::string ActionCreateAction::GetClassName() const
	{
		return mClassName;
	}

	/************************************************************************/
	void ActionCreateAction::SetClassName(const std::string& class_name)
	{
		mClassName = class_name;
	}

	/************************************************************************/
	std::string ActionCreateAction::GetInstanceName() const
	{
		return mInstanceName;
	}

	/************************************************************************/
	void ActionCreateAction::SetInstanceName(const std::string& instance_name)
	{
		mInstanceName = instance_name;
	}

	/************************************************************************/
	void ActionCreateAction::Update(WorldState& worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		Attributed* context = GetPredecessor();
		if (context)
		{
			CreateAction(mClassName, mInstanceName, *context);
		}	
	}

	/************************************************************************/
	void ActionCreateAction::InitPrescribedAttributes()
	{
		Action::InitPrescribedAttributes();
		AddExternalAttribute(CLASS_NAME_ENTRY_NAME, &mClassName, 1);
		AddExternalAttribute(INSTANCE_NAME_ENTRY_NAME, &mInstanceName, 1);
	}
}
