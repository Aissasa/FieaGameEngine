#include "pch.h"
#include "TestEntity.h"

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(TestEntity);

	/************************************************************************/
	TestEntity::TestEntity():
		mNumber(0)
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	void TestEntity::Update(Library::WorldState& worldState)
	{
		Entity::Update(worldState);
		worldState.SetEntity(this);
		++mNumber;
		worldState.SetEntity(nullptr);
	}

	/************************************************************************/
	std::int32_t TestEntity::GetNumber() const
	{
		return mNumber;
	}

	/************************************************************************/
	void TestEntity::SetNumber(const std::int32_t number)
	{
		mNumber = number;
	}

	/************************************************************************/
	void TestEntity::InitPrescribedAttributes()
	{
		Entity::InitPrescribedAttributes();
		AddExternalAttribute("Number", &mNumber, 1);
	}
}
