#include "pch.h"
#include "XmlParseHelperAction.h"
#include "WorldSharedData.h"
#include "Sector.h"
#include "World.h"
#include "Entity.h"
#include "ActionList.h"
#include "Reaction.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperAction);

	const string XmlParseHelperAction::ACTION_ELEMENT_NAME = "Action";
	const string XmlParseHelperAction::REACTION_ELEMENT_NAME = "Reaction";
	const string XmlParseHelperAction::ACTION_EVENT_ELEMENT_NAME= "ActionEvent";
	const string XmlParseHelperAction::CLASS_NAME_ATTRIBUTE_NAME = "Class";
	const string XmlParseHelperAction::INSTANCE_NAME_ATTRIBUTE_NAME = "InstanceName";
	const string XmlParseHelperAction::SUB_TYPE_ATTRIBUTE_NAME = "Subtype";
	const string XmlParseHelperAction::DELAY_ATTRIBUTE_NAME = "Delay";

	/************************************************************************/
	XmlParseHelperAction::XmlParseHelperAction()
	{
		InitializeFunctionsMaps();
	}

	/************************************************************************/
	void XmlParseHelperAction::Initialize()
	{
		IXmlParseHelper::Initialize();
	}

	/************************************************************************/
	bool XmlParseHelperAction::StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && (el == ACTION_ELEMENT_NAME || el == REACTION_ELEMENT_NAME || el == ACTION_EVENT_ELEMENT_NAME))
		{
			WorldSharedData& worldSharedData = static_cast<WorldSharedData&>(sharedData);

			if (worldSharedData.GetCurrentState() == WorldSharedData::State::NotParsing)
			{
				throw exception("Action should be a nested element.");
			}

			auto func = mStartFunctionsMap.Find(el)->second;
			func(worldSharedData, attributes);

			++mStartElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperAction::EndElementHandler(SharedDataC& sharedData, const std::string& el)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && (el == ACTION_ELEMENT_NAME || el == REACTION_ELEMENT_NAME || el == ACTION_EVENT_ELEMENT_NAME))
		{
			WorldSharedData& worldSharedData = static_cast<WorldSharedData&>(sharedData);

			Scope* action = worldSharedData.GetScope();
			worldSharedData.SetScope(*action->As<Action>()->GetPredecessor());
			worldSharedData.SetCurrentState(worldSharedData.GetPreviousStates().Back());
			worldSharedData.GetPreviousStates().PopBack();

			++mEndElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	IXmlParseHelper* XmlParseHelperAction::Clone()
	{
		XmlParseHelperAction* helper = new XmlParseHelperAction();

		helper->mStartElementHandlerCount = mStartElementHandlerCount;
		helper->mEndElementHandlerCount = mEndElementHandlerCount;
		helper->mCharDataHandlerCount = mCharDataHandlerCount;

		return helper;
	}

	/************************************************************************/
	void XmlParseHelperAction::InitializeFunctionsMaps()
	{
		mStartFunctionsMap.Insert(StartFuncPair(ACTION_ELEMENT_NAME, HandleActionElementStart));
		mStartFunctionsMap.Insert(StartFuncPair(REACTION_ELEMENT_NAME, HandleReactionElementStart));
		mStartFunctionsMap.Insert(StartFuncPair(ACTION_EVENT_ELEMENT_NAME, HandleActionEventElementStart));
	}

	/************************************************************************/
	void XmlParseHelperAction::HandleActionElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes)
	{
		string actionInstanceName;
		string actionClassName;
		bool instanceNamefound = false;
		bool classNamefound = false;
		for (auto& attr : attributes)
		{
			if (attr.first == CLASS_NAME_ATTRIBUTE_NAME)
			{
				classNamefound = true;
				actionClassName = attr.second;
				continue;;
			}

			if (attr.first == INSTANCE_NAME_ATTRIBUTE_NAME)
			{
				instanceNamefound = true;
				actionInstanceName = attr.second;;
			}
		}

		if (!classNamefound)
		{
			throw exception("The Action needs to have a class name.");
		}

		if (!instanceNamefound)
		{
			throw exception("The Action instance needs to have a name.");
		}

		Action* action;

		switch (sharedData.GetCurrentState())
		{
			case WorldSharedData::State::WorldParsing:
				action = &sharedData.GetScope()->As<World>()->CreateAction(actionClassName, actionInstanceName);
				break;

			case WorldSharedData::State::SectorParsing:
				action = &sharedData.GetScope()->As<Sector>()->CreateAction(actionClassName, actionInstanceName);
				break;

			case WorldSharedData::State::EntityParsing:
				action = &sharedData.GetScope()->As<Entity>()->CreateAction(actionClassName, actionInstanceName);
				break;

			case WorldSharedData::State::ActionParsing:
				action = &sharedData.GetScope()->As<ActionList>()->CreateAction(actionClassName, actionInstanceName);
				break;

			default:
				action = nullptr;
		}

		assert(action);
		sharedData.SetScope(*action);

		sharedData.GetPreviousStates().PushBack(sharedData.GetCurrentState());
		sharedData.SetCurrentState(WorldSharedData::State::ActionParsing);
	}

	/************************************************************************/
	void XmlParseHelperAction::HandleReactionElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes)
	{
		string actionInstanceName;
		string actionClassName;
		string subType;
		bool instanceNamefound = false;
		bool classNamefound = false;

		for (auto& attr : attributes)
		{
			if (attr.first == CLASS_NAME_ATTRIBUTE_NAME)
			{
				classNamefound = true;
				actionClassName = attr.second;
				continue;
			}

			if (attr.first == INSTANCE_NAME_ATTRIBUTE_NAME)
			{
				instanceNamefound = true;
				actionInstanceName = attr.second;
				continue;
			}

			if (attr.first == SUB_TYPE_ATTRIBUTE_NAME)
			{
				subType = attr.second;;
			}
		}

		if (!classNamefound)
		{
			throw exception("The Reaction needs to have a class name.");
		}

		if (!instanceNamefound)
		{
			throw exception("The Reaction instance needs to have a name.");
		}

		Action* action = Factory<Reaction>::Create(actionClassName);
		if (!action)
		{
			throw exception("The correspondent factory needs to be initialized.");
		}

		action->SetName(actionInstanceName);
		action->As<ReactionAttributed>()->SetSubType(subType);
		sharedData.GetScope()->Adopt(*action, actionInstanceName);

		sharedData.SetScope(*action);

		sharedData.GetPreviousStates().PushBack(sharedData.GetCurrentState());
		sharedData.SetCurrentState(WorldSharedData::State::ActionParsing);
	}

	/************************************************************************/
	void XmlParseHelperAction::HandleActionEventElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes)
	{
		string actionInstanceName;
		string subType;
		string delay = "0";

		bool instanceNamefound = false;

		for (auto& attr : attributes)
		{
			if (attr.first == INSTANCE_NAME_ATTRIBUTE_NAME)
			{
				instanceNamefound = true;
				actionInstanceName = attr.second;
				continue;
			}

			if (attr.first == SUB_TYPE_ATTRIBUTE_NAME)
			{
				subType = attr.second;
				continue;
			}

			if (attr.first == DELAY_ATTRIBUTE_NAME)
			{
				delay = attr.second;
			}
		}

		if (!instanceNamefound)
		{
			throw exception("The ActionEvent instance needs to have a name.");
		}

		Action* action;

		switch (sharedData.GetCurrentState())
		{
			case WorldSharedData::State::WorldParsing:
				action = &sharedData.GetScope()->As<World>()->CreateAction(ACTION_EVENT_ELEMENT_NAME, actionInstanceName);
				break;

			case WorldSharedData::State::SectorParsing:
				action = &sharedData.GetScope()->As<Sector>()->CreateAction(ACTION_EVENT_ELEMENT_NAME, actionInstanceName);
				break;

			case WorldSharedData::State::EntityParsing:
				action = &sharedData.GetScope()->As<Entity>()->CreateAction(ACTION_EVENT_ELEMENT_NAME, actionInstanceName);
				break;

			case WorldSharedData::State::ActionParsing:
				action = &sharedData.GetScope()->As<ActionList>()->CreateAction(ACTION_EVENT_ELEMENT_NAME, actionInstanceName);
				break;

			default:
				action = nullptr;
		}

		assert(action);
		assert(action->Is(ActionEvent::TypeIdClass()));
		action->As<ActionEvent>()->SetSubType(subType);
		action->As<ActionEvent>()->SetDelay(stoi(delay));
		sharedData.SetScope(*action);

		sharedData.GetPreviousStates().PushBack(sharedData.GetCurrentState());
		sharedData.SetCurrentState(WorldSharedData::State::ActionParsing);
	}
}
