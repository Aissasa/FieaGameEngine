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

			// add nested scope
			if (sharedData.Depth() != 0)
			{
				Scope& nestedScope = sharedData.As<TableSharedData>()->GetCurrentScope()->AppendScope(attrToAdd);
				sharedData.As<TableSharedData>()->SetCurrentScope(&nestedScope);
			}

			Datum& dat = sharedData.As<TableSharedData>()->GetCurrentScope()->Append(NAME_ATTRIBUTE_NAME);
			dat.PushBack(attrToAdd);

			++mStartElementHandlerCount;
			sharedData.IncrementDepth();

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
			sharedData.DecrementDepth();

			// end of nested scope
			if (sharedData.Depth() != 0)
			{
				// go to parent scope
				sharedData.As<TableSharedData>()->SetCurrentScope(sharedData.As<TableSharedData>()->GetCurrentScope()->GetParent());
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