#include "pch.h"
#include "XmlParseMaster.h"
#include "XmlParseHelperFloat.h"
#include "TableSharedData.h"


using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperFloat)

	const std::string XmlParseHelperFloat::FLOAT_ELEMENT_NAME = "Float";
	const std::string XmlParseHelperFloat::NAME_ELEMENT_NAME = "Name";
	const std::string XmlParseHelperFloat::VALUE_ATTRIBUTE_NAME = "Value";

	/************************************************************************/
	XmlParseHelperFloat::XmlParseHelperFloat() :
		mIsParsing(false), mDataGotSet(false), mNameString(), mDataString(), IXmlParseHelper()
	{
	}

	/************************************************************************/
	void XmlParseHelperFloat::Initialize()
	{
		IXmlParseHelper::Initialize();
		Reset();
	}

	/************************************************************************/
	bool XmlParseHelperFloat::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && el == FLOAT_ELEMENT_NAME)
		{
			if (sharedData.As<TableSharedData>()->IsParsingElement)
			{
				throw exception("Cannot have nested elements in an float element.");
			}

			if (mIsParsing)
			{
				throw exception("This handler is already parsing another float element.");
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
				throw exception("The float element needs to have a name.");
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
	bool XmlParseHelperFloat::EndElementHandler(Library::XmlParseMaster::SharedData & sharedData, const std::string & el)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && el == FLOAT_ELEMENT_NAME)
		{
			// set the name and the data
			Datum& dat = sharedData.As<TableSharedData>()->GetScope().Append(mNameString);
			dat.PushBack(stof(mDataString));

			++mEndElementHandlerCount;
			sharedData.As<TableSharedData>()->IsParsingElement = false;
			Reset();

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperFloat::CharDataHandler(SharedDataC & sharedData, const std::string & str)
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
	IXmlParseHelper* XmlParseHelperFloat::Clone()
	{
		XmlParseHelperFloat* helper = new XmlParseHelperFloat();
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
	void XmlParseHelperFloat::Reset()
	{
		mDataGotSet = false;
		mIsParsing = false;
		mNameString = "";
		mDataString = "";
	}
}
