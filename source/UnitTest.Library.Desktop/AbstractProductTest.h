#pragma once

#include "RTTI.h"

namespace UnitTestLibraryDesktop
{
	class AbstractProductTest : public Library::RTTI
	{
		RTTI_DECLARATIONS(AbstractProductTest, RTTI)

	public:

		AbstractProductTest(const std::uint32_t number = 0);
		virtual ~AbstractProductTest() = default;
		AbstractProductTest(const AbstractProductTest & rhs) = default;
		AbstractProductTest& operator=(const AbstractProductTest& rhs) = default;

		std::uint32_t GetNumber() const;
		virtual void SetNumber(const std::uint32_t number) = 0;

	protected:

		std::uint32_t mNumber;

	};
}