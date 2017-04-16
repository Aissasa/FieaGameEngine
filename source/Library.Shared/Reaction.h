#pragma once

#include "ActionList.h"
#include "EventSubscriber.h"

namespace Library
{
	class Reaction : public ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList);

	public:

		Reaction() = default;
		virtual ~Reaction() = default;
		Reaction(const Reaction & rhs) = delete;
		Reaction& operator=(const Reaction& rhs) = delete;

	};

#define ReactionConcreteFactory(ReactionDerivedClass) \
	ConcreteFactory(Library::Reaction, ReactionDerivedClass);

}
