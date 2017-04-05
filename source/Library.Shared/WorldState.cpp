#include "pch.h"
#include "WorldState.h"

namespace Library
{
	/************************************************************************/
	WorldState::WorldState(): 
		mWorld(nullptr), mSector(nullptr), mEntity(nullptr)
	{
	}

	/************************************************************************/
	World* WorldState::GetWorld() const
	{
		return mWorld;
	}

	/************************************************************************/
	void WorldState::SetWorld(World* world)
	{
		mWorld = world;
	}

	/************************************************************************/
	Sector* WorldState::GetSector() const
	{
		return mSector;
	}

	/************************************************************************/
	void WorldState::SetSector(Sector* sector)
	{
		mSector = sector;
	}

	/************************************************************************/
	Entity* WorldState::GetEntity() const
	{
		return mEntity;
	}

	/************************************************************************/
	void WorldState::SetEntity(Entity* entity)
	{
		mEntity = entity;
	}

	/************************************************************************/
	Action* WorldState::GetAction() const
	{
		return mAction;
	}

	/************************************************************************/
	void WorldState::SetAction(Action* action)
	{
		mAction = action;
	}

	/************************************************************************/
	GameTime& WorldState::GetGameTime()
	{
		return mGameTime;
	}

	/************************************************************************/
	void WorldState::SetGameTime(const GameTime& gameTime)
	{
		mGameTime = gameTime;
	}
}
