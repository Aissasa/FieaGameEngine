#pragma once

#include "Action.h"
#include "Factory.h"

namespace Library
{
	/** Action that handles Action creation in Runtime.
	* @see Action
	*/
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action);

	public:

		/** ActionCreateAction Constructor.
		*/
		ActionCreateAction();

		/** ActionCreateAction Destructor.
		*/
		~ActionCreateAction() = default;

		ActionCreateAction(const ActionCreateAction & rhs) = delete;
		ActionCreateAction& operator=(const ActionCreateAction& rhs) = delete;

		/** Creates a new Action and adopts it in the context.
		* Needs the existance of the correspondant factory to work successfully.
		* @exception An exception is thrown if the Action creation fails.
		* @param actionClassName: Name of the Action class to create an instance from.
		* @param actionInstanceName: Name of the new Action instance.
		* @param context: Container of the new Action instance.
		* @return Newly created Action instance.
		* @see Action
		*/
		Action& CreateAction(const std::string& actionClassName, const std::string& actionInstanceName, Attributed& context) const;

		/** Gets the Class name used to create actions.
		* @return Class name.
		* @see Sector
		*/
		std::string GetClassName() const;

		/** Sets the class name to use while creating new Actions.
		* @param sector: New Class name to use.
		* @see Sector
		*/
		void SetClassName(const std::string& class_name);

		/** Gets the Instance name assigned to the created action.
		* @return Instance name.
		* @see Action
		*/
		std::string GetInstanceName() const;

		/** Sets the Instance name assigned to the created action.
		* @param sector: New Instance name to use.
		* @see Action
		*/
		void SetInstanceName(const std::string& instance_name);

		/** ActionCreateAction's update method. 
		* Launches the creation of an action.
		* @param worldState: Wolrd state to use.
		* @see WorldState
		* @see Action
		*/
		virtual void Update(WorldState& worldState) override;

	protected:

		void InitPrescribedAttributes() override;

	private:

		const static std::string CLASS_NAME_ENTRY_NAME;
		const static std::string INSTANCE_NAME_ENTRY_NAME;
		std::string mClassName;
		std::string mInstanceName;
	};

	ActionConcreteFactory(ActionCreateAction);
}
