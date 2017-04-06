#pragma once
#include "Attributed.h"

namespace Library
{
	class WorldState;
	class Sector;
	class Action;

	/** World is a container for an interactive simulation. 
	* It defines a global scope, contains a list of Sectors and an event queue.
	* It inherits from Attributed.
	* @see Attributed
	* @see Sector
	*/
	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed);

		typedef std::pair<Datum*, std::uint32_t> DatumAndIndexPair;

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

		/** Gets the Datum containing the World's Actions.
		* @return World Actions.
		* @see Action
		* @see Datum
		*/
		Datum& Actions() const;

		/** Creates a new Action and adopts it into the World.
		* Needs the existance of the correspondant factory to work successfully.
		* @exception An exception is thrown if the Action creation fails.
		* @param actionClassName: Name of the Action class to create an instance from.
		* @param actionInstanceName: Name of the new Action instance.
		* @return Newly created Action instance.
		* @see Action
		*/
		Action& CreateAction(const std::string& actionClassName, const std::string& actionInstanceName);

		/** Marks an Action in the World for delete.
		* The action will be deleted at the end of the world update.
		* @param worldState: worldState used to get the world where the action exist.
		* @param actionInstanceName: Name of the Action instance to delete.
		* @return Boolean expressing the success of the delete.
		* @see Action
		*/
		bool DestroyAction(const WorldState& worldState, const std::string& actionInstanceName) const;

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

		/** Marks a certain action for delete.
		* The delete operation of the actions added happens after the updates are done.
		* @param action: A datum and index pair representing the action to destroy.
		* @see Action
		* @see Datum
		*/
		void AddActionToDestory(Datum& datumContainingAction, const std::uint32_t index);

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

		const static std::string ACTIONS_ENTRY_NAME;
		const static std::string WORLD_NAME_ENTRY_NAME;
		std::string mName;

		Datum* mActionsDatumPtr;
		Datum* mSectorsDatumPtr;
		Vector<DatumAndIndexPair> mActionsToDestroy;

		void DeleteActions();
	};
}
