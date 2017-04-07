#pragma once

#include "Action.h"
#include "Factory.h"

namespace Library
{
	/** Action that handles Action destruction in Runtime.
	* @see Action
	*/
	class ActionDestroyAction final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action);

	public:

		/** ActionCreateAction Constructor.
		*/
		ActionDestroyAction();

		/** ActionCreateAction Destructor.
		*/
		~ActionDestroyAction() = default;

		ActionDestroyAction(const ActionDestroyAction & rhs) = delete;
		ActionDestroyAction& operator=(const ActionDestroyAction& rhs) = delete;

		/** Marks an Action in the context for delete.
		* The action will be deleted at the end of the world update.
		* @param worldState: worldState used to get the world where the action exist.
		* @param actionInstanceName: Name of the Action instance to delete.
		* @param context: Container of Action instance to delete.
		* @return Boolean expressing the success of the delete.
		* @see Action
		*/
		bool DestroyAction(const WorldState& worldState, const std::string& actionInstanceName, Attributed& context) const;

		/** Gets the Instance name of the action to destroy.
		* @return Instance name.
		* @see Action
		*/
		std::string GetInstanceName() const;

		/** Sets the Instance name of the action to destroy.
		* @param sector: New Instance name to use.
		* @see Action
		*/
		void SetInstanceName(const std::string& instance_name);

		/** ActionDestroyAction's update method.
		* Marks an action for destruction.
		* @param worldState: Wolrd state to use.
		* @see WorldState
		* @see Action
		*/
		virtual void Update(WorldState& worldState) override;

	protected:

		void InitPrescribedAttributes() override;

	private:

		const static std::string INSTANCE_NAME_ENTRY_NAME;
		std::string mInstanceName;
	};

	ActionConcreteFactory(ActionDestroyAction);
}
