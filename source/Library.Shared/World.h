#pragma once
#include "Attributed.h"

namespace Library
{
	class WorldState;
	class Sector;

	/** World is a container for an interactive simulation. 
	* It defines a global scope, contains a list of Sectors and an event queue.
	* It inherits from Attributed.
	* @see Attributed
	* @see Sector
	*/
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed);

	public:

		/** World Constructor.
		*/
		World();

		/** World Destructor.
		*/
		~World() = default;

		World(const World & rhs) = delete;
		World& operator=(const World& rhs) = delete;

		/** Gets the name of the World.
		* @return Name of the World.
		*/
		std::string Name() const;

		/** Sets the name of the World.
		* @param name: New name of the World to set.
		*/
		void SetName(const std::string& name);

		/** Gets the Datum containing the World's Sectors.
		* @return World Sectors.
		* @see Sector
		* @see Datum
		*/
		Datum& Sectors() const;

		/** Creates a new Sector and adopts it into the World.
		* @param sectorName: Name of the new Sector.
		* @return Newly created Sector.
		* @see Sector
		*/
		Sector& CreateSector(const std::string& sectorName);

		/** Iterates through the contained Sectors and calls their update method.
		* @param worldState: Wolrd state to use for updates.
		* @see Sector
		* @see WorldState
		*/
		void Update(WorldState& worldState);

		/** Key under which the sectors are stored in the World.
		* @see Sector
		*/
		const static std::string SECTORS_ENTRY_NAME;

	protected:
		void InitPrescribedAttributes() override;

	private:

		const static std::string WORLD_NAME_ENTRY_NAME;
		std::string mName;

		Datum* mSectorsDatumPtr;
	};
}
