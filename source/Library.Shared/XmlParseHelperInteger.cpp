#include "pch.h"
#include "XmlParseMaster.h"
#include "XmlParseHelperInteger.h"
#include "TableSharedData.h"


using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperInteger)

	const std::string XmlParseHelperInteger::INTEGER_ELEMENT_NAME = "Integer";
	const std::string XmlParseHelperInteger::NAME_ELEMENT_NAME = "Name";
	const std::string XmlParseHelperInteger::VALUE_ATTRIBUTE_NAME = "Value";

	/************************************************************************/
	XmlParseHelperInteger::XmlParseHelperInteger():
		mIsParsing(false), mDataGotSet(false), mNameString(), mDataString(), IXmlParseHelper()
	{
	}

	/************************************************************************/
	void XmlParseHelperInteger::Initialize()
	{
		IXmlParseHelper::Initialize();
		Reset();
	}

	/************************************************************************/
	bool XmlParseHelperInteger::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && el == INTEGER_ELEMENT_NAME)
		{
			if (sharedData.As<TableSharedData>()->IsParsingElement)
			{
				throw exception("Cannot have nested elements in an integer element.");
			}

			if (mIsParsing)
			{
				throw exception("This handler is already parsing another integer element.");
			}

			sharedData.As<TableSharedData>()->IsParsingElement = true;
			mIsParsing = true;

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
				throw exception("The integer element needs to have a name.");
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

			++mStartElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperInteger::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && el == INTEGER_ELEMENT_NAME)
		{
			// set the name and the data
			Datum& dat = sharedData.As<TableSharedData>()->GetScope().Append(mNameString);
			dat.PushBack(stoi(mDataString));

			++mEndElementHandlerCount;
			sharedData.As<TableSharedData>()->IsParsingElement = false;
			Reset();

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperInteger::CharDataHandler(SharedDataC & sharedData, const std::string & str)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()))
		{
			if (!mIsParsing)
			{
				throw exception("Cannot call CharDataHandler before StartElementHandler");
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
	IXmlParseHelper* XmlParseHelperInteger::Clone()
	{
		XmlParseHelperInteger* helper = new XmlParseHelperInteger();
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
	void XmlParseHelperInteger::Reset()
	{
		mDataGotSet = false;
		mIsParsing = false;
		mNameString = "";
		mDataString = "";
	}
}
