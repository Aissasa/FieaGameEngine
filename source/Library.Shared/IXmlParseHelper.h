#pragma once

#include "HashMap.h"

namespace Library
{
	class IXmlParseHelper
	{
	public:

		IXmlParseHelper() = default;
		virtual ~IXmlParseHelper() = default;

		IXmlParseHelper(const IXmlParseHelper& rhs) = delete;
		IXmlParseHelper& operator=(const IXmlParseHelper& rhs) = delete;

		virtual void Initialize();

		virtual bool StartElementHandler(const std::string& el, const HashMap<std::string, std::string>& attributes);

		virtual bool EndElementHandler(const std::string& el);

		virtual bool CharDataHandler(const char*& str, const std::uint32_t length);

		virtual IXmlParseHelper* Clone() = 0;

	};
}