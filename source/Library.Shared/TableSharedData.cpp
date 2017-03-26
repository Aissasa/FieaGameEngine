#include "pch.h"
#include "TableSharedData.h"
#include "Vector.h"

namespace Library
{
	RTTI_DEFINITIONS(TableSharedData)

	/************************************************************************/
	TableSharedData::TableSharedData(XmlParseMaster* xmlParseMaster):
		mScope(nullptr) ,IsParsingElement(false), XmlParseMaster::SharedData(xmlParseMaster)
	{
	}

	/************************************************************************/
	TableSharedData::~TableSharedData()
	{
		delete mScope;
	}

	/************************************************************************/
	XmlParseMaster::SharedData * TableSharedData::Clone()
	{
		TableSharedData* data = new TableSharedData(mXmlParseMaster);
		data->mDepth = mDepth;
		data->IsParsingElement = IsParsingElement;
		data->MatrixVectors = MatrixVectors;

		return data;
	}

	/************************************************************************/
	void TableSharedData::Initialize()
	{
		XmlParseMaster::SharedData::Initialize();
		IsParsingElement = false;
		IsParsingMatrix = false;
		MatrixVectors.Clear();
		if (mScope)
		{
			delete mScope;
			mScope = nullptr;
		}
	}

	/************************************************************************/
	Scope& TableSharedData::GetScope() const
	{
		return *mScope;
	}

	/************************************************************************/
	void TableSharedData::SetScope(Scope& scope)
	{
		mScope = &scope;
	}

}