#include "pch.h"
#include "XmlParseMaster.h"
#include "XmlParseHelperString.h"
#include "TableSharedData.h"


using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperString)

	const std::string XmlParseHelperString::STRING_ELEMENT_NAME = "String";
	const std::string XmlParseHelperString::NAME_ELEMENT_NAME = "Name";
	const std::string XmlParseHelperString::VALUE_ATTRIBUTE_NAME = "Value";

	/************************************************************************/
	XmlParseHelperString::XmlParseHelperString() :
		mIsParsing(false), mDataGotSet(false), mNameString(), mDataString(), IXmlParseHelper()
	{
	}

	/************************************************************************/
	void XmlParseHelperString::Initialize()
	{
		IXmlParseHelper::Initialize();
		Reset();
	}

	/************************************************************************/
	bool XmlParseHelperString::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && el == STRING_ELEMENT_NAME)
		{
			if (mIsParsing)
			{
				throw exception("This handler is already parsing another string element.");
			}

			if (sharedData.As<TableSharedData>()->IsParsingElement)
			{
				throw exception("Cannot have nested elements in a string element.");
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
				throw exception("The string element needs to have a name.");
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
	bool XmlParseHelperString::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && el == STRING_ELEMENT_NAME)
		{
			if (!mIsParsing)
			{
				throw exception("Cannot call EndElementHandler before StartElementHandler.");
			}

			// set the name and the data
			Datum& dat = sharedData.As<TableSharedData>()->GetScope().Append(mNameString);
			dat.PushBack(mDataString);

			++mEndElementHandlerCount;
			sharedData.As<TableSharedData>()->IsParsingElement = false;
			Reset();

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperString::CharDataHandler(XmlParseMaster::SharedData & sharedData, const std::string & str)
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
	IXmlParseHelper* XmlParseHelperString::Clone()
	{
		XmlParseHelperString* helper = new XmlParseHelperString();
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
	void XmlParseHelperString::Reset()
	{
		mDataGotSet = false;
		mIsParsing = false;
		mNameString.clear();
		mDataString.clear();
	}
}