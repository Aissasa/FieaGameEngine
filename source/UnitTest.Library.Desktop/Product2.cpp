#include "pch.h"
#include "Product2.h"

namespace UnitTestLibraryDesktop
{
	/************************************************************************/
	Product2::Product2(std::uint32_t number) :
		AbstractProductTest(number)
	{
	}

	/************************************************************************/
	void Product2::SetNumber(const std::uint32_t number)
	{
		mNumber = number * 2;
	}
}