#include "pch.h"
#include "IXmlParseHelper.h"
#include <expat.h>

namespace Library
{

	/************************************************************************/
	void IXmlParseHelper::Initialize()
	{
	}

	/************************************************************************/
	bool IXmlParseHelper::StartElementHandler(const std::string & el, const HashMap<std::string, std::string>& attributes)
	{
		return false;
	}

	/************************************************************************/
	bool IXmlParseHelper::EndElementHandler(const std::string & el)
	{
		return false;
	}

	/************************************************************************/
	bool IXmlParseHelper::CharDataHandler(const char *& str, const std::uint32_t length)
	{
		return false;
	}
}