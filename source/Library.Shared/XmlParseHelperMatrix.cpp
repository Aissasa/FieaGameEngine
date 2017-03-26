#include "pch.h"
#include "XmlParseMaster.h"
#include "XmlParseHelperMatrix.h"
#include "TableSharedData.h"


using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(XmlParseHelperMatrix)

	const std::string XmlParseHelperMatrix::MATRIX_ELEMENT_NAME = "Matrix";
	const std::string XmlParseHelperMatrix::NAME_ELEMENT_NAME = "Name";

	/************************************************************************/
	XmlParseHelperMatrix::XmlParseHelperMatrix() :
		mIsParsing(false), IXmlParseHelper()
	{
	}

	/************************************************************************/
	void XmlParseHelperMatrix::Initialize()
	{
		IXmlParseHelper::Initialize();
		Reset();
	}

	/************************************************************************/
	bool XmlParseHelperMatrix::StartElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el, const HashMap<std::string, std::string>& attributes)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && el == MATRIX_ELEMENT_NAME)
		{
			if (mIsParsing)
			{
				throw exception("This handler is already parsing another matrix element.");
			}

			if (sharedData.As<TableSharedData>()->IsParsingElement)
			{
				throw exception("Cannot have nested elements in a matrix element.");
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
				throw exception("The matrix element needs to have a name.");
			}

			sharedData.As<TableSharedData>()->IsParsingMatrix = true;
			sharedData.As<TableSharedData>()->IsParsingElement = true;
			mIsParsing = true;
			++mStartElementHandlerCount;

			return true;
		}

		return false;
	}

	/************************************************************************/
	bool XmlParseHelperMatrix::EndElementHandler(XmlParseMaster::SharedData & sharedData, const std::string & el)
	{
		if (sharedData.Is(TableSharedData::TypeIdClass()) && el == MATRIX_ELEMENT_NAME)
		{
			if (!mIsParsing)
			{
				throw exception("Cannot call EndElementHandler before StartElementHandler.");
			}

			if (sharedData.As<TableSharedData>()->MatrixVectors.Size() < EXPECTED_NUMBER_OF_VECTORS)
			{
				throw exception("Insufficient number of vectors to build the matrix.");
			}

			Vector<vec4> vectors = sharedData.As<TableSharedData>()->MatrixVectors;

			mat4x4 matrixToAdd(vectors[0], vectors[1], vectors[2], vectors[3]);

			Datum& dat = sharedData.As<TableSharedData>()->GetScope().Append(mNameString);
			dat.PushBack(matrixToAdd);

			++mEndElementHandlerCount;
			sharedData.As<TableSharedData>()->IsParsingElement = false;
			sharedData.As<TableSharedData>()->IsParsingMatrix = false;
			sharedData.As<TableSharedData>()->MatrixVectors.Clear();
			Reset();

			return true;
		}

		return false;
	}

	/************************************************************************/
	IXmlParseHelper* XmlParseHelperMatrix::Clone()
	{
		XmlParseHelperMatrix* helper = new XmlParseHelperMatrix();
		helper->mStartElementHandlerCount = mStartElementHandlerCount;
		helper->mEndElementHandlerCount = mEndElementHandlerCount;
		helper->mCharDataHandlerCount = mCharDataHandlerCount;
		helper->mIsParsing = mIsParsing;
		helper->mNameString = mNameString;

		return helper;
	}

	/************************************************************************/
	void XmlParseHelperMatrix::Reset()
	{
		mIsParsing = false;
		mNameString.clear();
	}
}