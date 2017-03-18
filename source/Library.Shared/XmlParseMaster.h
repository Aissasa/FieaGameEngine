#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"
#include "Vector.h"

namespace Library
{
	class XmlParseMaster
	{
	public:

		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI);

		public:

			SharedData();

			~SharedData() = default;

			SharedData(const SharedData & rhs) = delete;
			SharedData& operator=(const SharedData& rhs) = delete;

			virtual SharedData* Clone() = 0;

			void SetXmlParseMaster(const XmlParseMaster*& xmlParseMaster);

			XmlParseMaster* GetXmlParseMaster() const;

			void IncrementDepth();

			void DecrementDepth();

			std::uint32_t Depth() const;

		private:

			XmlParseMaster* mXmlParseMaster;
			std::uint32_t mDepth;

		};

		XmlParseMaster(const SharedData*& sharedData);

		~XmlParseMaster();

		XmlParseMaster(const XmlParseMaster & rhs) = delete;
		XmlParseMaster& operator=(const XmlParseMaster& rhs) = delete;

		XmlParseMaster* Clone();
		
		void AddHelper(const IXmlParseHelper& helper);

		void RemoveHelper(const IXmlParseHelper& helper);

		int Parse(const char* buffer, const std::uint32_t bufferSize, const bool lastChunk = false);

		int ParseFromFile(const std::string& fileName);

		std::string GetFileName() const;

		SharedData* GetSharedData() const;

		void SetSharedData(const SharedData*& sharedData);

	private:

		void StartElementHandler();
		void EndElementHandler();
		void CharDataHandler();

		SharedData* mSharedData;
		Vector<IXmlParseHelper> mHelpers;
		std::string mCashedFileName;
	};

}