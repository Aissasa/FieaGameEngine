#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"

namespace Library
{
	class XmlParseHelperVector final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperVector, IXmlParseHelper)

	public:

		XmlParseHelperVector();
		~XmlParseHelperVector() = default;
		XmlParseHelperVector(const XmlParseHelperVector & rhs) = delete;
		XmlParseHelperVector& operator=(const XmlParseHelperVector& rhs) = delete;

		virtual void Initialize() override;

		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el) override;

		virtual bool CharDataHandler(SharedDataC& sharedData, const std::string& str) override;

		virtual IXmlParseHelper* Clone() override;


	private:

		const static std::string VECTOR_ELEMENT_NAME;
		const static std::string NAME_ELEMENT_NAME;
		const static std::string X_ATTRIBUTE_NAME;
		const static std::string Y_ATTRIBUTE_NAME;
		const static std::string Z_ATTRIBUTE_NAME;
		const static std::string W_ATTRIBUTE_NAME;

		bool mIsParsing;
		bool mIsXSet;
		bool mIsYSet;
		bool mIsZSet;
		bool mIsWSet;

		void Reset();
	};
}