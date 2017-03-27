#pragma once

#include "AbstractProductTest.h"

namespace UnitTestLibraryDesktop
{
	class Product1 final : public AbstractProductTest
	{
		RTTI_DECLARATIONS(Product1, AbstractProductTest)

	public:

		Product1(std::uint32_t number = 0);
		~Product1() = default;
		Product1(const Product1 & rhs) = default;
		Product1& operator=(const Product1& rhs) = default;

		virtual void SetNumber(const std::uint32_t number) override;
	};
}
