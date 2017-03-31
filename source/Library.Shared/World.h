#pragma once
#include "Attributed.h"

namespace Library
{
	class WorldState;
	class Sector;

	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed);

	public:
		World();
		~World() = default;
		World(const World & rhs);
		World& operator=(const World& rhs);

		std::string Name() const;
		void SetName(const std::string& name);

		Datum& Sectors() const;

		Sector& CreateSector(const std::string& sectorName);

		void Update(WorldState& worldState);

		const static std::string SECTORS_ENTRY_NAME;

	protected:
		void InitPrescribedAttributes() override;

	private:

		const static std::string WORLD_NAME_ENTRY_NAME;
		std::string mName;

		Datum* mSectorsDatumPtr;
	};
}
