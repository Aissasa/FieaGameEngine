#pragma once
#include "ActionList.h"

namespace Library
{
	/** ActionListIf represnts an if block. Depending on the condition, one of the two actions is executed.
	* It inherits from ActionList.
	* @see ActionList
	*/
	class ActionListIf final : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList);

	public:

		/** ActionListIf Constructor.
		*/
		ActionListIf();

		/** ActionListIf Destructor.
		*/
		~ActionListIf() = default;

		ActionListIf(const ActionListIf & rhs) = delete;
		ActionListIf& operator=(const ActionListIf& rhs) = delete;

		/** Gets the Condition Integer Name.
		* @return Name of the Condition Integer.
		*/
		std::string GetConditionIntegerName() const;

		/** Sets the name of the Condition Integer.
		* @param name: New name of the Condition Integer to set.
		*/
		void SetConditionIntegerName(const std::string& condition_integer_name);

		/** Creates the Then Action and adopts it.
		* Needs the existance of the correspondant factory to work successfully.
		* @exception An exception is thrown if the Action creation fails.
		* @param actionClassName: Name of the Action class to create an instance from.
		* @return Newly created Then Action instance.
		* @see Action
		*/
		Action& CreateThenAction(const std::string& actionClassName);


		/** Marks the Then Action for delete.
		* The action will be deleted at the end of the world update.
		* @param worldState: worldState used to get the world where the action exist.
		* @return Boolean expressing the success of the mark for delete.
		* @see Action
		*/
		bool DestroyThenAction(const WorldState& worldState) const;

		/** Creates the Else Action and adopts it.
		* Needs the existance of the correspondant factory to work successfully.
		* @exception An exception is thrown if the Action creation fails.
		* @param actionClassName: Name of the Action class to create an instance from.
		* @return Newly created Else Action instance.
		* @see Action
		*/
		Action& CreateElseAction(const std::string& actionClassName);

		/** Marks the Else Action for delete.
		* The action will be deleted at the end of the world update.
		* @param worldState: worldState used to get the world where the action exist.
		* @return Boolean expressing the success of the mark for delete.
		* @see Action
		*/
		bool DestroyElseAction(const WorldState& worldState) const;

		/** ActionListIf's update method. It updates only one of the Actions depending on the condition.
		* @exception An exception is thrown if the Action that is supposed to be updated does not exist.
		* @param worldState: Wolrd state to use.
		* @see WorldState
		* @see Action
		*/
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
