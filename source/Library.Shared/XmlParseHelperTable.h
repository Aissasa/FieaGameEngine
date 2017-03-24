#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"

namespace Library
{
	class XmlParseHelperTable final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperTable, IXmlParseHelper)

	public:

		XmlParseHelperTable() = default;
		~XmlParseHelperTable() = default;
		XmlParseHelperTable(const XmlParseHelperTable & rhs) = delete;
		XmlParseHelperTable& operator=(const XmlParseHelperTable& rhs) = delete;

		virtual void Initialize() override;

		virtual bool StartElementHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& el, const Library::HashMap<std::string, std::string>& attributes) override;

		virtual bool EndElementHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& el) override;

		virtual bool CharDataHandler(SharedDataC& sharedData, const std::string& str) override;

		virtual IXmlParseHelper* Clone() override;


	private:

		const static std::string SCOPE_ELEMENT_NAME;
		const static std::string NAME_ATTRIBUTE_NAME;

		bool mIsParsing;
	};

}