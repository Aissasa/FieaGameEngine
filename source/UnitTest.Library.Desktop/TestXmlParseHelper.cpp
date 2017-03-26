#include "pch.h"
#include "TestXmlParseHelper.h"
#include "TestSharedData.h"

using namespace Library;
using namespace std;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(TestXmlParseHelper)

	/************************************************************************/
	void TestXmlParseHelper::Initialize()
	{
		IXmlParseHelper::Initialize();
	}

	/************************************************************************/
	bool TestXmlParseHelper::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el, const HashMap<std::string, std::string>& attributes)
	{
		// we can add constrains to el like == damage
		if (sharedData.Is(TestSharedData::TypeIdClass()) && el != "Untreated")
		{
			++mStartElementHandlerCount;

			for (auto& attr : attributes)
			{
				if (attr.first == "Damage" || attr.first == "Value")
				{
					sharedData.As<TestSharedData>()->mDamage = stoi(attr.second);
					break;
				}
			}

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool TestXmlParseHelper::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el)
	{
		// we can add constrains to el like == damage
		if (sharedData.Is(TestSharedData::TypeIdClass()) && el != "Untreated")
		{
			++mEndElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool TestXmlParseHelper::CharDataHandler(SharedDataC& sharedData, const std::string & str)
	{
		++mCharDataHandlerCount;
		sharedData.As<TestSharedData>()->mDamage = stoi(str);

		return true;
	}

	/************************************************************************/
	IXmlParseHelper* TestXmlParseHelper::Clone()
	{
		TestXmlParseHelper* helper = new TestXmlParseHelper();
		helper->mStartElementHandlerCount = mStartElementHandlerCount;
		helper->mEndElementHandlerCount = mEndElementHandlerCount;
		helper->mCharDataHandlerCount = mCharDataHandlerCount;

		return helper;
	}

}