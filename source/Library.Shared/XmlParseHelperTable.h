#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"

namespace Library
{
	/** XmlParseHelperTable is an implementation of IXmlParseHelper that handles Tables or also known as Scopes.
	* This class is used by XmlParseMaster to parse Scope elements.
	* @see RTTI
	* @see IXmlParseHelper
	* @see XmlParseMaster
	* @see Scope
	*/
	class XmlParseHelperTable final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperTable, IXmlParseHelper)

	public:

		/** XmlParseHelperTable constructor.
		*/
		XmlParseHelperTable() = default;

		/** XmlParseHelperTable destructor.
		*/
		~XmlParseHelperTable() = default;

		XmlParseHelperTable(const XmlParseHelperTable & rhs) = delete;
		XmlParseHelperTable& operator=(const XmlParseHelperTable& rhs) = delete;

		/** Initializes the XmlParseHelperTable to its default state.
		* Its purpose is to reset the XmlParseHelperTable before each parse.
		*/
		virtual void Initialize() override;

		/** Handles the StartElementHandler method of XmlParseMaster, and by extension, StartElement callback of Expat.
		* This is called to try handling the start of a Scope Xml element. This helper builds a hierarchy of scopes populated with other elements.
		* @exception An exception is thrown if the element does not have a name attribute.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @param attributes: The attributes contained within the current element.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see Scope
		*/
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		/** Handles the EndElementHandler method of XmlParseMaster, and by extension, EndElement callback of Expat.
		* This is called to try handling the end of a Scope Xml element.
		* @exception An exception is thrown if the helper is not already parsing a Scope element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see Scope
		*/
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el) override;

		/** XmlParseHelperTable clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		virtual IXmlParseHelper* Clone() override;


	private:

		const static std::string SCOPE_ELEMENT_NAME;
		const static std::string NAME_ATTRIBUTE_NAME;

		bool mIsParsing;
	};

}