#include "pch.h"
#include "XmlParseMaster.h"
#include "IXmlParseHelper.h"


using namespace std;

namespace Library
{

#pragma region SharedData

	RTTI_DEFINITIONS(XmlParseMaster::SharedData);

	/************************************************************************/
	XmlParseMaster::SharedData::SharedData(XmlParseMaster *xmlParseMaster) :
		mXmlParseMaster(xmlParseMaster), mDepth(0)
	{
		if (mXmlParseMaster)
		{
			mXmlParseMaster->SetSharedData(*this);
		}
	}

	/************************************************************************/
	void XmlParseMaster::SharedData::Initialize()
	{
		mDepth = 0;
	}

	/************************************************************************/
	void XmlParseMaster::SharedData::SetXmlParseMaster(XmlParseMaster * xmlParseMaster)
	{
		mXmlParseMaster = xmlParseMaster;
	}

	/************************************************************************/
	XmlParseMaster * XmlParseMaster::SharedData::GetXmlParseMaster() const
	{
		return mXmlParseMaster;
	}

	/************************************************************************/
	void XmlParseMaster::SharedData::IncrementDepth()
	{
		++mDepth;
	}

	/************************************************************************/
	void XmlParseMaster::SharedData::DecrementDepth()
	{
		if (mDepth > 0)
		{
			--mDepth;
		}
	}

	/************************************************************************/
	std::uint32_t XmlParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}

#pragma endregion

#pragma region XmlParseMaster

	/************************************************************************/
	XmlParseMaster::XmlParseMaster(SharedData* sharedData) :
		mParser(XML_ParserCreate(nullptr)), mSharedData(sharedData), mHelpers(), mCachedFileName(), mIsCloned(false)
	{
		if (mSharedData)
		{
			mSharedData->SetXmlParseMaster(this);
		}
	}

	/************************************************************************/
	XmlParseMaster::~XmlParseMaster()
	{
		if (mIsCloned)
		{
			delete mSharedData;
			for (auto& helper : mHelpers)
			{
				delete helper;
			}
		}
		mHelpers.Clear();
		XML_ParserFree(mParser);
	}

	/************************************************************************/
	XmlParseMaster * XmlParseMaster::Clone()
	{
		SharedData* data = mSharedData ? mSharedData->Clone() : mSharedData;

		XmlParseMaster* newXmlParseMaster = new XmlParseMaster(data);
		for (auto& helper : mHelpers)
		{
			newXmlParseMaster->AddHelper(*(helper->Clone()));
		}
		newXmlParseMaster->mCachedFileName = mCachedFileName;
		newXmlParseMaster->mIsCloned = true;

		return newXmlParseMaster;
	}

	/************************************************************************/
	void XmlParseMaster::AddHelper(IXmlParseHelper & helper)
	{
		if (mIsCloned)
		{
			throw exception("This XmlParseMaster is a clone, you cannot add helpers to it.");
		}

		if (mHelpers.Find(&helper) != mHelpers.end())
		{
			throw exception("This helper already exists");
		}
		mHelpers.PushBack(&helper);
	}

	/************************************************************************/
	void XmlParseMaster::RemoveHelper(IXmlParseHelper & helper)
	{
		if (mIsCloned)
		{
			throw exception("This XmlParseMaster is a clone, you cannot remove helpers from it.");
		}
		mHelpers.Remove(&helper);
	}

	/************************************************************************/
	void XmlParseMaster::Parse(const char* buffer, const std::uint32_t bufferSize, const bool firstChunk, const bool lastChunk)
	{
		if (firstChunk)
		{
			Reset();
		}
	
 		if (XML_Parse(mParser, buffer, bufferSize, lastChunk) != XML_STATUS_OK)
 		{
			string errorMsg = "Parsing failed! ";
			errorMsg.append(XML_ErrorString(XML_GetErrorCode(mParser)));
 			throw exception(errorMsg.c_str());
 		}
	}

	/************************************************************************/
	void XmlParseMaster::ParseFromFile(const std::string& fileName)
	{
		mCachedFileName = fileName;

		std::ifstream in(fileName);
		std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
		Parse(contents.c_str(), static_cast<uint32_t>(contents.length()));
	}

	/************************************************************************/
	std::string XmlParseMaster::GetFileName() const
	{
		return mCachedFileName;
	}

	/************************************************************************/
	XmlParseMaster::SharedData& XmlParseMaster::GetSharedData() const
	{
		return *mSharedData;
	}

	/************************************************************************/
	void XmlParseMaster::SetSharedData(SharedData& sharedData)
	{
		if (mIsCloned)
		{
			throw exception("This XmlParseMaster is a clone, you cannot set its SharedData.");
		}
		mSharedData = &sharedData;
		mSharedData->SetXmlParseMaster(this);
	}

	/************************************************************************/
	void XmlParseMaster::StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts)
	{
		assert(userData != nullptr);

		XmlParseMaster* master = static_cast<XmlParseMaster*>(userData);

		if (master->mHelpers.Size() == 0)
		{
			throw exception("No helpers are available.");
		}

		string el(name);
		HashMap<string, string> attributes;

		for (uint32_t i = 0; atts[i]; i += 2)
		{
			attributes.Insert(pair<string, string>(string(atts[i]), string(atts[i + 1])));
		}

		for (auto& helper : master->mHelpers)
		{
			if (helper->StartElementHandler(*(master->mSharedData), el, attributes))
			{
				break;
			}
		}

		master->GetSharedData().IncrementDepth();
	}

	/************************************************************************/
	void XmlParseMaster::EndElementHandler(void *userData, const XML_Char *name)
	{
		XmlParseMaster* master = static_cast<XmlParseMaster*>(userData);

		if (master->mHelpers.Size() == 0)
		{
			throw exception("No helpers are available.");
		}

		master->GetSharedData().DecrementDepth();

		string el(name);
		for (auto& helper : master->mHelpers)
		{
			if (helper->EndElementHandler(*(master->mSharedData), el))
			{
				break;
			}
		}
	}

	/************************************************************************/
	void XmlParseMaster::CharDataHandler(void *userData, const XML_Char *s, int len)
	{
		XmlParseMaster* master = static_cast<XmlParseMaster*>(userData);

		if (master->mHelpers.Size() == 0)
		{
			throw exception("No helpers are available.");
		}

		string el(s, len);
		for (auto& helper : master->mHelpers)
		{
			if (helper->CharDataHandler(*(master->mSharedData), el))
			{
				break;
			}
		}
	}

	/************************************************************************/
	void XmlParseMaster::Reset()
	{
		XML_ParserReset(mParser, NULL);
		XML_SetUserData(mParser, this);
		XML_SetElementHandler(mParser, StartElementHandler, EndElementHandler);
		XML_SetCharacterDataHandler(mParser, CharDataHandler);

		for (auto& helper : mHelpers)
		{
			helper->Initialize();
		}
		mSharedData->Initialize();
		mCachedFileName.clear();
	}

#pragma endregion

}
