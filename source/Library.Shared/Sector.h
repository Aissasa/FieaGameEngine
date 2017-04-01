#pragma once
#include "Attributed.h"
#include "EntityFactory.h"

namespace Library
{
	class WorldState;
	class World;
	class Entity;

	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed);

	public:

		Sector();
		~Sector() = default;
		Sector(const Sector & rhs) = delete;
		Sector& operator=(const Sector& rhs) = delete;

		std::string Name() const;
		void SetName(const std::string& name);

		Datum& Entities() const;

		Entity& CreateEntity(const std::string& entityClassName, const std::string& entityInstanceName);

		World* GetWorld() const;
		void SetWorld(World& world);

		void Update(WorldState& worldState);

		const static std::string ENTITIES_ENTRY_NAME;

	protected:
		void InitPrescribedAttributes() override;

	private:

		const static std::string SECTOR_NAME_ENTRY_NAME;
		std::string mName;

		Datum* mEntitiesDatumPtr;
	};

}
