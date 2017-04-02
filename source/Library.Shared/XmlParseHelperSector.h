#pragma once
#include "IXmlParseHelper.h"

namespace Library
{
	/** XmlParseHelperSector is an implementation of IXmlParseHelper that handles Sectors.
	* This class is used by XmlParseMaster to parse Sector elements.
	* @see RTTI
	* @see IXmlParseHelper
	* @see XmlParseMaster
	* @see Sector
	*/
	class XmlParseHelperSector final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperSector, IXmlParseHelper);

	public:

		/** XmlParseHelperSector constructor.
		*/
		XmlParseHelperSector() = default;

		/** XmlParseHelperSector destructor.
		*/
		~XmlParseHelperSector() = default;

		XmlParseHelperSector(const XmlParseHelperSector & rhs) = delete;
		XmlParseHelperSector& operator=(const XmlParseHelperSector& rhs) = delete;

		/** Initializes the XmlParseHelperSector to its default state.
		* Its purpose is to reset the XmlParseHelperSector before each parse.
		*/
		void Initialize() override;

		/** Handles the StartElementHandler method of XmlParseMaster, and by extension, StartElement callback of Expat.
		* This is called to try handling the start of a Sector Xml element.
		* @exception An exception is thrown if the element does not have a name attribute.
		* @exception An exception is thrown if the element is not the root element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @param attributes: The attributes contained within the current element.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see Sector
		*/
		bool StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		/** Handles the EndElementHandler method of XmlParseMaster, and by extension, EndElement callback of Expat.
		* This is called to try handling the end of a Sector Xml element.
		* @exception An exception is thrown if the helper is not already parsing a Sector element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see Sector
		*/
		bool EndElementHandler(SharedDataC& sharedData, const std::string& el) override;

		/** XmlParseHelperSector clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		IXmlParseHelper* Clone() override;

	private:

		const static std::string SECTOR_ELEMENT_NAME;
		const static std::string NAME_ATTRIBUTE_NAME;

		bool mIsParsing;
	};
}
