#include "pch.h"
#include "TableSharedData.h"

namespace Library
{
	RTTI_DEFINITIONS(TableSharedData)

	/************************************************************************/
	TableSharedData::TableSharedData(XmlParseMaster* xmlParseMaster):
		mIsParsingElement(false), XmlParseMaster::SharedData(xmlParseMaster)
	{
		mScope = new Scope();
		mCurrentScope = mScope;
	}

	/************************************************************************/
	TableSharedData::~TableSharedData()
	{
		if (mScope)
		{
			mScope->Clear();
		}
	}

	/************************************************************************/
	XmlParseMaster::SharedData * TableSharedData::Clone()
	{
		TableSharedData* data = new TableSharedData(mXmlParseMaster);
		data->mDepth = mDepth;
		data->mIsParsingElement = mIsParsingElement;

		return data;
	}

	/************************************************************************/
	void TableSharedData::Initialize()
	{
		XmlParseMaster::SharedData::Initialize();
		mScope->Clear();
	}

	/************************************************************************/
	Scope * TableSharedData::GetScope() const
	{
		return mScope;
	}

	/************************************************************************/
	Scope * TableSharedData::GetCurrentScope() const
	{
		//Scope* scope = nullptr;
		//if (mScope)
		//{
		//	scope = mScope;
		//	while (scope->GetParent())
		//	{
		//		scope = scope->GetParent();
		//	}
		//}

		//return scope;
		return mCurrentScope;
	}

	/************************************************************************/
	void TableSharedData::SetCurrentScope(Scope * scope)
	{
		mCurrentScope = scope;
	}

	/************************************************************************/
	bool TableSharedData::GetIsParsingElement() const
	{
		return mIsParsingElement;
	}

	/************************************************************************/
	void TableSharedData::SetIsParsingElement(bool isParsingElement)
	{
		mIsParsingElement = isParsingElement;
	}

}