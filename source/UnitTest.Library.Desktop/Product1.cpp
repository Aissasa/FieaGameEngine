#include "pch.h"
#include "Product1.h"

namespace UnitTestLibraryDesktop
{
	/************************************************************************/
	Product1::Product1(std::uint32_t number):
		AbstractProductTest(number)
	{
	}

	/************************************************************************/
	void Product1::SetNumber(const std::uint32_t number)
	{
		mNumber = number;
	}
}