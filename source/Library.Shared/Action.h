#pragma once
#include "Attributed.h"

namespace Library
{
	class WorldState;

	/** Action represnts a unit that contains a certain behavior.
	* It inherits from Attributed.
	* @see Attributed
	*/
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)

	public:

		/** Action Constructor.
		*/
		Action();

		/** Action Destructor.
		*/
		virtual ~Action() = default;

		Action(const Action & rhs) = delete;
		Action& operator=(const Action& rhs) = delete;

		/** Gets the name of the Action.
		* @return Name of the Action.
		*/
		std::string Name() const;

		/** Sets the name of the Action.
		* @param name: New name of the Action to set.
		*/
		void SetName(const std::string& name);

		/** Action's update method.
		* @param worldState: Wolrd state to use.
		* @see WorldState
		*/
		virtual void Update(WorldState& worldState) = 0;

		/** Gets the predecessor of the Action.
		* @return Predecessor of the Action.
		*/
		virtual Attributed* GetPredecessor() const;

		/** Sets the predecessor of the Action.
		* @param predecessor: New predecessor of the Action to set.
		*/
		virtual void SetPredecessor(Attributed& predecessor);
		 
	protected:
		void InitPrescribedAttributes() override;
		const static std::string ACTIONS_ENTRY_NAME;

	private:

		const static std::string ACTION_NAME_ENTRY_NAME;
		std::string mName;

	};

#define ActionConcreteFactory(ActionDerivedClass) \
	ConcreteFactory(Library::Action, ActionDerivedClass);

}
