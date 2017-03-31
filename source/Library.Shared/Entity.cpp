#include "pch.h"
#include "Entity.h"
#include "Sector.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(Entity);

	const string Entity::ENTITY_NAME_ENTRY_NAME = "Name";

	/************************************************************************/
	Entity::Entity()
	{
		Entity::InitPrescribedAttributes();
	}

	/************************************************************************/
	Entity::Entity(const Entity& rhs):
		Attributed(rhs)
	{
		mName = rhs.mName;
	}

	/************************************************************************/
	Entity& Entity::operator=(const Entity& rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(rhs);
			mName = rhs.mName;
		}

		return *this;
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
		UNREFERENCED_PARAMETER(worldState);
	}

	/************************************************************************/
	void Entity::InitPrescribedAttributes()
	{
		AddExternalAttribute(ENTITY_NAME_ENTRY_NAME, &mName, 1);
	}
}
