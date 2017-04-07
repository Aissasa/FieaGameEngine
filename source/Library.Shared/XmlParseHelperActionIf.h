#pragma once

#include "IXmlParseHelper.h"
#include "WorldSharedData.h"

namespace Library
{
	/** XmlParseHelperActionIf is an implementation of IXmlParseHelper that handles ActionListIfs.
	* This class is used by XmlParseMaster to parse ActionListIf elements.
	* @see RTTI
	* @see IXmlParseHelper
	* @see XmlParseMaster
	* @see ActionListIf
	*/
	class XmlParseHelperActionIf final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperActionIf, IXmlParseHelper);

	public:

		typedef std::function<void(WorldSharedData&, const HashMap<std::string, std::string>&)> StartFunc;
		typedef std::pair < std::string, StartFunc> StartFuncPair;
		typedef std::function<void(WorldSharedData&)> EndFunc;
		typedef std::pair < std::string, EndFunc> EndFuncPair;


		/** XmlParseHelperActionIf constructor.
		*/
		XmlParseHelperActionIf();

		/** XmlParseHelperActionIf destructor.
		*/
		~XmlParseHelperActionIf() = default;

		XmlParseHelperActionIf(const XmlParseHelperActionIf & rhs) = delete;
		XmlParseHelperActionIf& operator=(const XmlParseHelperActionIf& rhs) = delete;

		/** Initializes the XmlParseHelperActionIf to its default state.
		* Its purpose is to reset the XmlParseHelperActionIf before each parse.
		*/
		void Initialize() override;

		/** Handles the StartElementHandler method of XmlParseMaster, and by extension, StartElement callback of Expat.
		* This is called to try handling the start of an If Xml element.
		* @exception An exception is thrown if the element is not nested.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @param attributes: The attributes contained within the current element.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see ActionListIf
		*/
		bool StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		/** Handles the EndElementHandler method of XmlParseMaster, and by extension, EndElement callback of Expat.
		* This is called to try handling the end of an If Xml element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see ActionListIf
		*/
		bool EndElementHandler(SharedDataC& sharedData, const std::string& el) override;

		/** XmlParseHelperActionIf clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		IXmlParseHelper* Clone() override;

	private:

		const static std::string ACTION_LIST_IF_CLASS_NAME;

		const static std::string IF_ELEMENT_NAME;
		const static std::string INSTANCE_NAME_ATTRIBUTE_NAME;
		const static std::string CLASS_NAME_ATTRIBUTE_NAME;

		const static std::string CONDITION_ELEMENT_NAME;
		const static std::string CONDITION_ELEMENT_VALUE_ATTRIBUTE_NAME;
		const static std::string THEN_ELEMENT_NAME;
		const static std::string ELSE_ELEMENT_NAME;

		HashMap<std::string, StartFunc> mStartFunctionsMap;
		HashMap<std::string, EndFunc> mEndFunctionsMap;

		void InitializeFunctionsMaps();

		static void HandleIfElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes);
		static void HandleConditionElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes);
		static void HandleThenElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes);
		static void HandleElseElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes);

		static void HandleIfElementEnd(WorldSharedData& sharedData);
		static void HandleThenElementEnd(WorldSharedData& sharedData);
		static void HandleElseElementEnd(WorldSharedData& sharedData);

	};
}
