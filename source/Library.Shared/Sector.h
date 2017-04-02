#pragma once
#include "Attributed.h"
#include "EntityFactory.h"

namespace Library
{
	class WorldState;
	class World;
	class Entity;

	/** Sector facilitates segmenting the World into modules with multiple datas.
	* It contains entities under it.
	* It inherits from Attributed.
	* @see Attributed
	* @see World
	* @see Entity
	*/
	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed);

	public:

		/** Sector Constructor.
		*/
		Sector();

		/** Sector Destructor.
		*/
		~Sector() = default;

		Sector(const Sector & rhs) = delete;
		Sector& operator=(const Sector& rhs) = delete;

		/** Gets the name of the Sector.
		* @return Name of the Sector.
		*/
		std::string Name() const;

		/** Sets the name of the Sector.
		* @param name: New name of the Sector to set.
		*/
		void SetName(const std::string& name);

		/** Gets the Datum containing the Sector's Entities.
		* @return Sector Entities.
		* @see Entity
		* @see Datum
		*/
		Datum& Entities() const;

		/** Creates a new Entity and adopts it into the World.
		* Needs the existance of the correspondant factory to work successfully.
		* @exception An exception is thrown if the entity creation fails.
		* @param entityClassName: Name of the Entity class to create an instance from.
		* @param entityInstanceName: Name of the new Entity instance.
		* @return Newly created Entity instance.
		* @see Entity
		*/
		Entity& CreateEntity(const std::string& entityClassName, const std::string& entityInstanceName);

		/** Gets the World that the Sector belongs to.
		* @return Parent World.
		* @see World
		*/
		World* GetWorld() const;

		/** Sets a new World that the Sector belongs to.
		* @param world: New Parent World.
		* @see World
		*/
		void SetWorld(World& world);

		/** Iterates through the contained entities and calls their update method.
		* @param worldState: Wolrd state to use for updates.
		* @see Entity
		* @see WorldState
		*/
		void Update(WorldState& worldState);

		/** Key under which the entities are stored in the World.
		* @see Entity
		*/
		const static std::string ENTITIES_ENTRY_NAME;

	protected:
		void InitPrescribedAttributes() override;

	private:

		const static std::string SECTOR_NAME_ENTRY_NAME;
		std::string mName;

		Datum* mEntitiesDatumPtr;
	};

}
