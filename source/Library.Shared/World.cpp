#include "pch.h"
#include "World.h"
#include "Sector.h"
#include "WorldState.h"
#include "Action.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(World);

	const string World::ACTIONS_ENTRY_NAME = "Actions";
	const string World::SECTORS_ENTRY_NAME = "Sectors";
	const string World::WORLD_NAME_ENTRY_NAME = "Name";

	/************************************************************************/
	World::World():
		mActionsDatumPtr(nullptr) ,mSectorsDatumPtr(nullptr), mActionsToDestroy(), mEventQueue()
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

	EventQueue& World::GetEventQueue()
	{
		return mEventQueue;
	}

	/************************************************************************/
	Datum& World::Actions() const
	{
		assert(mActionsDatumPtr != nullptr);
		return *mActionsDatumPtr;
	}

	/************************************************************************/
	Action& World::CreateAction(const std::string& actionClassName, const std::string& actionInstanceName)
	{
		Action* action = Factory<Action>::Create(actionClassName);
		if (!action)
		{
			throw exception("The correspondent factory needs to be initialized.");
		}
		action->SetName(actionInstanceName);
		Adopt(*action, ACTIONS_ENTRY_NAME);

		return *action;
	}

	/************************************************************************/
	bool World::DestroyAction(const WorldState& worldState, const std::string& actionInstanceName) const
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
		Adopt(*sector, SECTORS_ENTRY_NAME);

		return *sector;
	}

	/************************************************************************/
	void World::AddActionToDestroy(Action& actionToDestroy)
	{
		mActionsToDestroy.PushBack(&actionToDestroy);
	}

	/************************************************************************/
	void World::Update(WorldState& worldState)
	{
		assert(mActionsDatumPtr != nullptr);
		assert(mSectorsDatumPtr != nullptr);

		// update the events queue
		mEventQueue.Update(worldState.GetGameTime());

		worldState.SetWorld(this);

		// update actions
		uint32_t size = mActionsDatumPtr->Size();
		for (uint32_t i = 0; i < size; ++i)
		{
			assert((*mActionsDatumPtr)[i].Is(Action::TypeIdClass()));
			static_cast<Action&>((*mActionsDatumPtr)[i]).Update(worldState);
		}

		// update sectors
		size = mSectorsDatumPtr->Size();
		for (uint32_t i = 0; i < size; ++i)
		{
			assert((*mSectorsDatumPtr)[i].Is(Sector::TypeIdClass()));
			static_cast<Sector&>((*mSectorsDatumPtr)[i]).Update(worldState);
		}

		DeleteActions();
		//worldState.SetWorld(nullptr);
	}

	/************************************************************************/
	void World::InitPrescribedAttributes()
	{
		AddInternalAttribute("this", static_cast<RTTI*>(this));
		AddExternalAttribute(WORLD_NAME_ENTRY_NAME, &mName, 1);
		mActionsDatumPtr = &(AddEmptyNestedScopeAttribute(ACTIONS_ENTRY_NAME));
		mSectorsDatumPtr = &(AddEmptyNestedScopeAttribute(SECTORS_ENTRY_NAME));
	}

	/************************************************************************/
	void World::DeleteActions()
	{
		for (auto& action : mActionsToDestroy)
		{
			delete action;
		}
		mActionsToDestroy.Empty();
	}
}
