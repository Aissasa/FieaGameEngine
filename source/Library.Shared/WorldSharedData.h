#pragma once
#include "RTTI.h"
#include "TableSharedData.h"

namespace Library
{
	/** WorldSharedData is an implementation of TableSharedData. It represents the shared data between the XmlParseMaster and the XmlParseHelpers related to World.
	* @see TableSharedData
	* @see XmlParseMaster
	* @see IXmlParseHelper
	*/
	class WorldSharedData final : public TableSharedData
	{
		RTTI_DECLARATIONS(WorldSharedData, TableSharedData)

	public:

		/** An enumaration represnting the state of the Xml Parsing.
		*/
		enum class State
		{
			NotParsing,
			WorldParsing,
			SectorParsing,
			EntityParsing
		};

		/** WorldSharedData Constructor.
		* Creates and initializes a WorldSharedData instance, and hooks it to the passed XmlParseMaster.
		* @param xmlParseMaster: XmlParseMaster to hook the instance to. Defaulted to nullptr.
		* @see XmlParseMaster
		*/
		WorldSharedData(XmlParseMaster* xmlParseMaster = nullptr);

		/** WorldSharedData Destructor.
		*/
		~WorldSharedData() = default;

		WorldSharedData(const WorldSharedData & rhs) = delete;
		WorldSharedData& operator=(const WorldSharedData& rhs) = delete;

		/** WorldSharedData clone method.
		* This serves as a virtual constructor. It returns a copy of the current instance.
		* @return Pointer to the copy instance.
		*/
		SharedData* Clone() override;

		/** Initializes the WorldSharedData to its default state.
		* Its purpose is to reset the WorldSharedData after each parse.
		*/
		void Initialize() override;

		/** Gets the current parsing state.
		 * @return Current parsing state.
		*/
		State GetCurrentState() const;

		/** Sets the current parsing state.
		* @param current_state: New parsing state to set.
		*/
		void SetCurrentState(const State current_state);

	private:

		State mCurrentState;
	};
}
