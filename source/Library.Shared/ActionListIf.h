#pragma once
#include "ActionList.h"

namespace Library
{
	class ActionListIf final : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList);

	public:

		ActionListIf();
		~ActionListIf() = default;
		ActionListIf(const ActionListIf & rhs) = delete;
		ActionListIf& operator=(const ActionListIf& rhs) = delete;

		std::string GetConditionIntegerName() const;

		void SetConditionIntegerName(const std::string& condition_integer_name);

		Action& CreateThenAction(const std::string& actionClassName);

		bool DestroyThenAction(const WorldState& worldState) const;

		Action& CreateElseAction(const std::string& actionClassName);

		bool DestroyElseAction(const WorldState& worldState) const;


		void Update(WorldState& worldState) override;

	protected:

		void InitPrescribedAttributes() override;
	
	private:

		const static std::string CONDITION_INTEGER_NAME;
		const static std::string THEN_ACTION_NAME;
		const static std::string ELSE_ACTION_NAME;
		std::string mConditionIntegerName;

	};

	ActionConcreteFactory(ActionListIf);
}
