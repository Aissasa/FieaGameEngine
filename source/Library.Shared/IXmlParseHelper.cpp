#include "pch.h"
#include "IXmlParseHelper.h"
#include <expat.h>

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(IXmlParseHelper)

	/************************************************************************/
	bool IXmlParseHelper::CharDataHandler(SharedDataC& sharedData, const std::string& str)
	{
		UNREFERENCED_PARAMETER(sharedData);
		UNREFERENCED_PARAMETER(str);
		return false;
	}
}