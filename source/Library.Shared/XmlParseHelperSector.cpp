#include "pch.h"
#include "XmlParseHelperSector.h"
#include "WorldSharedData.h"
#include "Sector.h"
#include "World.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperSector);

	const string XmlParseHelperSector::SECTOR_ELEMENT_NAME = "Sector";
	const string XmlParseHelperSector::NAME_ATTRIBUTE_NAME = "Name";

	/************************************************************************/
	void XmlParseHelperSector::Initialize()
	{
		IXmlParseHelper::Initialize();
		mIsParsing = false;
	}

	/************************************************************************/
	bool XmlParseHelperSector::StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && el == SECTOR_ELEMENT_NAME)
		{
			WorldSharedData& worldSharedData = static_cast<WorldSharedData&>(sharedData);

			if (worldSharedData.GetCurrentState() != WorldSharedData::State::WorldParsing)
			{
				throw exception("Sector should be a nested element to world.");
			}

			worldSharedData.SetCurrentState(WorldSharedData::State::SectorParsing);
			mIsParsing = true;

			string sectorName;
			bool found = false;
			for (auto& attr : attributes)
			{
				if (attr.first == NAME_ATTRIBUTE_NAME)
				{
					found = true;
					sectorName = attr.second;
					break;
				}
			}

			if (!found)
			{
				throw exception("The sector needs to have a name.");
			}

			// urgent refactor in a way that allows less classes, think hashmap and lamda expressions
			Sector& sector = worldSharedData.GetScope()->As<World>()->CreateSector(sectorName);
			worldSharedData.SetScope(sector);
			++mStartElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperSector::EndElementHandler(SharedDataC& sharedData, const std::string& el)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && el == SECTOR_ELEMENT_NAME)
		{
			if (!mIsParsing)
			{
				throw exception("Cannot call EndElementHandler before StartElementHandler.");
			}

			WorldSharedData& worldSharedData = static_cast<WorldSharedData&>(sharedData);

			Scope* sector = worldSharedData.GetScope();
			worldSharedData.SetScope(*sector->As<Sector>()->GetWorld());
			worldSharedData.SetCurrentState(WorldSharedData::State::WorldParsing);
			++mEndElementHandlerCount;
			mIsParsing = false;

			return true;
		}

		return false;
	}

	/************************************************************************/
	IXmlParseHelper* XmlParseHelperSector::Clone()
	{
		XmlParseHelperSector* helper = new XmlParseHelperSector();
		helper->mStartElementHandlerCount = mStartElementHandlerCount;
		helper->mEndElementHandlerCount = mEndElementHandlerCount;
		helper->mCharDataHandlerCount = mCharDataHandlerCount;

		return helper;
	}
}
