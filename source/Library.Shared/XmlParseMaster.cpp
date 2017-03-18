#include "pch.h"
#include "XmlParseMaster.h"

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
		mSharedData(sharedData), mHelpers(), mCashedFileName(""), mParser()
	{
	}

	/************************************************************************/
	XmlParseMaster * XmlParseMaster::Clone()
	{
		// urgent implement
		return nullptr;
	}

	/************************************************************************/
	void XmlParseMaster::AddHelper(const IXmlParseHelper & helper)
	{
		mHelpers.PushBack(helper);
	}

	/************************************************************************/
	void XmlParseMaster::RemoveHelper(const IXmlParseHelper & helper)
	{
		mHelpers.Remove(helper);
	}

	/************************************************************************/
	int XmlParseMaster::Parse(const char * buffer, const std::uint32_t bufferSize, const bool lastChunk)
	{
		// urgent implement this : ps , for loop helpers
		return 0;
	}

	/************************************************************************/
	int XmlParseMaster::ParseFromFile(const std::string & fileName)
	{
		// urgent implement this : ps , for loop helpers
		return 0;
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
	bool XmlParseMaster::StartElementHandler(const std::string & el, const HashMap<std::string, std::string>& attributes)
	{
		// urgent implement
		return false;
	}

	/************************************************************************/
	bool XmlParseMaster::EndElementHandler(const std::string & el)
	{
		// urgent implement
		return false;
	}

	/************************************************************************/
	bool XmlParseMaster::CharDataHandler(const char *& str, const std::uint32_t length)
	{
		// urgent implement
		return false;
	}

#pragma endregion

}
