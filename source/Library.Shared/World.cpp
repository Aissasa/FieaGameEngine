#include "pch.h"
#include "World.h"
#include "Sector.h"
#include "WorldState.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(World);

	const string World::SECTORS_ENTRY_NAME = "Sectors";
	const string World::WORLD_NAME_ENTRY_NAME = "Name";

	/************************************************************************/
	World::World():
		mSectorsDatumPtr(nullptr)
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	string World::Name() const
	{
		return mName;
	}

	/************************************************************************/
	void World::SetName(const std::string& name)
	{
		mName = name;
	}

	/************************************************************************/
	Datum& World::Sectors() const
	{
		assert(mSectorsDatumPtr != nullptr);
		return *mSectorsDatumPtr;
	}

	/************************************************************************/
	Sector& World::CreateSector(const string& sectorName)
	{
		Sector* sector = new Sector();
		assert(sector != nullptr);
		sector->SetName(sectorName);
		sector->SetWorld(*this);
		AddNestedScopeAttribute(SECTORS_ENTRY_NAME, sector);

		return *sector;
	}

	/************************************************************************/
	void World::Update(WorldState& worldState)
	{
		assert(mSectorsDatumPtr != nullptr);
		worldState.SetWorld(this);
		uint32_t size = mSectorsDatumPtr->Size();
		for (uint32_t i = 0; i < size; ++i)
		{
			assert((*mSectorsDatumPtr)[i].Is(Sector::TypeIdClass()));
			static_cast<Sector&>((*mSectorsDatumPtr)[i]).Update(worldState);
		}
		//worldState.SetWorld(nullptr);
	}

	/************************************************************************/
	void World::InitPrescribedAttributes()
	{
		AddExternalAttribute(WORLD_NAME_ENTRY_NAME, &mName, 1);
		mSectorsDatumPtr = &(AddEmptyNestedScopeAttribute(SECTORS_ENTRY_NAME));
	}
}
