#include "pch.h"
#include "XmlParseHelperAction.h"
#include "WorldSharedData.h"
#include "Sector.h"
#include "World.h"
#include "Entity.h"
#include "ActionList.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperAction);

	const string XmlParseHelperAction::ACTION_ELEMENT_NAME = "Action";
	const string XmlParseHelperAction::CLASS_NAME_ATTRIBUTE_NAME = "Class";
	const string XmlParseHelperAction::INSTANCE_NAME_ATTRIBUTE_NAME = "InstanceName";

	/************************************************************************/
	XmlParseHelperAction::XmlParseHelperAction(): 
		mPreviousStates()
	{
	}

	/************************************************************************/
	void XmlParseHelperAction::Initialize()
	{
		IXmlParseHelper::Initialize();
	}

	/************************************************************************/
	bool XmlParseHelperAction::StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && el == ACTION_ELEMENT_NAME)
		{
			WorldSharedData& worldSharedData = static_cast<WorldSharedData&>(sharedData);

			if (worldSharedData.GetCurrentState() == WorldSharedData::State::NotParsing)
			{
				throw exception("Action should be a nested element.");
			}

			mPreviousStates.PushBack(worldSharedData.GetCurrentState());
			worldSharedData.SetCurrentState(WorldSharedData::State::ActionParsing);

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
				throw exception("The entity instance needs to have a name.");
			}

			Action* action;

			switch (mPreviousStates.Back())
			{
				case WorldSharedData::State::WorldParsing:
					action = &worldSharedData.GetScope()->As<World>()->CreateAction(actionClassName, actionInstanceName);
					break;

				case WorldSharedData::State::SectorParsing:
					action = &worldSharedData.GetScope()->As<Sector>()->CreateAction(actionClassName, actionInstanceName);
					break;

				case WorldSharedData::State::EntityParsing: 
					action = &worldSharedData.GetScope()->As<Entity>()->CreateAction(actionClassName, actionInstanceName);
					break;

				case WorldSharedData::State::ActionParsing: 
					action = &worldSharedData.GetScope()->As<ActionList>()->CreateAction(actionClassName, actionInstanceName);
					break;

				default:
					action = nullptr;
			}

			assert(action);
			worldSharedData.SetScope(*action);
			++mStartElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperAction::EndElementHandler(SharedDataC& sharedData, const std::string& el)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && el == ACTION_ELEMENT_NAME)
		{
			WorldSharedData& worldSharedData = static_cast<WorldSharedData&>(sharedData);

			Scope* action = worldSharedData.GetScope();
			worldSharedData.SetScope(*action->As<Action>()->GetPredecessor());
			worldSharedData.SetCurrentState(mPreviousStates.Back());
			mPreviousStates.PopBack();

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
}
