#pragma once

#include "RTTI.h"
#include "IXmlParseHelper.h"
#include "Vector.h"
#include <expat.h>

namespace Library
{
	class XmlParseMaster
	{
	public:

		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI);

		public:

			SharedData(XmlParseMaster* xmlParseMaster = nullptr);

			virtual ~SharedData() = default;

			SharedData(const SharedData & rhs) = delete;
			SharedData& operator=(const SharedData& rhs) = delete;

			virtual SharedData* Clone();

			void SetXmlParseMaster(XmlParseMaster*& xmlParseMaster);

			XmlParseMaster* GetXmlParseMaster() const;

			void IncrementDepth();

			void DecrementDepth();

			std::uint32_t Depth() const;

		private:

			XmlParseMaster* mXmlParseMaster;
			std::uint32_t mDepth;

		};

		XmlParseMaster(SharedData*& sharedData);

		~XmlParseMaster() = default;

		XmlParseMaster(const XmlParseMaster & rhs) = delete;
		XmlParseMaster& operator=(const XmlParseMaster& rhs) = delete;

		XmlParseMaster* Clone();
		
		void AddHelper(const IXmlParseHelper& helper);

		void RemoveHelper(const IXmlParseHelper& helper);

		int Parse(const char* buffer, const std::uint32_t bufferSize, const bool lastChunk = false);

		int ParseFromFile(const std::string& fileName);

		std::string GetFileName() const;

		SharedData* GetSharedData() const;

		void SetSharedData(SharedData*& sharedData);

	private:

		static bool StartElementHandler(const std::string& el, const HashMap<std::string, std::string>& attributes);
		static bool EndElementHandler(const std::string& el);
		static bool CharDataHandler(const char*& str, const std::uint32_t length);

		SharedData* mSharedData;
		Vector<IXmlParseHelper> mHelpers;
		std::string mCashedFileName;
		XML_Parser mParser;
	};

}