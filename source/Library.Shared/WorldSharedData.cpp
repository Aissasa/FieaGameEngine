#include "pch.h"
#include "WorldSharedData.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(WorldSharedData);

	/************************************************************************/
	WorldSharedData::WorldSharedData(XmlParseMaster* xmlParseMaster):
		TableSharedData(xmlParseMaster), mCurrentState(State::NotParsing), mPreviousStates()
	{
	}

	/************************************************************************/
	XmlParseMaster::SharedData* WorldSharedData::Clone()
	{
		WorldSharedData* data = new WorldSharedData(mXmlParseMaster);
		data->mDepth = mDepth;
		data->IsParsingElement = IsParsingElement;
		data->IsParsingMatrix = IsParsingMatrix;
		data->MatrixVectors = MatrixVectors;
		data->mCurrentState = mCurrentState;
		data->mPreviousStates = mPreviousStates;

		return data;
	}

	/************************************************************************/
	void WorldSharedData::Initialize()
	{
		TableSharedData::Initialize();
		mCurrentState = State::NotParsing;
		mPreviousStates.Empty();
	}

	/************************************************************************/
	WorldSharedData::State WorldSharedData::GetCurrentState() const
	{
		return mCurrentState;
	}

	/************************************************************************/
	void WorldSharedData::SetCurrentState(const State current_state)
	{
		mCurrentState = current_state;
	}

	Vector<WorldSharedData::State>& WorldSharedData::GetPreviousStates()
	{
		return mPreviousStates;
	}
}
