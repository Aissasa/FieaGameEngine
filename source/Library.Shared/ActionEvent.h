#pragma once

#include "Action.h"

namespace Library
{
	class ActionEvent : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action);

	public:

		ActionEvent(const std::string& subType = "", const std::int32_t delay = 0);
		~ActionEvent() = default;
		ActionEvent(const ActionEvent & rhs) = delete;
		ActionEvent& operator=(const ActionEvent& rhs) = delete;

		virtual void Update(WorldState& worldState) override;


		std::string GetSubType() const;
		void SetSubType(const std::string& subType);
		std::int32_t GetDelay() const;
		void SetDelay(const std::int32_t delay);
	protected:

		virtual void InitPrescribedAttributes() override;

	private:

		static const std::string SUB_TYPE_KEY;
		static const std::string DELAY_KEY;

		std::string mSubType;
		std::int32_t mDelay;
	};

#define ActionEventConcreteFactory(ActionEventDerivedClass) \
	ConcreteFactory(Library::ActionEvent, ActionEventDerivedClass);

}
