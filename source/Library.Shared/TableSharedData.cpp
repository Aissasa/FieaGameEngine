#include "pch.h"
#include "TableSharedData.h"

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

		return data;
	}

	/************************************************************************/
	void TableSharedData::Initialize()
	{
		XmlParseMaster::SharedData::Initialize();
		mScope->Clear();
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