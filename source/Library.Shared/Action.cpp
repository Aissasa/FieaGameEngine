#include "pch.h"
#include "Action.h"
#include "Entity.h"
#include "World.h"
#include "Sector.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(Action);

	const string Action::ACTION_NAME_ENTRY_NAME = "Name";
	const string Action::ACTIONS_ENTRY_NAME = "Actions";

	/************************************************************************/
	Action::Action()
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	string Action::Name() const
	{
		return mName;
	}

	/************************************************************************/
	void Action::SetName(const string& name)
	{
		mName = name;
	}

	/************************************************************************/
	Attributed* Action::GetPredecessor() const
	{
		Scope* parent = GetParent();
		if (!parent)
		{
			return nullptr;
		}

		assert(parent->Is(Attributed::TypeIdClass()));
		return static_cast<Attributed*>(parent);
	}

	/************************************************************************/
	void Action::SetPredecessor(Attributed& predecessor)
	{
		if (!predecessor.Is(Action::TypeIdClass()) && !predecessor.Is(Entity::TypeIdClass()) &&
			!predecessor.Is(Sector::TypeIdClass()) && !predecessor.Is(World::TypeIdClass()))
		{
			throw invalid_argument("Wrong predecessor type.");
		}
		assert(predecessor.IsPrescribedAttribute(ACTIONS_ENTRY_NAME));
		predecessor.Adopt(*this, ACTIONS_ENTRY_NAME);
	}

	/************************************************************************/
	void Action::InitPrescribedAttributes()
	{
		AddInternalAttribute("this", static_cast<RTTI*>(this));
		AddExternalAttribute(ACTION_NAME_ENTRY_NAME, &mName, 1);
	}
}
