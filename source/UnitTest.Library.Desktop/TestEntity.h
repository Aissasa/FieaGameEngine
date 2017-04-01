#pragma once
#include "Entity.h"
#include "Factory.h"

namespace UnitTestLibraryDesktop
{
	class TestEntity final : public Library::Entity
	{
		RTTI_DECLARATIONS(TestEntity, Library::Entity);

	public:

		TestEntity();
		~TestEntity() = default;
		TestEntity(const TestEntity& rhs) = delete;
		TestEntity& operator=(const TestEntity& rhs) = delete;

		void Update(Library::WorldState& worldState) override;


		std::int32_t GetNumber() const;
		void SetNumber(const std::int32_t number);

	protected:

		void InitPrescribedAttributes() override;

	private:

		std::int32_t mNumber;

	};

	EntityConcreteFactory(TestEntity);
}
