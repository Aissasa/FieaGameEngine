#include "pch.h"
#include "XmlParseMaster.h"
#include "XmlParseHelperNumber.h"
#include "TableSharedData.h"
#include "WorldSharedData.h"
#include "Entity.h"


using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperNumber)

	const string XmlParseHelperNumber::INTEGER_ELEMENT_NAME = "Integer";
	const string XmlParseHelperNumber::FLOAT_ELEMENT_NAME = "Float";
	const string XmlParseHelperNumber::NAME_ELEMENT_NAME = "Name";
	const string XmlParseHelperNumber::VALUE_ATTRIBUTE_NAME = "Value";

	/************************************************************************/
	XmlParseHelperNumber::XmlParseHelperNumber():
		IXmlParseHelper(), mNameString(), mDataString(), mDataGotSet(false), mIsParsing(false)
	{
	}

	/************************************************************************/
	void XmlParseHelperNumber::Initialize()
	{
		IXmlParseHelper::Initialize();
		Reset();
	}

	/************************************************************************/
	bool XmlParseHelperNumber::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && (el == INTEGER_ELEMENT_NAME || el == FLOAT_ELEMENT_NAME))
		{
			if (mIsParsing)
			{
				throw exception("This handler is already parsing another number element.");
			}

			if (sharedData.As<TableSharedData>()->IsParsingElement)
			{
				throw exception("Cannot have nested elements in a number element.");
			}

			// look for the name
			bool nameFound = false;
			for (auto& attr : attributes)
			{
				if (attr.first == NAME_ELEMENT_NAME)
				{
					nameFound = true;
					mNameString = attr.second;
					break;
				}
			}

			if (!nameFound)
			{
				throw exception("The number element needs to have a name.");
			}

			// look for the value in the attributes
			for (auto& attr : attributes)
			{
				if (attr.first == VALUE_ATTRIBUTE_NAME)
				{
					mDataGotSet = true;
					mDataString = attr.second;
					break;
				}
			}

			sharedData.As<TableSharedData>()->IsParsingElement = true;
			mIsParsing = true;
			++mStartElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperNumber::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && (el == INTEGER_ELEMENT_NAME || el == FLOAT_ELEMENT_NAME))
		{
			if (!mIsParsing)
			{
				throw exception("Cannot call EndElementHandler before StartElementHandler.");
			}

			Datum* dat;
			// set the name and the data
			if (sharedData.Is(WorldSharedData::TypeIdClass()))
			{
				dat = &sharedData.As<WorldSharedData>()->GetScope()->As<Attributed>()->AppendAuxiliaryAttribute(mNameString);
			}
			else
			{
				dat = &sharedData.As<TableSharedData>()->GetScope()->Append(mNameString);
			}

			if (el == INTEGER_ELEMENT_NAME)
			{
				dat->PushBack(stoi(mDataString));
			}
			if (el == FLOAT_ELEMENT_NAME)
			{
				dat->PushBack(stof(mDataString));
			}

			++mEndElementHandlerCount;
			sharedData.As<TableSharedData>()->IsParsingElement = false;
			Reset();

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperNumber::CharDataHandler(XmlParseMaster::SharedData & sharedData, const std::string & str)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()))
		{
			if (!mIsParsing)
			{
				return false;
			}

			if (!mDataGotSet)
			{
				mDataString.append(str);
			}

			return true;
		}

		return false;
	}

	/************************************************************************/
	IXmlParseHelper* XmlParseHelperNumber::Clone()
	{
		XmlParseHelperNumber* helper = new XmlParseHelperNumber();
		helper->mStartElementHandlerCount = mStartElementHandlerCount;
		helper->mEndElementHandlerCount = mEndElementHandlerCount;
		helper->mCharDataHandlerCount = mCharDataHandlerCount;
		helper->mDataGotSet = mDataGotSet;
		helper->mIsParsing = mIsParsing;
		helper->mNameString = mNameString;
		helper->mDataString = mDataString;

		return helper;
	}

	/************************************************************************/
	void XmlParseHelperNumber::Reset()
	{
		mDataGotSet = false;
		mIsParsing = false;
		mNameString = "";
		mDataString = "";
	}
}
