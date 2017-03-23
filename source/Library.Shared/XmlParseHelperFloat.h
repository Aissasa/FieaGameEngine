#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"

namespace Library
{
	class XmlParseHelperFloat final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperFloat, IXmlParseHelper)

	public:

		XmlParseHelperFloat();
		~XmlParseHelperFloat() = default;
		XmlParseHelperFloat(const XmlParseHelperFloat & rhs) = delete;
		XmlParseHelperFloat& operator=(const XmlParseHelperFloat& rhs) = delete;

		virtual void Initialize() override;

		virtual bool StartElementHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& el, const Library::HashMap<std::string, std::string>& attributes) override;

		virtual bool EndElementHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& el) override;

		virtual bool CharDataHandler(SharedDataC& sharedData, const std::string& str) override;

		virtual IXmlParseHelper* Clone() override;


	private:

		const static std::string FLOAT_ELEMENT_NAME;
		const static std::string NAME_ELEMENT_NAME;
		const static std::string VALUE_ATTRIBUTE_NAME;

		std::string mNameString;
		std::string mDataString;
		bool mDataGotSet;
		bool mIsParsing;

		void Reset();
	};
}