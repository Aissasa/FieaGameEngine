#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace Library
{
	class Sector;

	/** Entity represnts a unit that contains several data and behaviors.
	* It inherits from Attributed.
	* @see Attributed
	*/
	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)

	public:

		/** Entity Constructor.
		*/
		Entity();

		/** Entity Destructor.
		*/
		virtual ~Entity() = default;

		Entity(const Entity& rhs) = delete;
		Entity& operator=(const Entity& rhs) = delete;

		/** Gets the name of the Entity.
		* @return Name of the Entity.
		*/
		std::string Name() const;

		/** Sets the name of the Entity.
		* @param name: New name of the Entity to set.
		*/
		void SetName(const std::string& name);

		/** Gets the Datum containing the Entity's Actions.
		* @return Entity Actions.
		* @see Action
		* @see Datum
		*/
		Datum& Actions() const;

		/** Creates a new Action and adopts it into the Entity.
		* Needs the existance of the correspondant factory to work successfully.
		* @exception An exception is thrown if the Action creation fails.
		* @param actionClassName: Name of the Action class to create an instance from.
		* @param actionInstanceName: Name of the new Action instance.
		* @return Newly created Action instance.
		* @see Action
		*/
		Action& CreateAction(const std::string& actionClassName, const std::string& actionInstanceName);

		/** Marks an Action in the Entity for delete.
		* The action will be deleted at the end of the world update.
		* @param worldState: worldState used to get the world where the action exist.
		* @param actionInstanceName: Name of the Action instance to delete.
		* @return Boolean expressing the success of the mark for delete.
		* @see Action
		*/
		bool DestroyAction(const WorldState& worldState, const std::string& actionInstanceName) const;

		/** Gets the Sector that the Entity belongs to.
		* @return Parent Sector.
		* @see Sector
		*/
		Sector* GetSector() const;

		/** Sets a new Sector that the Entity belongs to.
		* @param sector: New Parent Sector.
		* @see Sector
		*/
		void SetSector(Sector& sector);

		/** Entity's update method. Iterates through the contained Action and calls their update methods.
		* @param worldState: Wolrd state to use.
		* @see WorldState
		*/
		virtual void Update(WorldState& worldState);

	protected:
		void InitPrescribedAttributes() override;

	private:

		const static std::string ENTITY_NAME_ENTRY_NAME;
		const static std::string ACTIONS_ENTRY_NAME;
		std::string mName;

		Datum* mActionsDatumPtr;
	};

#define EntityConcreteFactory(EntityDerivedClass) \
	ConcreteFactory(Library::Entity, EntityDerivedClass);
}
