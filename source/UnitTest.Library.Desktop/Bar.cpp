#include "pch.h"
#include "Bar.h"

namespace UnitTestLibraryDesktop
{
	/************************************************************************/
	Bar::Bar(std::float_t fl):
		mFloat(fl)
	{
	}

	/************************************************************************/
	std::float_t Bar::GetFloat() const
	{
		return mFloat;
	}

	/************************************************************************/
	void Bar::SetFloat(std::float_t fl)
	{
		mFloat = fl;
	}
}