#include "pch.h"
#include "TestSharedData.h"

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(TestSharedData)

	/************************************************************************/
	TestSharedData::TestSharedData(Library::XmlParseMaster* xmlParseMaster):
		mDamage(0), mMaxDepth(0), SharedDataC::SharedData(xmlParseMaster)
	{
	}

	/************************************************************************/
	Library::XmlParseMaster::SharedData* TestSharedData::Clone()
	{
		TestSharedData* data = new TestSharedData(mXmlParseMaster);
		data->mDepth = mDepth;
		data->mMaxDepth = mMaxDepth;
		data->mDamage = mDamage;

		return data;
	}

	/************************************************************************/
	void TestSharedData::Initialize()
	{
		SharedDataC::Initialize();
		mMaxDepth = 0;
		mDamage = 0;
	}

	/************************************************************************/
	void TestSharedData::UpdateMaxDepth()
	{
		if (mDepth > mMaxDepth)
		{
			mMaxDepth = mDepth;
		}
	}

	/************************************************************************/
	std::uint32_t TestSharedData::GetMaxDepth() const
	{
		return mMaxDepth;
	}

}