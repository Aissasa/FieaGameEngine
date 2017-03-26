#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"

namespace Library
{
	/** XmlParseHelperMatrix is an implementation of IXmlParseHelper that handles 4x4 Matrices.
	* This class is used by XmlParseMaster to parse Matrix4x4 elements.
	* @see RTTI
	* @see IXmlParseHelper
	* @see XmlParseMaster
	*/
	class XmlParseHelperMatrix final : public IXmlParseHelper
	{
		RTTI_DECLARATIONS(XmlParseHelperMatrix, IXmlParseHelper)

	public:

		/** XmlParseHelperMatrix constructor.
		*/
		XmlParseHelperMatrix();

		/** XmlParseHelperMatrix destructor.
		*/
		~XmlParseHelperMatrix() = default;

		XmlParseHelperMatrix(const XmlParseHelperMatrix & rhs) = delete;
		XmlParseHelperMatrix& operator=(const XmlParseHelperMatrix& rhs) = delete;

		/** Initializes the XmlParseHelperMatrix to its default state.
		* Its purpose is to reset the XmlParseHelperString before each parse.
		*/
		virtual void Initialize() override;

		/** Handles the StartElementHandler method of XmlParseMaster, and by extension, StartElement callback of Expat.
		* This is called to try handling the start of a Matrix Xml element.
		* @exception An exception is thrown if the element does not have a name attribute.
		* @exception An exception is thrown if the helper is already parsing another Matrix element. Nested Matrix elements are not allowed.
		* @exception An exception is thrown if the Matrix element is nested in another element.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @param attributes: The attributes contained within the current element.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		*/
		virtual bool StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes) override;

		/** Handles the EndElementHandler method of XmlParseMaster, and by extension, EndElement callback of Expat.
		* This is called to try handling the end of a Matrix Xml element.
		* @exception An exception is thrown if the helper is not already parsing a Matrix element.
		* @exception An exception is thrown if the TableSharedData holds less vectors than the required number of vectors.
		* @param sharedData: SharedData of the XmlParseMaster and its handlers.
		* @param el: Name of the current element in the Xml.
		* @return Boolean representing if the handler was able to handle XmlParseMaster request.
		* @see SharedData
		* @see XmlParseMaster
		*/
		virtual bool EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string& el) override;

		/** XmlParseHelperMatrix clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		virtual IXmlParseHelper* Clone() override;


	private:

		const static std::string MATRIX_ELEMENT_NAME;
		const static std::string NAME_ELEMENT_NAME;
		const static std::uint32_t EXPECTED_NUMBER_OF_VECTORS = 4;

		std::string mNameString;
		bool mIsParsing;

		void Reset();
	};
}