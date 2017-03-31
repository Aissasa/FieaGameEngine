#pragma once
#include "Attributed.h"
#include "WorldState.h"
#include "Factory.h"

namespace Library
{
	class Sector;

	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)

	public:

		Entity();
		virtual ~Entity() = default;
		Entity(const Entity& rhs);
		Entity& operator=(const Entity& rhs);

		std::string Name() const;
		void SetName(const std::string& name);

		Sector* GetSector() const;
		void SetSector(Sector& sector);

		void Update(WorldState& worldState);

	protected:
		void InitPrescribedAttributes() override;

	private:

		const static std::string ENTITY_NAME_ENTRY_NAME;
		std::string mName;
	};

#define EntityConcreteFactory(EntityDerivedClass) \
	ConcreteFactory(Entity, EntityDerivedClass);
}
