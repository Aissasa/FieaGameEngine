#pragma once

#include "Reaction.h"

namespace Library
{
	/** ReactionAttributed is an implementation of Reaction. 
	* It works in conjuction with ActionEvent to create a mechanism to create events in script.
	* @see RTTI
	* @see Reaction
	* @see ActionEvent
	*/
	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);

	public:

		/** ReactionAttributed constructor.
		* @param subType: subtype of the ReactionAttributed. Defaulted to an empty string.
		*/
		ReactionAttributed(const std::string& subType = "");

		/** ReactionAttributed destructor.
		*/
		virtual ~ReactionAttributed() = default;

		ReactionAttributed(const ReactionAttributed & rhs) = delete;
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = delete;

		/** Notify executes a certain logic when the Event<EventMessageAttributed> it is subscribed to is fired.
		* @param eventPublisher: The event that got fired.
		* @see EventPublisher
		* @see Event
		*/
		virtual void Notify(EventPublisher& eventPublisher) override;

		/** Gets the subtype of the EventMessageAttributed.
		* @return Subtype of the EventMessageAttributed.
		*/
		std::string GetSubType() const;

		/** Sets the subtype of the EventMessageAttributed.
		* @param subType: New subtype of the EventMessageAttributed to set.
		*/
		void SetSubType(const std::string& subType);

	protected:

		virtual void Update(WorldState& worldState) override;
		virtual void InitPrescribedAttributes() override;

	private:

		static const std::string SUB_TYPE_KEY;

		std::string mSubType;
	};

	ReactionConcreteFactory(ReactionAttributed);
}
