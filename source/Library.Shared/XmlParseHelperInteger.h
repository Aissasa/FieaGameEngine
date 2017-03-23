#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"

namespace Library
{
	class XmlParseHelperInteger final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperInteger, IXmlParseHelper)

	public:

		XmlParseHelperInteger();
		~XmlParseHelperInteger() = default;
		XmlParseHelperInteger(const XmlParseHelperInteger & rhs) = delete;
		XmlParseHelperInteger& operator=(const XmlParseHelperInteger& rhs) = delete;

		virtual void Initialize() override;

		virtual bool StartElementHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& el, const Library::HashMap<std::string, std::string>& attributes) override;

		virtual bool EndElementHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& el) override;

		virtual bool CharDataHandler(SharedDataC& sharedData, const std::string& str) override;

		virtual IXmlParseHelper* Clone() override;


	private:

		const static std::string INTEGER_ELEMENT_NAME;
		const static std::string NAME_ELEMENT_NAME;
		const static std::string VALUE_ATTRIBUTE_NAME;

		std::string mNameString;
		std::string mDataString;
		bool mDataGotSet;
		bool mIsParsing;

		void Reset();
	};
}