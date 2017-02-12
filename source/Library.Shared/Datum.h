#pragma once

namespace Library
{
	class Datum
	{
	private:

		union DataValues
		{
			void* theVoid;
			std::int32_t* integer;
			std::float_t* floatt;
			glm::vec4* vect4;
			glm::mat4x4* matrix4x4;
			std::string* string;
			RTTI** rtti;
		};

	public:

		enum class DatumType
		{
			Unknown,
			Integer,
			Float,
			Vector,
			Matrix,
			String,
			Pointer
		};

		Datum();
		~Datum();
		Datum(const Datum & rhs);

		Datum& operator=(const Datum& rhs);
		Datum& operator=(const std::int32_t& rhs);
		Datum& operator=(const std::float_t& rhs);
		Datum& operator=(const glm::vec4& rhs);
		Datum& operator=(const glm::mat4x4& rhs);
		Datum& operator=(const std::string& rhs);
		Datum& operator=(const RTTI*& rhs);

		DatumType Type() const;
		void SetType(const DatumType type);
		std::uint32_t Size() const;
		void SetSize(const std::uint32_t size);
		void Clear();

		void SetStorage(const std::int32_t* table, const std::uint32_t tableSize);
		void SetStorage(const std::float_t* table, const std::uint32_t tableSize);
		void SetStorage(const glm::vec4* table, const std::uint32_t tableSize);
		void SetStorage(const glm::mat4x4* table, const std::uint32_t tableSize);
		void SetStorage(const std::string* table, const std::uint32_t tableSize);
		void SetStorage(const RTTI** table, const std::uint32_t tableSize);

		bool operator==(const Datum& rhs) const;
		bool operator==(const std::int32_t& rhs) const;
		bool operator==(const std::float_t& rhs) const;
		bool operator==(const glm::vec4& rhs) const;
		bool operator==(const glm::mat4x4& rhs) const;
		bool operator==(const std::string& rhs) const;
		bool operator==(const  RTTI*& rhs) const;

		bool operator!=(const Datum& rhs) const;
		bool operator!=(const std::int32_t& rhs) const;
		bool operator!=(const std::float_t& rhs) const;
		bool operator!=(const glm::vec4& rhs) const;
		bool operator!=(const glm::mat4x4& rhs) const;
		bool operator!=(const std::string& rhs) const;
		bool operator!=(const RTTI*& rhs) const;

		// urgent think about template private method for set
		void Set(const std::int32_t& rhs, std::uint32_t index = 0);
		void Set(const std::float_t& rhs, std::uint32_t index = 0);
		void Set(const glm::vec4& rhs, std::uint32_t index = 0);
		void Set(const glm::mat4x4& rhs, std::uint32_t index = 0);
		void Set(const std::string& rhs, std::uint32_t index = 0);
		void Set(const RTTI*& rhs, std::uint32_t index = 0);

	private:
		template<typename T>
		const T& Get(std::uint32_t index = 0) const;

		template<typename T>
		T& Get(std::uint32_t index = 0);
	
	public:
		template<>
		const std::int32_t& Get(std::uint32_t index) const;
		template<>
		std::int32_t& Get(std::uint32_t index);

		template<>
		const std::float_t& Get(std::uint32_t index) const;
		template<>
		std::float_t& Get(std::uint32_t index);

		template<>
		const glm::vec4& Get(std::uint32_t index) const;
		template<>
		glm::vec4& Get(std::uint32_t index);

		template<>
		const glm::mat4x4& Get(std::uint32_t index) const;
		template<>
		glm::mat4x4& Get(std::uint32_t index);

		template<>
		const std::string& Get(std::uint32_t index) const;
		template<>
		std::string& Get(std::uint32_t index);

		template<>
		RTTI* const & Get(std::uint32_t index) const;
		template<>
		RTTI*& Get(std::uint32_t index);

		void SetFromString(std::string str, std::uint32_t index = 0);
		std::string ToString(std::uint32_t index = 0);

	private:
		DataValues mData;
		std::uint32_t mSize;
		std::uint32_t mCapacity;
		bool mExternalStorage;
	};
}