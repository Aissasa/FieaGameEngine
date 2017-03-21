#pragma once

#include "RTTI.h"
#include "XmlParseMaster.h"

namespace UnitTestLibraryDesktop
{
	class TestSharedData final : public Library::XmlParseMaster::SharedData
	{
		RTTI_DECLARATIONS(TestSharedData, Library::XmlParseMaster::SharedData)

	public:

		typedef Library::XmlParseMaster::SharedData SharedDataC;

		TestSharedData(Library::XmlParseMaster* xmlParseMaster = nullptr);
		~TestSharedData() = default;
		TestSharedData(const TestSharedData & rhs) = delete;
		TestSharedData& operator=(const TestSharedData& rhs) = delete;

		virtual SharedDataC* Clone() override;

		virtual void Initialize() override;

		void UpdateMaxDepth();

		std::uint32_t GetMaxDepth() const;

		std::uint32_t mDamage;

	private:

		std::uint32_t mMaxDepth;
	};

}
