#pragma once

#include "HashMap.h"
#include "IXmlParseHelper.h"
#include "XmlParseMaster.h"

namespace UnitTestLibraryDesktop
{
	class TestXmlParseHelper final : public Library::IXmlParseHelper
	{

		RTTI_DECLARATIONS(TestXmlParseHelper, Library::IXmlParseHelper)

	public:

		TestXmlParseHelper() = default;
		~TestXmlParseHelper() = default;
		TestXmlParseHelper(const TestXmlParseHelper & rhs) = delete;
		TestXmlParseHelper& operator=(const TestXmlParseHelper& rhs) = delete;

		virtual void Initialize() override;

		virtual bool StartElementHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& el, const Library::HashMap<std::string, std::string>& attributes) override;

		virtual bool EndElementHandler(Library::XmlParseMaster::SharedData& sharedData, const std::string& el) override;

		virtual bool CharDataHandler(SharedDataC& sharedData, const std::string& str) override;

		virtual IXmlParseHelper* Clone() override;

	};
}