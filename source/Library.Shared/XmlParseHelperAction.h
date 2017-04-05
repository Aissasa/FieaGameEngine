#pragma once
#include "IXmlParseHelper.h"
#include "WorldSharedData.h"

namespace Library
{
	/** XmlParseHelperAction is an implementation of IXmlParseHelper that handles Actions.
	* This class is used by XmlParseMaster to parse Action elements.
	* @see RTTI
	* @see IXmlParseHelper
	* @see XmlParseMaster
	* @see Action
	*/
	class XmlParseHelperAction final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperAction, IXmlParseHelper);

	public:

		/** XmlParseHelperAction constructor.
		*/
		XmlParseHelperAction();

		/** XmlParseHelperAction destructor.
		*/
		~XmlParseHelperAction() = default;

		XmlParseHelperAction(const XmlParseHelperAction & rhs) = delete;
		XmlParseHelperAction& operator=(const XmlParseHelperAction& rhs) = delete;

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

		/** XmlParseHelperAction clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		IXmlParseHelper* Clone() override;

	private:

		const static std::string ACTION_ELEMENT_NAME;
		const static std::string CLASS_NAME_ATTRIBUTE_NAME;
		const static std::string INSTANCE_NAME_ATTRIBUTE_NAME;

		Vector<WorldSharedData::State> mPreviousStates;
	};
}
