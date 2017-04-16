#pragma once

#include "Action.h"
#include "Factory.h"

namespace Library
{
	/** ActionEvent is an implementation of Action. 
	* It works in conjuction with ReactionAttributed to create a mechanism to create events in script.
	* @see RTTI
	* @see Action
	* @see ReactionAttributed
	*/
	class ActionEvent : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action);

	public:

		/** ActionEvent constructor.
		* @param subType: subtype of the ActionEvent. Defaulted to an empty string.
		*/
		ActionEvent(const std::string& subType = "", const std::int32_t delay = 0);

		/** ActionEvent destructor.
		*/
		virtual ~ActionEvent() = default;

		ActionEvent(const ActionEvent & rhs) = delete;
		ActionEvent& operator=(const ActionEvent& rhs) = delete;

		/** ActionEvent's update method.
		* It create, configures and adds an event to the event queue.
		* @param worldState: Wolrd state to use.
		* @see WorldState
		*/
		virtual void Update(WorldState& worldState) override;

		/** Gets the subtype of the ActionEvent.
		* @return Subtype of the ActionEvent.
		*/
		std::string GetSubType() const;

		/** Sets the subtype of the ActionEvent.
		* @param subType: New subtype of the ActionEvent to set.
		*/
		void SetSubType(const std::string& subType);

		/** Gets the delay used by ActionEvent.
		* @return Delay used by ActionEvent.
		*/
		std::int32_t GetDelay() const;

		/** Sets the delay used by ActionEvent.
		* @param delay: New delay used to set.
		*/
		void SetDelay(const std::int32_t delay);

	protected:

		virtual void InitPrescribedAttributes() override;

	private:

		static const std::string SUB_TYPE_KEY;
		static const std::string DELAY_KEY;

		std::string mSubType;
		std::int32_t mDelay;
	};

	ActionConcreteFactory(ActionEvent);
}
