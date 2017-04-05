#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);

	public:

		ActionList();
		virtual ~ActionList() = default;
		ActionList(const ActionList & rhs) = delete;
		ActionList& operator=(const ActionList& rhs) = delete;

		Datum& Actions() const;

		Action& CreateAction(const std::string& actionClassName, const std::string& actionInstanceName);

		bool DestroyAction(const WorldState& worldState, const std::string& actionInstanceName) const;

		virtual void Update(WorldState& worldState) override;

	protected:
		void InitPrescribedAttributes() override;

	private:

		Datum* mActionsDatumPtr;
	};

	ActionConcreteFactory(ActionList);
}
