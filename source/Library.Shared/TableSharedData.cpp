#include "pch.h"
#include "TableSharedData.h"
#include "Vector.h"

namespace Library
{
	RTTI_DEFINITIONS(TableSharedData)

	/************************************************************************/
	TableSharedData::TableSharedData(XmlParseMaster* xmlParseMaster):
		SharedData(xmlParseMaster), IsParsingElement(false), IsParsingMatrix(false), mScope(nullptr)
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
		data->IsParsingMatrix = IsParsingMatrix;
		data->MatrixVectors = MatrixVectors;

		return data;
	}

	/************************************************************************/
	void TableSharedData::Initialize()
	{
		SharedData::Initialize();
		IsParsingElement = false;
		IsParsingMatrix = false;
		MatrixVectors.Clear();
		delete mScope;
		mScope = nullptr;
	}

	/************************************************************************/
	Scope* TableSharedData::GetScope() const
	{
		return mScope;
	}

	/************************************************************************/
	void TableSharedData::SetScope(Scope& scope)
	{
		mScope = &scope;
	}

}