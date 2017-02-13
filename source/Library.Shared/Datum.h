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
		Datum& operator=(RTTI*const& rhs);

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


		inline DatumType Type() const;
		void SetType(const DatumType& type);
		inline std::uint32_t Size() const;
		void SetSize(const std::uint32_t size);
		void Clear();
		void PopBack();

		void SetStorage(std::int32_t*& table, const std::uint32_t tableSize);
		void SetStorage(std::float_t*& table, const std::uint32_t tableSize);
		void SetStorage(glm::vec4*& table, const std::uint32_t tableSize);
		void SetStorage(glm::mat4x4*& table, const std::uint32_t tableSize);
		void SetStorage(std::string*& table, const std::uint32_t tableSize);
		void SetStorage(RTTI**& table, const std::uint32_t tableSize);

		void Set(const std::int32_t& rhs, const std::uint32_t index = 0);
		void Set(const std::float_t& rhs, const std::uint32_t index = 0);
		void Set(const glm::vec4& rhs, const std::uint32_t index = 0);
		void Set(const glm::mat4x4& rhs, const std::uint32_t index = 0);
		void Set(const std::string& rhs, const std::uint32_t index = 0);
		void Set(RTTI*const & rhs, const std::uint32_t index = 0);

		template<typename T>
		const T& Get(std::uint32_t index = 0) const;

		template<typename T>
		T& Get(std::uint32_t index = 0);

		void SetFromString(std::string str, std::uint32_t index = 0);
		std::string ToString(std::uint32_t index = 0);

		bool Reserve(std::uint32_t newCapacity);

		void PushBack(const std::int32_t& rhs);
		void PushBack(const std::float_t& rhs);
		void PushBack(const glm::vec4& rhs);
		void PushBack(const glm::mat4x4& rhs);
		void PushBack(const std::string& rhs);
		void PushBack(RTTI *const & rhs);

		inline bool IsEmpty() const;

	private:

		// todo consider this
		//Datum(DatumType type, std::uint32_t size, std::uint32_t capacity, bool externalStorage);
		void Empty();
		void DeepCopy(const Datum& rhs);

		DataValues mData;
		DatumType mCurrentType;
		std::uint32_t mSize;
		std::uint32_t mCapacity;
		bool mExternalStorage;
	};

	template<>
	const std::int32_t& Datum::Get<std::int32_t>(std::uint32_t index) const;
	template<>
	std::int32_t& Datum::Get<std::int32_t>(std::uint32_t index);

	template<>
	const std::float_t& Datum::Get<std::float_t>(std::uint32_t index) const;
	template<>
	std::float_t& Datum::Get<std::float_t>(std::uint32_t index);

	template<>
	const glm::vec4& Datum::Get<glm::vec4>(std::uint32_t index) const;
	template<>
	glm::vec4& Datum::Get(std::uint32_t index);

	template<>
	const glm::mat4x4& Datum::Get<glm::mat4x4>(std::uint32_t index) const;
	template<>
	glm::mat4x4& Datum::Get<glm::mat4x4>(std::uint32_t index);

	template<>
	const std::string& Datum::Get<std::string>(std::uint32_t index) const;
	template<>
	std::string& Datum::Get<std::string>(std::uint32_t index);

	template<>
	RTTI* const & Datum::Get<RTTI*>(std::uint32_t index) const;
	template<>
	RTTI*& Datum::Get<RTTI*>(std::uint32_t index);

}
//#include "Datum.inl"
