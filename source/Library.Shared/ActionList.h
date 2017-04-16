#pragma once

#include "Action.h"
#include "Factory.h"

namespace Library
{
	/** ActionList represnts an Action that contains other actions.
	* It inherits from Action.
	* @see Action
	*/
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);

	public:

		/** ActionList Constructor.
		*/
		ActionList();

		/** ActionList Destructor.
		*/
		virtual ~ActionList() = default;

		ActionList(const ActionList & rhs) = delete;
		ActionList& operator=(const ActionList& rhs) = delete;

		/** Gets the Datum containing the ActionList's Actions.
		* @return ActionList Actions.
		* @see Action
		* @see Datum
		*/
		Datum& Actions() const;

		/** Creates a new Action and adopts it into the ActionList.
		* Needs the existance of the correspondant factory to work successfully.
		* @exception An exception is thrown if the Action creation fails.
		* @param actionClassName: Name of the Action class to create an instance from.
		* @param actionInstanceName: Name of the new Action instance.
		* @return Newly created Action instance.
		* @see Action
		*/
		Action& CreateAction(const std::string& actionClassName, const std::string& actionInstanceName);

		/** Marks an Action in the ActionList for delete.
		* The action will be deleted at the end of the world update.
		* @param worldState: worldState used to get the world where the action exist.
		* @param actionInstanceName: Name of the Action instance to delete.
		* @return Boolean expressing the success of the mark for delete.
		* @see Action
		*/
		bool DestroyAction(const WorldState& worldState, const std::string& actionInstanceName) const;

		/** Searches for an action in the ActionList Actions
		* @param actionInstanceName: Name of the Action instance to find.
		* @return Found action.
		* @see Action
		*/
		Action* FindNestedActionByName(const std::string& actionInstanceName) const;

		/** ActionList's update method. Iterates through the contained Actions and calls their update methods.
		* @param worldState: Wolrd state to use.
		* @see WorldState
		*/
		virtual void Update(WorldState& worldState) override;

	protected:

		void InitPrescribedAttributes() override;
		Datum* mActionsDatumPtr;

	};

	ActionConcreteFactory(ActionList);
}
