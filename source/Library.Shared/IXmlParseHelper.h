#pragma once

#include "HashMap.h"

namespace Library
{
	class IXmlParseHelper
	{
	public:

		IXmlParseHelper();

		virtual ~IXmlParseHelper();

		IXmlParseHelper(const IXmlParseHelper& rhs) = delete;
		IXmlParseHelper& operator=(const IXmlParseHelper& rhs) = delete;

		virtual void Initialize();

		virtual bool StartElementHandler(const std::string& str, const HashMap<std::string, std::string>& map);

		virtual bool EndElementHandler(const std::string& str);

		virtual bool CharDataHandler(const char*& str, const std::uint32_t length);

		virtual IXmlParseHelper* Clone() = 0;

	};
}