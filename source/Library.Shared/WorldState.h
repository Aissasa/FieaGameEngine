#pragma once

#include "GameTime.h"

namespace Library
{
	class World;
	class Sector;
	class Entity;
	class Action;

    /** WorldState represnts the state of processing a World.
	* It serves the role of passing data between various layers in the world.
	* @see World
	* @see Sector
	* @see Entity
	* @see Action
	* @see GameTime
	*/
	class WorldState final
	{
	public:

		/** WorldState Constructor.
		 */
		WorldState();

		/** WorldState Constructor.
		*/
		~WorldState() = default;

		WorldState(const WorldState & rhs) = delete;
		WorldState& operator=(const WorldState& rhs) = delete;

		/** Gets the current World.
		* @return Current World.
		* @see World
		*/
		World* GetWorld() const;

		/** Sets the current World.
		* @param world: New World to set.
		* @see World
		*/
		void SetWorld(World* world);

		/** Gets the current Sector.
		* @return Current Sector.
		* @see Sector
		*/
		Sector* GetSector() const;

		/** Sets the current Sector.
		* @param sector: New Sector to set.
		* @see Sector
		*/
		void SetSector(Sector* sector);

		/** Gets the current Entity.
		* @return Current Entity.
		* @see Entity
		*/
		Entity* GetEntity() const;

		/** Sets the current Entity.
		* @param entity: New Entity to set.
		* @see Entity
		*/
		void SetEntity(Entity* entity);

		/** Gets the current Action.
		* @return Current Action.
		* @see Action
		*/
		Action* GetAction() const;

		/** Sets the current Action.
		* @param entity: New Action to set.
		* @see Action
		*/
		void SetAction(Action* action);

		/** Gets the current GameTime.
		* @return Current GameTime.
		* @see GameTime
		*/
		GameTime& GetGameTime();

		/** Sets the current GameTime.
		* @param gameTime: New GameTime to set.
		* @see GameTime
		*/
		void SetGameTime(const GameTime& gameTime);

	private:

		World*  mWorld;
		Sector* mSector;
		Entity* mEntity;
		Action* mAction;

		GameTime mGameTime;
	};

}