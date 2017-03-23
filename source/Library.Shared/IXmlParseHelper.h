#pragma once

#include "HashMap.h"
#include "RTTI.h"
#include "XmlParseMaster.h"

namespace Library
{

	/** IXmlParseHelper is an abstract class that is used to parse a certain Xml grammar.
	* This class is used by XmlParseMaster to parse Xml. It uses Expat for parsing.
	* @see RTTI
	* @see XmlParseMaster
	*/
	class IXmlParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IXmlParseHelper, RTTI)

	public:

		typedef XmlParseMaster::SharedData SharedDataC;

		/** IXmlParseHelper default constructor.
		*/
		IXmlParseHelper() = default;

		/** IXmlParseHelper destructor.
		*/
		virtual ~IXmlParseHelper() = default;

		IXmlParseHelper(const IXmlParseHelper& rhs) = delete;
		IXmlParseHelper& operator=(const IXmlParseHelper& rhs) = delete;

		/** Initializes the IXmlParseHelper to its default state.
		* Its purpose is to reset the IXmlParseHelper after each parse.
		*/
		virtual void Initialize();

		/** Handles the StartElementHandler method of XmlParseMaster, and by extension, StartElement callback of Expat.
		* This is called to handle the start of an Xml element.
		* This method is pure virtual.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @param attributes: The attributes contained within the current element.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		*/
		virtual bool StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) = 0;

		/** Handles the EndElementHandler method of XmlParseMaster, and by extension, EndElement callback of Expat.
		* This is called to handle the end of an Xml element.
		* This method is pure virtual.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		*/
		virtual bool EndElementHandler(SharedDataC& sharedData, const std::string& el) = 0;

		/** Handles the CharDataHandler method of XmlParseMaster, and by extension, CharData callback of Expat.
		* This is called to handle the content of an Xml element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param str: Content of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		*/
		virtual bool CharDataHandler(SharedDataC& sharedData, const std::string& str);

		/** IXmlParseHelper clone method.
		* Pure virtual method that serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		virtual IXmlParseHelper* Clone() = 0;

		/** StartElementHandler counter.
		*/
		std::uint32_t mStartElementHandlerCount;

		/** EndElementHandler counter.
		*/
		std::uint32_t mEndElementHandlerCount;

		/** CharDataHandler counter.
		*/
		std::uint32_t mCharDataHandlerCount;
	};
}