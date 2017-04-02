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

		/** Entity's update method.
		* @param worldState: Wolrd state to use.
		* @see WorldState
		*/
		virtual void Update(WorldState& worldState);

	protected:
		void InitPrescribedAttributes() override;

	private:

		const static std::string ENTITY_NAME_ENTRY_NAME;
		std::string mName;
	};

#define EntityConcreteFactory(EntityDerivedClass) \
	ConcreteFactory(Library::Entity, EntityDerivedClass);
}
