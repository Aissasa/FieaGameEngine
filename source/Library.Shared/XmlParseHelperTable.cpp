#include "pch.h"
#include "XmlParseMaster.h"
#include "XmlParseHelperTable.h"
#include "TableSharedData.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperTable)

	const std::string XmlParseHelperTable::SCOPE_ELEMENT_NAME = "Scope";
	const std::string XmlParseHelperTable::NAME_ATTRIBUTE_NAME = "Name";

	/************************************************************************/
	void XmlParseHelperTable::Initialize()
	{
		IXmlParseHelper::Initialize();
	}

	/************************************************************************/
	bool XmlParseHelperTable::StartElementHandler(XmlParseMaster::SharedData& sharedData, const std::string & el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && el == SCOPE_ELEMENT_NAME)
		{
			string attrToAdd;
			bool found = false;
			for (auto& attr : attributes)
			{
				if (attr.first == NAME_ATTRIBUTE_NAME)
				{
					found = true;
					attrToAdd = attr.second;
					break;
				}
			}

			if (!found)
			{
				throw exception("The scope needs to have a name.");
			}

			if (sharedData.Depth() <= 1)
			{
				Scope* mainScope = new Scope();
				sharedData.As<TableSharedData>()->SetScope(*mainScope);
			}
			else
			{
				// add nested scope
				Scope& nestedScope = sharedData.As<TableSharedData>()->GetScope().AppendScope(attrToAdd);
				sharedData.As<TableSharedData>()->SetScope(nestedScope);
			}

			Datum& dat = sharedData.As<TableSharedData>()->GetScope().Append(NAME_ATTRIBUTE_NAME);
			dat.PushBack(attrToAdd);

			++mStartElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperTable::EndElementHandler(XmlParseMaster::SharedData& sharedData, const std::string & el)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && el == SCOPE_ELEMENT_NAME)
		{
			++mEndElementHandlerCount;

			// end of nested scope
			if (sharedData.Depth() > 1)
			{
				// go to parent scope
				Scope* parentScope = sharedData.As<TableSharedData>()->GetScope().GetParent();
				sharedData.As<TableSharedData>()->SetScope(*parentScope);
			}
			return true;
		}
		return false;
	}

	/************************************************************************/
	IXmlParseHelper * XmlParseHelperTable::Clone()
	{
		XmlParseHelperTable* helper = new XmlParseHelperTable();
		helper->mStartElementHandlerCount = mStartElementHandlerCount;
		helper->mEndElementHandlerCount = mEndElementHandlerCount;
		helper->mCharDataHandlerCount = mCharDataHandlerCount;

		return helper;
	}
}