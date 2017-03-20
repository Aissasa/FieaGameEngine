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
	}

	/************************************************************************/
	XmlParseMaster::SharedData* XmlParseMaster::SharedData::Clone()
	{
		SharedData* clone = new SharedData(mXmlParseMaster);
		clone->mDepth = mDepth;
		return clone;
	}

	/************************************************************************/
	void XmlParseMaster::SharedData::SetXmlParseMaster(XmlParseMaster *& xmlParseMaster)
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
		--mDepth;
	}

	/************************************************************************/
	std::uint32_t XmlParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}

#pragma endregion

#pragma region XmlParseMaster

	/************************************************************************/
	XmlParseMaster::XmlParseMaster(SharedData *& sharedData) :
		mSharedData(sharedData), mHelpers(), mCashedFileName(""), mCloned(false)
	{
		mParser = XML_ParserCreate(nullptr);
		XML_SetUserData(mParser ,mSharedData);
		XML_SetElementHandler(mParser, StartElementHandler, EndElementHandler);
		XML_SetCharacterDataHandler(mParser, CharDataHandler);
	}

	/************************************************************************/
	XmlParseMaster::~XmlParseMaster()
	{
		if (mCloned)
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
		SharedData* data = mSharedData->Clone();

		XmlParseMaster* newXmlParseMaster = new XmlParseMaster(data);
		newXmlParseMaster->mHelpers = mHelpers;
		newXmlParseMaster->mCashedFileName = mCashedFileName;
		newXmlParseMaster->mParser = mParser;
		newXmlParseMaster->mCloned = true;

		return newXmlParseMaster;
	}

	/************************************************************************/
	void XmlParseMaster::AddHelper(IXmlParseHelper* & helper)
	{
		mHelpers.PushBack(helper);
	}

	/************************************************************************/
	void XmlParseMaster::RemoveHelper(IXmlParseHelper* & helper)
	{
		mHelpers.Remove(helper);
	}

	/************************************************************************/
	int XmlParseMaster::Parse(const char* buffer, const std::uint32_t bufferSize, const bool lastChunk)
	{
		static bool firstCall = true;
		if (firstCall)
		{
			firstCall = false;
			for (auto& helper : mHelpers)
			{
				helper->Initialize();
			}
		}

		return XML_Parse(mParser, buffer, bufferSize, lastChunk);
	}

	/************************************************************************/
	int XmlParseMaster::ParseFromFile(const std::string& fileName)
	{
		int result = -1;
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
		return mCashedFileName;
	}

	/************************************************************************/
	XmlParseMaster::SharedData* XmlParseMaster::GetSharedData() const
	{
		return mSharedData;
	}

	/************************************************************************/
	void XmlParseMaster::SetSharedData(SharedData *& sharedData)
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

		for (uint32_t i = 0; atts[i]; i+=2)
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

		if (master->mHelpers.Size() == 0)
		{
			throw exception("No helpers are available.");
		}

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

#pragma endregion

}
