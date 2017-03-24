#include "pch.h"
#include "XmlParseMaster.h"
#include "XmlParseHelperVector.h"
#include "TableSharedData.h"
#include "Vector.h"


using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperVector)

	const std::string XmlParseHelperVector::VECTOR_ELEMENT_NAME = "Vector";
	const std::string XmlParseHelperVector::NAME_ELEMENT_NAME = "Name";
	const std::string XmlParseHelperVector::X_ATTRIBUTE_NAME = "x";
	const std::string XmlParseHelperVector::Y_ATTRIBUTE_NAME = "y";
	const std::string XmlParseHelperVector::Z_ATTRIBUTE_NAME = "z";
	const std::string XmlParseHelperVector::W_ATTRIBUTE_NAME = "w";

	/************************************************************************/
	XmlParseHelperVector::XmlParseHelperVector() :
		mIsXSet(false), mIsYSet(false), mIsZSet(false), mIsWSet(false), mIsParsing(false), IXmlParseHelper()
	{
	}

	/************************************************************************/
	void XmlParseHelperVector::Initialize()
	{
		IXmlParseHelper::Initialize();
		Reset();
	}

	/************************************************************************/
	bool XmlParseHelperVector::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && el == VECTOR_ELEMENT_NAME)
		{
			if (sharedData.As<TableSharedData>()->IsParsingElement && !sharedData.As<TableSharedData>()->IsParsingMatrix)
			{
				throw exception("Cannot have nested elements in a vector element.");
			}

			if (mIsParsing)
			{
				throw exception("This handler is already parsing another vector element.");
			}

			// look for the name
			bool nameFound = false;
			string name;
			for (auto& attr : attributes)
			{
				if (attr.first == NAME_ELEMENT_NAME)
				{
					nameFound = true;
					name = attr.second;
					break;
				}
			}

			if (!nameFound)
			{
				throw exception("The vector element needs to have a name.");
			}

			// look for the value in the attributes
			vec4 vectorToAdd;
			for (auto& attr : attributes)
			{
				if (attr.first == X_ATTRIBUTE_NAME)
				{
					vectorToAdd.x = stof(attr.second);
					mIsXSet = true;
					continue;
				}

				if (attr.first == Y_ATTRIBUTE_NAME)
				{
					vectorToAdd.y = stof(attr.second);
					mIsYSet = true;
					continue;
				}

				if (attr.first == Z_ATTRIBUTE_NAME)
				{
					vectorToAdd.z = stof(attr.second);
					mIsZSet = true;
					continue;
				}

				if (attr.first == W_ATTRIBUTE_NAME)
				{
					vectorToAdd.w = stof(attr.second);
					mIsWSet = true;
					continue;
				}
			}

			if (!mIsXSet || !mIsYSet || !mIsZSet || !mIsWSet)
			{
				throw exception("Missing information for the vector element to be set up.");
			}

			if (sharedData.As<TableSharedData>()->IsParsingMatrix)
			{
				// to be added to the matrix later
				sharedData.As<TableSharedData>()->MatrixVectors.PushBack(vectorToAdd);
			}
			else
			{
				// set the name and the data
				Datum& dat = sharedData.As<TableSharedData>()->GetScope().Append(name);
				dat.PushBack(vectorToAdd);
				++mStartElementHandlerCount;
			}

			sharedData.As<TableSharedData>()->IsParsingElement = true;
			mIsParsing = true;

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperVector::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && el == VECTOR_ELEMENT_NAME)
		{
			++mEndElementHandlerCount;
			sharedData.As<TableSharedData>()->IsParsingElement = false;
			Reset();

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperVector::CharDataHandler(SharedDataC & sharedData, const std::string & str)
	{
		UNREFERENCED_PARAMETER(str);
		if (sharedData.Is(TableSharedData::TypeIdClass()))
		{
			if (!mIsParsing)
			{
				throw exception("Cannot call CharDataHandler before StartElementHandler");
			}

			return true;
		}

		return false;
	}

	/************************************************************************/
	IXmlParseHelper* XmlParseHelperVector::Clone()
	{
		XmlParseHelperVector* helper = new XmlParseHelperVector();
		helper->mStartElementHandlerCount = mStartElementHandlerCount;
		helper->mEndElementHandlerCount = mEndElementHandlerCount;
		helper->mCharDataHandlerCount = mCharDataHandlerCount;
		helper->mIsParsing = mIsParsing;
		helper->mIsXSet = mIsXSet;
		helper->mIsYSet = mIsYSet;
		helper->mIsZSet = mIsZSet;
		helper->mIsWSet = mIsWSet;

		return helper;
	}

	/************************************************************************/
	void XmlParseHelperVector::Reset()
	{
		mIsParsing = false;
		mIsXSet = false;
		mIsYSet = false;
		mIsZSet = false;
		mIsWSet = false;
	}
}