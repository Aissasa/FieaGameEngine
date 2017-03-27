#include "pch.h"
#include "AbstractProductTest.h"

using namespace std;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(AbstractProductTest)

	/************************************************************************/
	AbstractProductTest::AbstractProductTest(const std::uint32_t number):
		mNumber(number)
	{
	}

	/************************************************************************/
	std::uint32_t AbstractProductTest::GetNumber() const
	{
		return mNumber;
	}
}