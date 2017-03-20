#pragma once

#include "RTTI.h"
#include "Vector.h"
#include <expat.h>

namespace Library
{
	class IXmlParseHelper;

	class XmlParseMaster final
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

		protected:

			XmlParseMaster* mXmlParseMaster;
			std::uint32_t mDepth;

		};

		XmlParseMaster(SharedData*& sharedData);

		~XmlParseMaster();

		XmlParseMaster(const XmlParseMaster & rhs) = delete;
		XmlParseMaster& operator=(const XmlParseMaster& rhs) = delete;

		XmlParseMaster* Clone();
		
		void AddHelper(IXmlParseHelper*& helper);

		void RemoveHelper(IXmlParseHelper*& helper);

		int Parse(const char* buffer, const std::uint32_t bufferSize, const bool lastChunk = true);

		int ParseFromFile(const std::string& fileName);

		std::string GetFileName() const;

		SharedData* GetSharedData() const;

		void SetSharedData(SharedData*& sharedData);

	private:

		static void StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts);
		static void EndElementHandler(void *userData, const XML_Char *name);
		static void CharDataHandler(void *userData, const XML_Char *s, int len);

		SharedData* mSharedData;
		Vector<IXmlParseHelper*> mHelpers;
		std::string mCashedFileName;
		XML_Parser mParser;
		bool mCloned;
	};
}