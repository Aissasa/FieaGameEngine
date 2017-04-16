#pragma once

#include "Attributed.h"

namespace Library
{
	class WorldState;

	/** EventMessageAttributed represents the payload that the event subscribed to by ReactionAttributed uses.
	* It derives from Attributed.
	* @see RTTI
	* @see Attributed
	* @see ReactionAttributed
	*/
	class EventMessageAttributed : public Attributed
	{	
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);

	public:

		/** EventMessageAttributed constructor.
		* @param subType: subtype of the EventMessageAttributed. Defaulted to an empty string.
		* @param worldState: The WorldState stored by EventMessageAttributed. Defaulted to nullptr.
		* @see WorldState
		*/
		EventMessageAttributed(const std::string& subType = "", WorldState* worldState = nullptr);

		/** EventMessageAttributed destructor.
		*/
		virtual ~EventMessageAttributed() = default;

		/** EventMessageAttributed copy constructor.
		*/
		EventMessageAttributed(const EventMessageAttributed & rhs) = default;

		/** EventMessageAttributed copy assignment operator.
		*/
		EventMessageAttributed& operator=(const EventMessageAttributed& rhs) = default;

		/** Gets the subtype of the EventMessageAttributed.
		* @return Subtype of the EventMessageAttributed.
		*/
		std::string GetSubType() const;

		/** Sets the subtype of the EventMessageAttributed.
		* @param subType: New subtype of the EventMessageAttributed to set.
		*/
		void SetSubType(const std::string& subType);

		/** Gets the worldState of the EventMessageAttributed.
		* @return WorldState of the EventMessageAttributed.
		* @see WorldState
		*/
		WorldState* GetWorldState() const;

		/** Sets the worldState of the EventMessageAttributed.
		* @param worldState: New worldState of the EventMessageAttributed to set.
		* @see WorldState
		*/
		void SetWorldState(WorldState& worldState);

	protected:

		virtual void InitPrescribedAttributes() override;

	private:

		static const std::string SUB_TYPE_KEY;
		static const std::string WORLD_KEY;

		std::string mSubType;
		WorldState* mWorldState;

	};
}