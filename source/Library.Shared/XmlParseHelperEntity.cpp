#include "pch.h"
#include "XmlParseHelperEntity.h"
#include "WorldSharedData.h"
#include "Sector.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperEntity);

	const string XmlParseHelperEntity::ENTITY_ELEMENT_NAME = "Entity";
	const string XmlParseHelperEntity::CLASS_NAME_ATTRIBUTE_NAME = "Class";
	const string XmlParseHelperEntity::INSTANCE_NAME_ATTRIBUTE_NAME = "InstanceName";

	/************************************************************************/
	void XmlParseHelperEntity::Initialize()
	{
		IXmlParseHelper::Initialize();
		mIsParsing = false;
	}

	/************************************************************************/
	bool XmlParseHelperEntity::StartElementHandler(SharedDataC& sharedData, const std::string& el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && el == ENTITY_ELEMENT_NAME)
		{
			WorldSharedData& worldSharedData = static_cast<WorldSharedData&>(sharedData);

			if (worldSharedData.GetCurrentState() != WorldSharedData::State::SectorParsing)
			{
				throw exception("Entity should be a nested element to Sector.");
			}

			worldSharedData.SetCurrentState(WorldSharedData::State::EntityParsing);
			mIsParsing = true;

			string entityInstanceName;
			string entityClassName;
			bool instanceNamefound = false;
			bool classNamefound = false;
			for (auto& attr : attributes)
			{
				if (attr.first == CLASS_NAME_ATTRIBUTE_NAME)
				{
					classNamefound = true;
					entityClassName = attr.second;
					continue;;
				}

				if (attr.first == INSTANCE_NAME_ATTRIBUTE_NAME)
				{
					instanceNamefound = true;
					entityInstanceName = attr.second;;
				}
			}

			if (!classNamefound)
			{
				throw exception("The entity needs to have a class name.");
			}

			if (!instanceNamefound)
			{
				throw exception("The entity instance needs to have a name.");
			}

			EntityFactory factory;

			Entity& entity = worldSharedData.GetScope()->As<Sector>()->CreateEntity(entityClassName, entityInstanceName);
			worldSharedData.SetScope(entity);
			++mStartElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperEntity::EndElementHandler(SharedDataC& sharedData, const std::string& el)
	{
		if (sharedData.Is(WorldSharedData::TypeIdClass()) && el == ENTITY_ELEMENT_NAME)
		{
			if (!mIsParsing)
			{
				throw exception("Cannot call EndElementHandler before StartElementHandler.");
			}

			WorldSharedData& worldSharedData = static_cast<WorldSharedData&>(sharedData);

			Scope* entity = worldSharedData.GetScope();
			worldSharedData.SetScope(*entity->As<Entity>()->GetSector());
			worldSharedData.SetCurrentState(WorldSharedData::State::SectorParsing);
			++mEndElementHandlerCount;
			mIsParsing = false;

			return true;
		}

		return false;
	}

	/************************************************************************/
	IXmlParseHelper* XmlParseHelperEntity::Clone()
	{
		XmlParseHelperEntity* helper = new XmlParseHelperEntity();
		helper->mStartElementHandlerCount = mStartElementHandlerCount;
		helper->mEndElementHandlerCount = mEndElementHandlerCount;
		helper->mCharDataHandlerCount = mCharDataHandlerCount;

		return helper;
	}
}
