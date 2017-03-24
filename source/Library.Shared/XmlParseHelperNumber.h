#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"

namespace Library
{
	class XmlParseHelperNumber final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperNumber, IXmlParseHelper)

	public:

		XmlParseHelperNumber();
		~XmlParseHelperNumber() = default;
		XmlParseHelperNumber(const XmlParseHelperNumber & rhs) = delete;
		XmlParseHelperNumber& operator=(const XmlParseHelperNumber& rhs) = delete;

		virtual void Initialize() override;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el) override;

		virtual bool CharDataHandler(SharedDataC& sharedData, const std::string& str) override;

		virtual IXmlParseHelper* Clone() override;


	private:

		const static std::string INTEGER_ELEMENT_NAME;
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