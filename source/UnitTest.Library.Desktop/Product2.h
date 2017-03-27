#pragma once

#include "AbstractProductTest.h"

namespace UnitTestLibraryDesktop
{
	class Product2 final : public AbstractProductTest
	{
		RTTI_DECLARATIONS(Product2, AbstractProductTest)

	public:

		Product2(std::uint32_t number = 0);
		~Product2() = default;
		Product2(const Product2 & rhs) = default;
		Product2& operator=(const Product2& rhs) = default;

		virtual void SetNumber(const std::uint32_t number) override;
	};
}
