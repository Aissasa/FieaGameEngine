#include "pch.h"
#include "XmlParseHelperActionIf.h"
#include "WorldSharedData.h"
#include "Sector.h"
#include "World.h"
#include "Entity.h"
#include "ActionList.h"
#include "ActionListIf.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperActionIf);

	const string XmlParseHelperActionIf::ACTION_LIST_IF_CLASS_NAME = "ActionListIf";
	const string XmlParseHelperActionIf::IF_ELEMENT_NAME = "If";
	const string XmlParseHelperActionIf::INSTANCE_NAME_ATTRIBUTE_NAME = "InstanceName";
	const string XmlParseHelperActionIf::CLASS_NAME_ATTRIBUTE_NAME = "ClassName";
	const string XmlParseHelperActionIf::CONDITION_ELEMENT_NAME = "Condition";
	const string XmlParseHelperActionIf::CONDITION_ELEMENT_VALUE_ATTRIBUTE_NAME = "Name";
	const string XmlParseHelperActionIf::THEN_ELEMENT_NAME = "Then";
	const string XmlParseHelperActionIf::ELSE_ELEMENT_NAME = "Else";


	/************************************************************************/
	XmlParseHelperActionIf::XmlParseHelperActionIf():
		mStartFunctionsMap(), mEndFunctionsMap()
	{
		InitializeFunctionsMaps();
	}

	/************************************************************************/
	void XmlParseHelperActionIf::Initialize()
	{
		IXmlParseHelper::Initialize();
	}

	/************************************************************************/
	bool XmlParseHelperActionIf::StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && (el == IF_ELEMENT_NAME || el == CONDITION_ELEMENT_NAME || el == THEN_ELEMENT_NAME || el == ELSE_ELEMENT_NAME))
		{
			WorldSharedData& worldSharedData = static_cast<WorldSharedData&>(sharedData);

			if (worldSharedData.GetCurrentState() == WorldSharedData::State::NotParsing)
			{
				throw exception("This should be a nested element.");
			}

			auto func = mStartFunctionsMap.Find(el)->second;
			func(worldSharedData, attributes);

			++mStartElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperActionIf::EndElementHandler(SharedDataC& sharedData, const std::string& el)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && (el == IF_ELEMENT_NAME || el == CONDITION_ELEMENT_NAME || el == THEN_ELEMENT_NAME || el == ELSE_ELEMENT_NAME))
		{
			WorldSharedData& worldSharedData = static_cast<WorldSharedData&>(sharedData);
			auto it = mEndFunctionsMap.Find(el);
			if (it != mEndFunctionsMap.end())
			{
				auto func = it->second;
				func(worldSharedData);
			}

			++mEndElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	IXmlParseHelper* XmlParseHelperActionIf::Clone()
	{
		XmlParseHelperActionIf* helper = new XmlParseHelperActionIf();
		helper->mStartElementHandlerCount = mStartElementHandlerCount;
		helper->mEndElementHandlerCount = mEndElementHandlerCount;
		helper->mCharDataHandlerCount = mCharDataHandlerCount;

		return helper;
	}

	/************************************************************************/
	void XmlParseHelperActionIf::InitializeFunctionsMaps()
	{
		mStartFunctionsMap.Insert(StartFuncPair(IF_ELEMENT_NAME, HandleIfElementStart));
		mStartFunctionsMap.Insert(StartFuncPair(CONDITION_ELEMENT_NAME, HandleConditionElementStart));
		mStartFunctionsMap.Insert(StartFuncPair(THEN_ELEMENT_NAME, HandleThenElementStart));
		mStartFunctionsMap.Insert(StartFuncPair(ELSE_ELEMENT_NAME, HandleElseElementStart));
		mEndFunctionsMap.Insert(EndFuncPair(IF_ELEMENT_NAME, HandleIfElementEnd));
		mEndFunctionsMap.Insert(EndFuncPair(THEN_ELEMENT_NAME, HandleThenElementEnd));
		mEndFunctionsMap.Insert(EndFuncPair(ELSE_ELEMENT_NAME, HandleElseElementEnd));
	}

	/************************************************************************/
	void XmlParseHelperActionIf::HandleIfElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes)
	{
		string actionInstanceName;
		bool instanceNamefound = false;
		for (auto& attr : attributes)
		{
			if (attr.first == INSTANCE_NAME_ATTRIBUTE_NAME)
			{
				instanceNamefound = true;
				actionInstanceName = attr.second;
				break;
			}
		}

		if (!instanceNamefound)
		{
			throw exception("The ActionListIf instance needs to have a name.");
		}

		Action* action;

		switch (sharedData.GetCurrentState())
		{
			case WorldSharedData::State::WorldParsing:
				action = &sharedData.GetScope()->As<World>()->CreateAction(ACTION_LIST_IF_CLASS_NAME, actionInstanceName);
				break;

			case WorldSharedData::State::SectorParsing:
				action = &sharedData.GetScope()->As<Sector>()->CreateAction(ACTION_LIST_IF_CLASS_NAME, actionInstanceName);
				break;

			case WorldSharedData::State::EntityParsing:
				action = &sharedData.GetScope()->As<Entity>()->CreateAction(ACTION_LIST_IF_CLASS_NAME, actionInstanceName);
				break;

			case WorldSharedData::State::ActionParsing:
				action = &sharedData.GetScope()->As<ActionList>()->CreateAction(ACTION_LIST_IF_CLASS_NAME, actionInstanceName);
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
	void XmlParseHelperActionIf::HandleConditionElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes)
	{
		string conditionName;
		bool conditionNamefound = false;
		for (auto& attr : attributes)
		{
			if (attr.first == CONDITION_ELEMENT_VALUE_ATTRIBUTE_NAME)
			{
				conditionNamefound = true;
				conditionName = attr.second;;
			}
		}

		if (!conditionNamefound)
		{
			throw exception("The Condition integer needs a name.");
		}

		sharedData.GetScope()->As<ActionListIf>()->SetConditionIntegerName(conditionName);
	}

	/************************************************************************/
	void XmlParseHelperActionIf::HandleThenElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes)
	{
		string className;
		bool classNamefound = false;
		for (auto& attr : attributes)
		{
			if (attr.first == CLASS_NAME_ATTRIBUTE_NAME)
			{
				classNamefound = true;
				className = attr.second;;
			}
		}

		if (!classNamefound)
		{
			throw exception("The then action instance needs a name.");
		}

		Action& action = sharedData.GetScope()->As<ActionListIf>()->CreateThenAction(className);
		sharedData.SetScope(action);
		sharedData.GetPreviousStates().PushBack(sharedData.GetCurrentState());
		sharedData.SetCurrentState(WorldSharedData::State::ActionParsing);
	}

	/************************************************************************/
	void XmlParseHelperActionIf::HandleElseElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes)
	{
		string className;
		bool classNamefound = false;
		for (auto& attr : attributes)
		{
			if (attr.first == CLASS_NAME_ATTRIBUTE_NAME)
			{
				classNamefound = true;
				className = attr.second;;
			}
		}

		if (!classNamefound)
		{
			throw exception("The else action instance needs a name.");
		}

		Action& action = sharedData.GetScope()->As<ActionListIf>()->CreateElseAction(className);
		sharedData.SetScope(action);
		sharedData.GetPreviousStates().PushBack(sharedData.GetCurrentState());
		sharedData.SetCurrentState(WorldSharedData::State::ActionParsing);
	}

	/************************************************************************/
	void XmlParseHelperActionIf::HandleIfElementEnd(WorldSharedData& sharedData)
	{
		Scope* action = sharedData.GetScope();
		assert(action);
		sharedData.SetScope(*action->As<Action>()->GetPredecessor());
		sharedData.SetCurrentState(sharedData.GetPreviousStates().Back());
		sharedData.GetPreviousStates().PopBack();
	}

	/************************************************************************/
	void XmlParseHelperActionIf::HandleThenElementEnd(WorldSharedData& sharedData)
	{
		HandleIfElementEnd(sharedData);
	}

	/************************************************************************/
	void XmlParseHelperActionIf::HandleElseElementEnd(WorldSharedData& sharedData)
	{
		HandleIfElementEnd(sharedData);
	}
}
