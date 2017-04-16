#pragma once

#include "IXmlParseHelper.h"
#include "WorldSharedData.h"

namespace Library
{
	/** XmlParseHelperActionCreateAndDestroyAction is an implementation of IXmlParseHelper that handles ActionCreateActions and ActionDestroyActions.
	* This class is used by XmlParseMaster to parse Action elements.
	* @see RTTI
	* @see IXmlParseHelper
	* @see XmlParseMaster
	* @see ActionCreateAction
	* @see ActionDestroyAction
	*/
	class XmlParseHelperActionCreateAndDestroyAction final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperActionCreateAndDestroyAction, IXmlParseHelper);

	public:

		typedef std::function<void(WorldSharedData&, const HashMap<std::string, std::string>&)> StartFunc;
		typedef std::pair < std::string, StartFunc> StartFuncPair;

		/** XmlParseHelperActionCreateAndDestroyAction constructor.
		*/
		XmlParseHelperActionCreateAndDestroyAction();

		/** XmlParseHelperActionCreateAndDestroyAction destructor.
		*/
		~XmlParseHelperActionCreateAndDestroyAction() = default;

		XmlParseHelperActionCreateAndDestroyAction(const XmlParseHelperActionCreateAndDestroyAction & rhs) = delete;
		XmlParseHelperActionCreateAndDestroyAction& operator=(const XmlParseHelperActionCreateAndDestroyAction& rhs) = delete;

		/** Initializes the XmlParseHelperActionCreateAndDestroyAction to its default state.
		* Its purpose is to reset the XmlParseHelperActionCreateAndDestroyAction before each parse.
		*/
		void Initialize() override;

		/** Handles the StartElementHandler method of XmlParseMaster, and by extension, StartElement callback of Expat.
		* This is called to try handling the start of an ActionCreateAction and an ActionDestroyAction Xml elements.
		* @exception An exception is thrown if the element is not nested.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @param attributes: The attributes contained within the current element.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see ActionCreateAction
		* @see ActionDestroyAction
		*/
		bool StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		/** Handles the EndElementHandler method of XmlParseMaster, and by extension, EndElement callback of Expat.
		* This is called to try handling the end of an ActionCreateAction and an ActionDestroyAction Xml element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		* @see ActionCreateAction
		* @see ActionDestroyAction
		*/
		bool EndElementHandler(SharedDataC& sharedData, const std::string& el) override;

		/** XmlParseHelperActionCreateAndDestroyAction clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		IXmlParseHelper* Clone() override;

	private:

		const static std::string ACTION_CREATE_ACTION_ELEMENT_NAME;
		const static std::string ACTION_DESTROY_ACTION_ELEMENT_NAME;

		const static std::string NAME_ATTRIBUTE_NAME;
		const static std::string CLASS_NAME_ATTRIBUTE_NAME;
		const static std::string INSTANCE_NAME_ATTRIBUTE_NAME;

		HashMap<std::string, StartFunc> mStartFunctionsMap;

		void InitializeFunctionsMaps();

		static void HandleCreateElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes);
		static void HandleDestroyElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes);
	};
}
