#pragma once

#include "GameTime.h"

namespace Library
{
	class World;
	class Sector;
	class Entity;
	//class Action;

	class WorldState final
	{
	public:

		WorldState();
		~WorldState() = default;
		WorldState(const WorldState & rhs) = delete;
		WorldState& operator=(const WorldState& rhs) = delete;

		World* GetWorld() const;
		void SetWorld(World* world);
		Sector* GetSector() const;
		void SetSector(Sector* sector);
		Entity* GetEntity() const;
		void SetEntity(Entity* entity);
		GameTime GetGameTime() const;
		void SetGameTime(const GameTime& game_time);

	private:

		World*  mWorld;
		Sector* mSector;
		Entity* mEntity;
		//Action* mAction;

		GameTime mGameTime;
	};

}