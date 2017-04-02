#pragma once
#include "IXmlParseHelper.h"

namespace Library
{
	/** XmlParseHelperWorld is an implementation of IXmlParseHelper that handles Worlds.
	* This class is used by XmlParseMaster to parse World elements.
	* @see RTTI
	* @see IXmlParseHelper
	* @see XmlParseMaster
	* @see World
	*/
	class XmlParseHelperWorld final : public IXmlParseHelper
	{
		
		RTTI_DECLARATIONS(XmlParseHelperWorld, IXmlParseHelper);

	public:

		/** XmlParseHelperWorld constructor.
		*/
		XmlParseHelperWorld() = default;

		/** XmlParseHelperWorld destructor.
		*/
		~XmlParseHelperWorld() = default;

		XmlParseHelperWorld(const XmlParseHelperWorld & rhs) = delete;
		XmlParseHelperWorld& operator=(const XmlParseHelperWorld& rhs) = delete;

		/** Initializes the XmlParseHelperWorld to its default state.
		* Its purpose is to reset the XmlParseHelperWorld before each parse.
		*/
		virtual void Initialize() override;

		/** Handles the StartElementHandler method of XmlParseMaster, and by extension, StartElement callback of Expat.
		* This is called to try handling the start of a World Xml element.
		* @exception An exception is thrown if the element does not have a name attribute.
		* @exception An exception is thrown if the element is not the root element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @param attributes: The attributes contained within the current element.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see World
		*/
		virtual bool StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		/** Handles the EndElementHandler method of XmlParseMaster, and by extension, EndElement callback of Expat.
		* This is called to try handling the end of a World Xml element.
		* @exception An exception is thrown if the helper is not already parsing a World element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see World
		*/
		virtual bool EndElementHandler(SharedDataC& sharedData, const std::string& el) override;

		/** XmlParseHelperWorld clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		virtual IXmlParseHelper* Clone() override;

	private:

		const static std::string WORLD_ELEMENT_NAME;
		const static std::string NAME_ATTRIBUTE_NAME;
		bool mIsParsing;
	};
}
