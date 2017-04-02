#pragma once
#include "IXmlParseHelper.h"

namespace Library
{
	/** XmlParseHelperEntity is an implementation of IXmlParseHelper that handles Entities.
	* This class is used by XmlParseMaster to parse Entity elements.
	* @see RTTI
	* @see IXmlParseHelper
	* @see XmlParseMaster
	* @see Entity
	*/
	class XmlParseHelperEntity final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperEntity, IXmlParseHelper);

	public:

		/** XmlParseHelperEntity constructor.
		*/
		XmlParseHelperEntity() = default;

		/** XmlParseHelperEntity destructor.
		*/
		~XmlParseHelperEntity() = default;

		XmlParseHelperEntity(const XmlParseHelperEntity & rhs) = delete;
		XmlParseHelperEntity& operator=(const XmlParseHelperEntity& rhs) = delete;

		/** Initializes the XmlParseHelperEntity to its default state.
		* Its purpose is to reset the XmlParseHelperEntity before each parse.
		*/
		void Initialize() override;

		/** Handles the StartElementHandler method of XmlParseMaster, and by extension, StartElement callback of Expat.
		* This is called to try handling the start of a Entity Xml element.
		* @exception An exception is thrown if the element does not have a name attribute.
		* @exception An exception is thrown if the element is not the root element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @param attributes: The attributes contained within the current element.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see Entity
		*/
		bool StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		/** Handles the EndElementHandler method of XmlParseMaster, and by extension, EndElement callback of Expat.
		* This is called to try handling the end of a Entity Xml element.
		* @exception An exception is thrown if the helper is not already parsing a Entity element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see Entity
		*/
		bool EndElementHandler(SharedDataC& sharedData, const std::string& el) override;

		/** XmlParseHelperEntity clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		IXmlParseHelper* Clone() override;

	private:

		const static std::string ENTITY_ELEMENT_NAME;
		const static std::string CLASS_NAME_ATTRIBUTE_NAME;
		const static std::string INSTANCE_NAME_ATTRIBUTE_NAME;

		bool mIsParsing;
	};
}
