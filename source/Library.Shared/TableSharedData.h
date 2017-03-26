#pragma once

#include "RTTI.h"
#include "XmlParseMaster.h"
#include "Scope.h"

namespace Library
{
	/** TableSharedData is an implementation of SharedData. It represents the shared data between the XmlParseMaster and the XmlParseHelpers related to Scope.
	* @see RTTI
	* @see XmlParseMaster
	* @see IXmlParseHelper
	*/
	class TableSharedData final : public XmlParseMaster::SharedData
	{
		RTTI_DECLARATIONS(TableSharedData, XmlParseMaster::SharedData)

	public:

		/** TableSharedData Constructor.
		* Creates and initializes a TableSharedData instance, and hooks it to the passed XmlParseMaster.
		* @param xmlParseMaster: XmlParseMaster to hook the instance to. Defaulted to nullptr.
		*/
		TableSharedData(XmlParseMaster* xmlParseMaster = nullptr);

		/** TableSharedData Destructor.
		*/
		~TableSharedData();

		TableSharedData(const TableSharedData & rhs) = delete;
		TableSharedData& operator=(const TableSharedData& rhs) = delete;

		/** TableSharedData clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		virtual XmlParseMaster::SharedData* Clone() override;

		/** Initializes the TableSharedData to its default state.
		* Its purpose is to reset the TableSharedData after each parse.
		*/
		virtual void Initialize() override;

		/** Gets the Scope stored within the TableSharedData.
		* The scope gets constructed during the parsing with the elements available in the latter.
		* @return Associated Scope.
		* @see Scope
		*/
		Scope& GetScope() const;

		/** Sets a new Scope to the Scope stored within the TableSharedData.
		* @param scope: Scope to set.
		* @see Scope
		*/
		void SetScope(Scope& scope);

		/** A boolean representing if an element is being parsed.
		*/
		bool IsParsingElement;

		/** A boolean representing if an Matrix element is being parsed.
		*/
		bool IsParsingMatrix;

		/** A dynamic array(Vector) of Vector4s used to build Matrix4x4 elements.
		*/
		Vector<glm::vec4> MatrixVectors;

	private:

		Scope* mScope;
	};

}