#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"

namespace Library
{
	class XmlParseHelperString final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperString, IXmlParseHelper)

	public:

		XmlParseHelperString();
		~XmlParseHelperString() = default;
		XmlParseHelperString(const XmlParseHelperString & rhs) = delete;
		XmlParseHelperString& operator=(const XmlParseHelperString& rhs) = delete;

		virtual void Initialize() override;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el) override;

		virtual bool CharDataHandler(SharedDataC& sharedData, const std::string& str) override;

		virtual IXmlParseHelper* Clone() override;


	private:

		const static std::string STRING_ELEMENT_NAME;
		const static std::string NAME_ELEMENT_NAME;
		const static std::string VALUE_ATTRIBUTE_NAME;

		std::string mNameString;
		std::string mDataString;
		bool mDataGotSet;
		bool mIsParsing;

		void Reset();
	};
}