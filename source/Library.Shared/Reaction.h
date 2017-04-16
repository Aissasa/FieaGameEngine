#pragma once

#include "ActionList.h"
#include "EventSubscriber.h"

namespace Library
{
	/** Reaction is the tie between the event system and the scripting unit of the game engine.
	* It derives from ActionList and EventSubscriber.
	* @see RTTI
	* @see ActionList
	* @see EventSubscriber
	*/
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList);

	public:

		/** Reaction Constructor.
		*/
		Reaction() = default;

		/** Reaction Destructor.
		*/
		virtual ~Reaction() = default;

		Reaction(const Reaction & rhs) = delete;
		Reaction& operator=(const Reaction& rhs) = delete;

	};

#define ReactionConcreteFactory(ReactionDerivedClass) \
	ConcreteFactory(Library::Reaction, ReactionDerivedClass);

}
