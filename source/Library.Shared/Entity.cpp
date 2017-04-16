#include "pch.h"
#include "Entity.h"
#include "Sector.h"
#include "Action.h"
#include "World.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(Entity);

	const string Entity::ENTITY_NAME_ENTRY_NAME = "Name";
	const string Entity::ACTIONS_ENTRY_NAME = "Actions";


	/************************************************************************/
	Entity::Entity():
		mActionsDatumPtr(nullptr)
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	std::string Entity::Name() const
	{
		return mName;
	}

	/************************************************************************/
	void Entity::SetName(const std::string& name)
	{
		mName = name;
	}

	/************************************************************************/
	Datum& Entity::Actions() const
	{
		assert(mActionsDatumPtr != nullptr);
		return *mActionsDatumPtr;
	}

	/************************************************************************/
	Action& Entity::CreateAction(const std::string& actionClassName, const std::string& actionInstanceName)
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
	bool Entity::DestroyAction(const WorldState& worldState, const std::string& actionInstanceName) const
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
					worldState.GetWorld()->AddActionToDestroy(action);
					return true;
				}
			}
		}

		return false;
	}

	/************************************************************************/
	Sector* Entity::GetSector() const
	{
		Scope* parent = GetParent();
		if (!parent)
		{
			return nullptr;
		}

		assert(parent->Is(Sector::TypeIdClass()));
		return static_cast<Sector*>(parent);
	}

	/************************************************************************/
	void Entity::SetSector(Sector& sector)
	{
		sector.Adopt(*this, sector.ENTITIES_ENTRY_NAME);
	}

	/************************************************************************/
	void Entity::Update(WorldState& worldState)
	{
		assert(mActionsDatumPtr != nullptr);
		worldState.SetEntity(this);

		// update actions
		uint32_t size = mActionsDatumPtr->Size();
		for (uint32_t i = 0; i < size; ++i)
		{
			assert((*mActionsDatumPtr)[i].Is(Action::TypeIdClass()));
			static_cast<Action&>((*mActionsDatumPtr)[i]).Update(worldState);
		}
		worldState.SetEntity(nullptr);
	}

	/************************************************************************/
	void Entity::InitPrescribedAttributes()
	{
		AddInternalAttribute("this", static_cast<RTTI*>(this));
		AddExternalAttribute(ENTITY_NAME_ENTRY_NAME, &mName, 1);
		mActionsDatumPtr = &AddEmptyNestedScopeAttribute(ACTIONS_ENTRY_NAME);
	}
}
