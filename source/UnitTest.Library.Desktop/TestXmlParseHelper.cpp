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
		if (!mIsInitialized)
		{
			mIsInitialized = true;
			// do something
		}
	}

	/************************************************************************/
	bool TestXmlParseHelper::StartElementHandler(Library::XmlParseMaster::SharedData & sharedData, const std::string & el, const Library::HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(TestSharedData::TypeIdClass()) && el == "Weapon")
		{
			++mStartElementHandlerCount;
			sharedData.IncrementDepth();
			sharedData.As<TestSharedData>()->UpdateMaxDepth();

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
	bool TestXmlParseHelper::EndElementHandler(Library::XmlParseMaster::SharedData & sharedData, const std::string & el)
	{
		if (sharedData.Is(TestSharedData::TypeIdClass()))
		{
			++mEndElementHandlerCount;

			sharedData.DecrementDepth();
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
		helper->mIsInitialized = mIsInitialized;
		helper->mStartElementHandlerCount = mStartElementHandlerCount;
		helper->mEndElementHandlerCount = mEndElementHandlerCount;
		helper->mCharDataHandlerCount = mCharDataHandlerCount;

		return helper;
	}

}