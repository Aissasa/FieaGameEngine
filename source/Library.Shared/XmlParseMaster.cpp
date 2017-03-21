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
			mXmlParseMaster->SetSharedData(this);
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
		mSharedData(sharedData), mHelpers(), mCachedFileName(""), mIsCloned(false)
	{
		if (mSharedData)
		{
			mSharedData->SetXmlParseMaster(this);
		}

		mParser = XML_ParserCreate(nullptr);
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
	int XmlParseMaster::Parse(const char* buffer, const std::uint32_t bufferSize, const bool lastChunk)
	{
		static bool firstCall = true;
		if (firstCall)
		{
			firstCall = false;

			ParserReset();
			for (auto& helper : mHelpers)
			{
				helper->Initialize();
			}
			mSharedData->Initialize();
		}

		if (lastChunk)
		{
			firstCall = true;
		}

		return XML_Parse(mParser, buffer, bufferSize, lastChunk);
	}

	/************************************************************************/
	int XmlParseMaster::ParseFromFile(const std::string& fileName)
	{
		int result = -10;
		mCachedFileName = fileName;
		std::ifstream file(fileName, std::ios::in | std::ios::binary | std::ios::ate);
		if (file.is_open())
		{
			file.seekg(0, std::ios::end);
			uint32_t size = static_cast<uint32_t>(file.tellg());
			char *contents = new char[size];
			file.seekg(0, std::ios::beg);
			file.read(contents, size);
			file.close();

			result = Parse(contents, size);

			delete[] contents;
		}
		return result;
	}

	/************************************************************************/
	std::string XmlParseMaster::GetFileName() const
	{
		return mCachedFileName;
	}

	/************************************************************************/
	XmlParseMaster::SharedData* XmlParseMaster::GetSharedData() const
	{
		return mSharedData;
	}

	/************************************************************************/
	void XmlParseMaster::SetSharedData(SharedData * sharedData)
	{
		mSharedData = sharedData;
	}

	/************************************************************************/
	void XmlParseMaster::StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts)
	{
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
	}

	/************************************************************************/
	void XmlParseMaster::EndElementHandler(void *userData, const XML_Char *name)
	{
		XmlParseMaster* master = static_cast<XmlParseMaster*>(userData);

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
	void XmlParseMaster::ParserReset()
	{
		XML_ParserReset(mParser, NULL);
		XML_SetUserData(mParser, this);
		XML_SetElementHandler(mParser, StartElementHandler, EndElementHandler);
		XML_SetCharacterDataHandler(mParser, CharDataHandler);
	}

#pragma endregion

}
