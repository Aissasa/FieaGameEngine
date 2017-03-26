#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"

namespace Library
{
	/** XmlParseHelperVector is an implementation of IXmlParseHelper that handles Vector4s.
	* This class is used by XmlParseMaster to parse Vector4 of elements.
	* @see RTTI
	* @see IXmlParseHelper
	* @see XmlParseMaster
	*/
	class XmlParseHelperVector final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperVector, IXmlParseHelper)

	public:

		/** XmlParseHelperVector constructor.
		*/
		XmlParseHelperVector();

		/** XmlParseHelperVector destructor.
		*/
		~XmlParseHelperVector() = default;

		XmlParseHelperVector(const XmlParseHelperVector & rhs) = delete;
		XmlParseHelperVector& operator=(const XmlParseHelperVector& rhs) = delete;

		/** Initializes the XmlParseHelperVector to its default state.
		* Its purpose is to reset the XmlParseHelperVector before each parse.
		*/
		virtual void Initialize() override;

		/** Handles the StartElementHandler method of XmlParseMaster, and by extension, StartElement callback of Expat.
		* This is called to try handling the start of a Vector4 Xml element.
		* @exception An exception is thrown if the element does not have a name attribute.
		* @exception An exception is thrown if the element is missing one of the coordinates attributes(x, y, z, w).
		* @exception An exception is thrown if the helper is already parsing another Vector4 element. Nested Vector4 elements are not allowed.
		* @exception An exception is thrown if the Vector4 element is nested in another element aside from a Matrix4x4.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @param attributes: The attributes contained within the current element.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		*/
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		/** Handles the EndElementHandler method of XmlParseMaster, and by extension, EndElement callback of Expat.
		* This is called to try handling the end of a Vector4 Xml element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		*/
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el) override;

		/** XmlParseHelperVector clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		virtual IXmlParseHelper* Clone() override;


	private:

		const static std::string VECTOR_ELEMENT_NAME;
		const static std::string NAME_ELEMENT_NAME;
		const static std::string X_ATTRIBUTE_NAME;
		const static std::string Y_ATTRIBUTE_NAME;
		const static std::string Z_ATTRIBUTE_NAME;
		const static std::string W_ATTRIBUTE_NAME;

		bool mIsParsing;
		bool mIsXSet;
		bool mIsYSet;
		bool mIsZSet;
		bool mIsWSet;

		void Reset();
	};
}