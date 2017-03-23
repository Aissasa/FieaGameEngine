#pragma once

#include "RTTI.h"
#include "Vector.h"
#include <expat.h>

namespace Library
{

	/** Forward declaration of IXmlParseHelper.
	* @see IXmlParseHelper
	*/
	class IXmlParseHelper;

	/** XmlParseMaster is an Xml Parser that uses IXmlParseHelper.
	* It uses the C library Expat to do the parsing.
	*/
	class XmlParseMaster final
	{
	public:
		
		/** SharedData is an abstract class that represents the shared data between the XmlParseMaster and the IXmlParseHelpers.
		* @see RTTI
		* @see XmlParseMaster
		* @see IXmlParseHelpers
		*/
		class SharedData : public RTTI
		{
			RTTI_DECLARATIONS(SharedData, RTTI);

		public:

			/** SharedData Constructor.
			* Creates and initializes a SharedData instance, and hooks it to the passed XmlParseMaster.
			* @param xmlParseMaster: XmlParseMaster to hook the instance to. Defaulted to nullptr.
			*/
			SharedData(XmlParseMaster* xmlParseMaster = nullptr);

			/** SharedData Destructor.
			*/
			virtual ~SharedData() = default;

			SharedData(const SharedData & rhs) = delete;
			SharedData& operator=(const SharedData& rhs) = delete;

			/** SharedData clone method.
			* Pure virtual method that serves as a virtual constructor. It returns a copy of the current instance.
			* @return Pointer to the copy instance.
			*/
			virtual SharedData* Clone() = 0;

			/** Initializes the SharedData to its default state.
			* Its purpose is to reset the SharedData after each parse.
			*/
			virtual void Initialize();

			/** Sets the XmlParseMaster associated to this SharedData.
			* @param xmlParseMaster: XmlParseMaster to set.
			* @see XmlParseMaster
			*/
			void SetXmlParseMaster(XmlParseMaster* xmlParseMaster);

			/** Gets the associated XmlParseMaster to the SharedData.
			* @return Associated XmlParseMaster.
			* @see XmlParseMaster
			*/
			XmlParseMaster* GetXmlParseMaster() const;

			/** Increments the depth in the SharedData.
			*/
			void IncrementDepth();

			/** Decrements the depth in the SharedData.
			*/
			void DecrementDepth();

			/** Gets the depth in the SharedData.
			* @return Depth .
			*/
			std::uint32_t Depth() const;

		protected:

			XmlParseMaster* mXmlParseMaster;
			std::uint32_t mDepth;

		};

		/** XmlParseMaster Constructor.
		* Creates and initializes a XmlParseMaster instance, and hooks it to the passed SharedData.
		* @param sharedData: SharedData to hook the instance to. Defaulted to nullptr.
		* @see SharedData
		*/
		XmlParseMaster(SharedData* sharedData = nullptr);

		/** XmlParseMaster Destructor.
		* It does not delete the SharedData or any of the helpers, unless it's a cloned instance.
		*/
		~XmlParseMaster();

		XmlParseMaster(const XmlParseMaster & rhs) = delete;
		XmlParseMaster& operator=(const XmlParseMaster& rhs) = delete;

		/** XmlParseMaster clone method.
		* It serves as a virtual constructor. It returns a copy of the current instance.
		* It marks the instance as cloned.
		* @return Pointer to the copy instance.
		*/
		XmlParseMaster* Clone();
		
		/** Adds a helper to the list of helpers.
		* @exception An exception is thrown if the instance is a clone.
		* @param helper: Helper to add.
		* @see IXmlParseHelper
		*/
		void AddHelper(IXmlParseHelper& helper);

		/** Removes a helper from the list of helpers.
		* @exception An exception is thrown if the instance is a clone.
		* @param helper: Helper to remove.
		* @see IXmlParseHelper
		*/
		void RemoveHelper(IXmlParseHelper& helper);

		/** Parses an Xml or a chunk of it.
		* This is a thin wrapper of Expat's XML_Parse.
		* @param buffer: The Xml chunk to parse.
		* @param bufferSize: The Xml chunk size.
		* @param lastChunk: Bool expressing if the passed chunk is the last chunk to parse from the current Xml. Defaulted to true.
		* @return Parsing result code.
		*/
		int Parse(const char* buffer, const std::uint32_t bufferSize, const bool lastChunk = true);

		/** Parses an Xml file.
		* @param fileName: The name of the Xml file to parse.
		* @return Parsing result code.
		*/
		int ParseFromFile(const std::string& fileName);

		/** Gets the name of the file lastly or currently being parsed.
		* @return Name of the xml file.
		*/
		std::string GetFileName() const;

		/** Gets the associated SharedData to the XmlParseMaster.
		* @return Associated SharedData.
		* @see SharedData
		*/
		SharedData* GetSharedData() const;

		/** Sets the SharedData associated to this XmlParseMaster.
		* @param sharedData: SharedData to set.
		* @see SharedData
		*/
		void SetSharedData(SharedData* sharedData);

	private:

		/** Callback that handles the start elements.
		* It's a callback for Expat.
		* @exception An exception will be thrown if there are no helpers available.
		* @param userData: Data the parser user provides. Generally a pointer to the current instance.
		* @param name: Name of the current element.
		* @param atts: Attributes in the current start element.
		*/
		static void StartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts);

		/** Callback that handles the end elements.
		* It's a callback for Expat.
		* @exception An exception will be thrown if there are no helpers available.
		* @param userData: Data the parser user provides. Generally a pointer to the current instance.
		* @param name: Name of the current element.
		*/
		static void EndElementHandler(void *userData, const XML_Char *name);

		/** Callback that handles the content of elements.
		* It's a callback for Expat.
		* @exception An exception will be thrown if there are no helpers available.
		* @param userData: Data the parser user provides. Generally a pointer to the current instance.
		* @param s: Content of the current element.
		* @param len: Size of the content of the current element.
		*/
		static void CharDataHandler(void *userData, const XML_Char *s, int len);

		void ParserReset();

		SharedData* mSharedData;
		Vector<IXmlParseHelper*> mHelpers;
		std::string mCachedFileName;
		XML_Parser mParser;
		bool mIsCloned;
	};
}