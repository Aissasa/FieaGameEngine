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

		typedef std::function<void(WorldSharedData&, const HashMap<std::string, std::string>&)> StartFunc;
		typedef std::pair < std::string, StartFunc> StartFuncPair;
		typedef std::function<void(WorldSharedData&)> EndFunc;
		typedef std::pair < std::string, EndFunc> EndFuncPair;


		/** XmlParseHelperAction constructor.
		*/
		XmlParseHelperAction();

		/** XmlParseHelperAction destructor.
		*/
		~XmlParseHelperAction() = default;

		XmlParseHelperAction(const XmlParseHelperAction & rhs) = delete;
		XmlParseHelperAction& operator=(const XmlParseHelperAction& rhs) = delete;

		/** Initializes the XmlParseHelperAction to its default state.
		* Its purpose is to reset the XmlParseHelperAction before each parse.
		*/
		void Initialize() override;

		/** Handles the StartElementHandler method of XmlParseMaster, and by extension, StartElement callback of Expat.
		* This is called to try handling the start of an Action Xml element.
		* @exception An exception is thrown if the element does not have a name attribute.
		* @exception An exception is thrown if the element does not have a class name attribute.
		* @exception An exception is thrown if the element is not nested.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @param attributes: The attributes contained within the current element.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see Action
		*/
		bool StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		/** Handles the EndElementHandler method of XmlParseMaster, and by extension, EndElement callback of Expat.
		* This is called to try handling the end of an Action Xml element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see Action
		*/
		bool EndElementHandler(SharedDataC& sharedData, const std::string& el) override;

		/** XmlParseHelperAction clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		IXmlParseHelper* Clone() override;

	private:

		const static std::string ACTION_ELEMENT_NAME;
		const static std::string REACTION_ELEMENT_NAME;
		const static std::string ACTION_EVENT_ELEMENT_NAME;
		const static std::string CLASS_NAME_ATTRIBUTE_NAME;
		const static std::string INSTANCE_NAME_ATTRIBUTE_NAME;
		const static std::string SUB_TYPE_ATTRIBUTE_NAME;
		const static std::string DELAY_ATTRIBUTE_NAME;

		HashMap<std::string, StartFunc> mStartFunctionsMap;

		void InitializeFunctionsMaps();

		static void HandleActionElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes);
		static void HandleReactionElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes);
		static void HandleActionEventElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes);
	};
}
