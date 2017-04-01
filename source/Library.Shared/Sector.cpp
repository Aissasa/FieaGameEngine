#include "pch.h"
#include "Sector.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "World.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(Sector);

	const string Sector::ENTITIES_ENTRY_NAME = "Entities";
	const string Sector::SECTOR_NAME_ENTRY_NAME = "Name";

	/************************************************************************/
	Sector::Sector():
		mEntitiesDatumPtr(nullptr)
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
		entity->SetSector(*this);
		AddNestedScopeAttribute(ENTITIES_ENTRY_NAME, entity);

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
		assert(mEntitiesDatumPtr != nullptr);
		worldState.SetSector(this);
		uint32_t size = mEntitiesDatumPtr->Size();
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
		mEntitiesDatumPtr = &(AddEmptyNestedScopeAttribute(ENTITIES_ENTRY_NAME));
	}
}
