#include "pch.h"
#include "Sector.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "World.h"
#include "Action.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(Sector);

	const string Sector::ACTIONS_ENTRY_NAME = "Actions";
	const string Sector::ENTITIES_ENTRY_NAME = "Entities";
	const string Sector::SECTOR_NAME_ENTRY_NAME = "Name";

	/************************************************************************/
	Sector::Sector():
		mActionsDatumPtr(nullptr), mEntitiesDatumPtr(nullptr)
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	std::string Sector::Name() const
	{
		return mName;
	}

	/************************************************************************/
	void Sector::SetName(const string& name)
	{
		mName = name;
	}

	/************************************************************************/
	Datum& Sector::Actions() const
	{
		assert(mActionsDatumPtr != nullptr);
		return *mActionsDatumPtr;
	}

	/************************************************************************/
	Action& Sector::CreateAction(const std::string& actionClassName, const std::string& actionInstanceName)
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
	bool Sector::DestroyAction(const WorldState& worldState, const std::string& actionInstanceName) const
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
	Datum& Sector::Entities() const
	{
		assert(mEntitiesDatumPtr != nullptr);
		return *mEntitiesDatumPtr;
	}

	/************************************************************************/
	Entity& Sector::CreateEntity(const string& entityClassName, const string& entityInstanceName)
	{
		Entity* entity = Factory<Entity>::Create(entityClassName);
		if (!entity)
		{
			throw exception("The correspondant factory needs to be initialized.");
		}
		entity->SetName(entityInstanceName);
		Adopt(*entity, ENTITIES_ENTRY_NAME);

		return *entity;
	}

	/************************************************************************/
	World* Sector::GetWorld() const
	{
		Scope* parent = GetParent();
		if (!parent)
		{
			return nullptr;
		}

		assert(parent->Is(World::TypeIdClass()));
		return static_cast<World*>(parent);
	}

	/************************************************************************/
	void Sector::SetWorld(World& world)
	{
		world.Adopt(*this, world.SECTORS_ENTRY_NAME);
	}

	/************************************************************************/
	void Sector::Update(WorldState& worldState)
	{
		assert(mActionsDatumPtr != nullptr);
		assert(mEntitiesDatumPtr != nullptr);
		worldState.SetSector(this);

		// update actions
		uint32_t size = mActionsDatumPtr->Size();
		for (uint32_t i = 0; i < size; ++i)
		{
			assert((*mActionsDatumPtr)[i].Is(Action::TypeIdClass()));
			static_cast<Action&>((*mActionsDatumPtr)[i]).Update(worldState);
		}

		// update entities
		size = mEntitiesDatumPtr->Size();
		for (uint32_t i = 0; i < size; ++i)
		{
			assert((*mEntitiesDatumPtr)[i].Is(Entity::TypeIdClass()));
			static_cast<Entity&>((*mEntitiesDatumPtr)[i]).Update(worldState);
		}

		worldState.SetSector(nullptr);
	}

	/************************************************************************/
	void Sector::InitPrescribedAttributes()
	{
		AddExternalAttribute(SECTOR_NAME_ENTRY_NAME, &mName, 1);
		mActionsDatumPtr = &AddEmptyNestedScopeAttribute(ACTIONS_ENTRY_NAME);
		mEntitiesDatumPtr = &AddEmptyNestedScopeAttribute(ENTITIES_ENTRY_NAME);
	}
}
