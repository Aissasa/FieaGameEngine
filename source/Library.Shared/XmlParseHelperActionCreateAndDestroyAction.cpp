#include "pch.h"
#include "XmlParseHelperActionCreateAndDestroyAction.h"
#include "WorldSharedData.h"
#include "Sector.h"
#include "World.h"
#include "Entity.h"
#include "ActionList.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperActionCreateAndDestroyAction);
	
	const string XmlParseHelperActionCreateAndDestroyAction::ACTION_CREATE_ACTION_ELEMENT_NAME = "ActionCreateAction";
	const string XmlParseHelperActionCreateAndDestroyAction::ACTION_DESTROY_ACTION_ELEMENT_NAME = "ActionDestroyAction";
	const string XmlParseHelperActionCreateAndDestroyAction::NAME_ATTRIBUTE_NAME = "Name";
	const string XmlParseHelperActionCreateAndDestroyAction::CLASS_NAME_ATTRIBUTE_NAME = "ClassName";
	const string XmlParseHelperActionCreateAndDestroyAction::INSTANCE_NAME_ATTRIBUTE_NAME = "InstanceName";

	/************************************************************************/
	XmlParseHelperActionCreateAndDestroyAction::XmlParseHelperActionCreateAndDestroyAction():
		mStartFunctionsMap()
	{
		InitializeFunctionsMaps();
	}

	/************************************************************************/
	void XmlParseHelperActionCreateAndDestroyAction::Initialize()
	{
		IXmlParseHelper::Initialize();
	}

	/************************************************************************/
	bool XmlParseHelperActionCreateAndDestroyAction::StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && (el == ACTION_CREATE_ACTION_ELEMENT_NAME || el == ACTION_DESTROY_ACTION_ELEMENT_NAME))
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
	bool XmlParseHelperActionCreateAndDestroyAction::EndElementHandler(SharedDataC& sharedData, const std::string& el)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && (el == ACTION_CREATE_ACTION_ELEMENT_NAME || el == ACTION_DESTROY_ACTION_ELEMENT_NAME))
		{
			++mEndElementHandlerCount;
			return true;
		}

		return false;
	}

	/************************************************************************/
	IXmlParseHelper* XmlParseHelperActionCreateAndDestroyAction::Clone()
	{
		XmlParseHelperActionCreateAndDestroyAction* helper = new XmlParseHelperActionCreateAndDestroyAction();

		helper->mStartElementHandlerCount = mStartElementHandlerCount;
		helper->mEndElementHandlerCount = mEndElementHandlerCount;
		helper->mCharDataHandlerCount = mCharDataHandlerCount;

		return helper;
	}

	/************************************************************************/
	void XmlParseHelperActionCreateAndDestroyAction::InitializeFunctionsMaps()
	{
		mStartFunctionsMap.Insert(StartFuncPair(ACTION_CREATE_ACTION_ELEMENT_NAME, HandleCreateElementStart));
		mStartFunctionsMap.Insert(StartFuncPair(ACTION_DESTROY_ACTION_ELEMENT_NAME, HandleDestroyElementStart));
	}

	/************************************************************************/
	void XmlParseHelperActionCreateAndDestroyAction::HandleCreateElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes)
	{
		string creatorName;
		string actionClassName;
		string actionInstanceName;
		bool creatorNamefound = false;
		bool classNamefound = false;
		bool instanceNamefound = false;

		for (auto& attr : attributes)
		{
			if (attr.first == NAME_ATTRIBUTE_NAME)
			{
				creatorNamefound = true;
				creatorName = attr.second;
				continue;
			}

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
			}
		}

		if (!creatorNamefound)
		{
			throw exception("The ActionCreateAction needs to have a name.");
		}

		if (!classNamefound)
		{
			throw exception("The ActionCreateAction needs to have an action class name.");
		}

		if (!instanceNamefound)
		{
			throw exception("The ActionCreateAction needs to have an action instance name.");
		}

		Action* action;
		switch (sharedData.GetCurrentState())
		{
			case WorldSharedData::State::WorldParsing:
				action = &sharedData.GetScope()->As<World>()->CreateAction(ACTION_CREATE_ACTION_ELEMENT_NAME, creatorName);
				break;

			case WorldSharedData::State::SectorParsing:
				action = &sharedData.GetScope()->As<Sector>()->CreateAction(ACTION_CREATE_ACTION_ELEMENT_NAME, creatorName);
				break;

			case WorldSharedData::State::EntityParsing:
				action = &sharedData.GetScope()->As<Entity>()->CreateAction(ACTION_CREATE_ACTION_ELEMENT_NAME, creatorName);
				break;

			case WorldSharedData::State::ActionParsing:
				action = &sharedData.GetScope()->As<ActionList>()->CreateAction(ACTION_CREATE_ACTION_ELEMENT_NAME, creatorName);
				break;

			default:
				action = nullptr;
		}

		assert(action);
		action->As<ActionCreateAction>()->SetClassName(actionClassName);
		action->As<ActionCreateAction>()->SetInstanceName(actionInstanceName);
	}

	/************************************************************************/
	void XmlParseHelperActionCreateAndDestroyAction::HandleDestroyElementStart(WorldSharedData& sharedData, const HashMap<std::string, std::string>& attributes)
	{
		string destroyerName;
		string actionInstanceName;
		bool creatorNamefound = false;
		bool instanceNamefound = false;

		for (auto& attr : attributes)
		{
			if (attr.first == NAME_ATTRIBUTE_NAME)
			{
				creatorNamefound = true;
				destroyerName = attr.second;
				continue;
			}

			if (attr.first == INSTANCE_NAME_ATTRIBUTE_NAME)
			{
				instanceNamefound = true;
				actionInstanceName = attr.second;
			}
		}

		if (!creatorNamefound)
		{
			throw exception("The ActionDestroyAction needs to have a name.");
		}

		if (!instanceNamefound)
		{
			throw exception("The ActionDestroyAction needs to have an action instance name.");
		}

		Action* action;
		switch (sharedData.GetCurrentState())
		{
			case WorldSharedData::State::WorldParsing:
				action = &sharedData.GetScope()->As<World>()->CreateAction(ACTION_DESTROY_ACTION_ELEMENT_NAME, destroyerName);
				break;

			case WorldSharedData::State::SectorParsing:
				action = &sharedData.GetScope()->As<Sector>()->CreateAction(ACTION_DESTROY_ACTION_ELEMENT_NAME, destroyerName);
				break;

			case WorldSharedData::State::EntityParsing:
				action = &sharedData.GetScope()->As<Entity>()->CreateAction(ACTION_DESTROY_ACTION_ELEMENT_NAME, destroyerName);
				break;

			case WorldSharedData::State::ActionParsing:
				action = &sharedData.GetScope()->As<ActionList>()->CreateAction(ACTION_DESTROY_ACTION_ELEMENT_NAME, destroyerName);
				break;

			default:
				action = nullptr;
		}

		assert(action);
		action->As<ActionDestroyAction>()->SetInstanceName(actionInstanceName);
	}
}
