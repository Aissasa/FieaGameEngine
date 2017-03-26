#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"

namespace Library
{
	class XmlParseHelperMatrix final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperMatrix, IXmlParseHelper)

	public:

		XmlParseHelperMatrix();
		~XmlParseHelperMatrix() = default;
		XmlParseHelperMatrix(const XmlParseHelperMatrix & rhs) = delete;
		XmlParseHelperMatrix& operator=(const XmlParseHelperMatrix& rhs) = delete;

		virtual void Initialize() override;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el) override;

		//virtual bool CharDataHandler(SharedDataC& sharedData, const std::string& str) override;

		virtual IXmlParseHelper* Clone() override;


	private:

		const static std::string MATRIX_ELEMENT_NAME;
		const static std::string NAME_ELEMENT_NAME;
		const static std::uint32_t EXPECTED_NUMBER_OF_VECTORS = 4;

		std::string mNameString;
		bool mIsParsing;

		void Reset();
	};
}