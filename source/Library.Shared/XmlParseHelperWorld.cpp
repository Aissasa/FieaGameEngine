#include "pch.h"
#include "XmlParseHelperWorld.h"
#include "WorldSharedData.h"
#include "World.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperWorld);

	const string XmlParseHelperWorld::WORLD_ELEMENT_NAME = "World";
	const string XmlParseHelperWorld::NAME_ATTRIBUTE_NAME = "Name";

	/************************************************************************/
	void XmlParseHelperWorld::Initialize()
	{
		IXmlParseHelper::Initialize();
		mIsParsing = false;
	}

	/************************************************************************/
	bool XmlParseHelperWorld::StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && el == WORLD_ELEMENT_NAME)
		{
			if (sharedData.Depth() > 0)
			{
				throw exception("World should be the root element and not a nested element.");
			}

			WorldSharedData& worldSharedData = static_cast<WorldSharedData&>(sharedData);
			worldSharedData.Initialize();
			worldSharedData.SetCurrentState(WorldSharedData::State::WorldParsing);
			mIsParsing = true;

			string worldName;
			bool found = false;
			for (auto& attr : attributes)
			{
				if (attr.first == NAME_ATTRIBUTE_NAME)
				{
					found = true;
					worldName = attr.second;
					break;
				}
			}

			if (!found)
			{
				throw exception("The world needs to have a name.");
			}

			World* world = new World();
			world->SetName(worldName);
			worldSharedData.SetScope(*world);

			++mStartElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperWorld::EndElementHandler(SharedDataC& sharedData, const std::string& el)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && el == WORLD_ELEMENT_NAME)
		{
			if (!mIsParsing)
			{
				throw exception("Cannot call EndElementHandler before StartElementHandler.");
			}

			sharedData.As<WorldSharedData>()->SetCurrentState(WorldSharedData::State::NotParsing);
			++mEndElementHandlerCount;
			mIsParsing = false;

			return true;
		}

		return false;
	}

	/************************************************************************/
	IXmlParseHelper* XmlParseHelperWorld::Clone()
	{
		XmlParseHelperWorld* helper = new XmlParseHelperWorld();
		helper->mStartElementHandlerCount = mStartElementHandlerCount;
		helper->mEndElementHandlerCount = mEndElementHandlerCount;
		helper->mCharDataHandlerCount = mCharDataHandlerCount;

		return helper;
	}
}
