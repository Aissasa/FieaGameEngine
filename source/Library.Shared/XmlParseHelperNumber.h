#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"

namespace Library
{
	/** XmlParseHelperNumber is an implementation of IXmlParseHelper that handles numbers like Integers and Floats.
	* This class is used by XmlParseMaster to parse Integer and Floats elements.
	* @see RTTI
	* @see IXmlParseHelper
	* @see XmlParseMaster
	*/
	class XmlParseHelperNumber final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperNumber, IXmlParseHelper)

	public:

		/** XmlParseHelperNumber constructor.
		*/
		XmlParseHelperNumber();

		/** XmlParseHelperNumber destructor.
		*/
		~XmlParseHelperNumber() = default;

		XmlParseHelperNumber(const XmlParseHelperNumber & rhs) = delete;
		XmlParseHelperNumber& operator=(const XmlParseHelperNumber& rhs) = delete;

		/** Initializes the XmlParseHelperNumber to its default state.
		* Its purpose is to reset the XmlParseHelperNumber before each parse.
		*/
		virtual void Initialize() override;

		/** Handles the StartElementHandler method of XmlParseMaster, and by extension, StartElement callback of Expat.
		* This is called to try handling the start of an Integer or a Float Xml element.
		* @exception An exception is thrown if the element does not have a name attribute.
		* @exception An exception is thrown if the helper is already parsing another number element. Nested number elements are not allowed.
		* @exception An exception is thrown if the number element is nested in another element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @param attributes: The attributes contained within the current element.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		*/
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		/** Handles the EndElementHandler method of XmlParseMaster, and by extension, EndElement callback of Expat.
		* This is called to try handling the end of an Integer or a Float Xml element.
		* @exception An exception is thrown if the helper is not already parsing a number element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		*/
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el) override;

		/** Handles the CharDataHandler method of XmlParseMaster, and by extension, CharData callback of Expat.
		* This is called to try handling the content of an Integer or a Float Xml element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param str: Content of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		*/
		virtual bool CharDataHandler(XmlParseMaster::SharedData& sharedData, const std::string& str) override;

		/** XmlParseHelperNumber clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
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