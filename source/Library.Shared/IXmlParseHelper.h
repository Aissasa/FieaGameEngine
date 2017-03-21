#pragma once

#include "HashMap.h"
#include "RTTI.h"
#include "XmlParseMaster.h"

namespace Library
{
	class IXmlParseHelper : public RTTI
	{

		RTTI_DECLARATIONS(IXmlParseHelper, RTTI)

	public:

		typedef XmlParseMaster::SharedData SharedDataC;

		IXmlParseHelper() = default;
		virtual ~IXmlParseHelper() = default;

		IXmlParseHelper(const IXmlParseHelper& rhs) = delete;
		IXmlParseHelper& operator=(const IXmlParseHelper& rhs) = delete;

		virtual void Initialize() = 0;

		virtual bool StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) = 0;

		virtual bool EndElementHandler(SharedDataC& sharedData, const std::string& el) = 0;

		virtual bool CharDataHandler(SharedDataC& sharedData, const std::string& str);

		virtual IXmlParseHelper* Clone() = 0;

		std::uint32_t mStartElementHandlerCount;
		std::uint32_t mEndElementHandlerCount;
		std::uint32_t mCharDataHandlerCount;
	};
}