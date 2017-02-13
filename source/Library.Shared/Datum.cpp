#include "pch.h"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/string_cast.hpp"
#include "RTTI.h"
#include "Datum.h"

namespace Library
{

#pragma region Ctors and dtor

	/************************************************************************/
	Datum::Datum() :
		mCurrentType(DatumType::Unknown), mSize(0), mCapacity(0), mExternalStorage(false)
	{
		mData.theVoid = nullptr;
	}

	/************************************************************************/
	Datum::~Datum()
	{
		if (!mExternalStorage)
		{
			Clear();
		}
		mData.theVoid = nullptr;
		mSize = 0;
		mCapacity = 0;
		mExternalStorage = false;
	}

	/************************************************************************/
	Datum::Datum(const Datum & rhs) :
		Datum()
	{
		mCurrentType = rhs.mCurrentType;
		Reserve(rhs.mCapacity);
		DeepCopy(rhs);
	}

#pragma endregion

#pragma region Operator=

	/************************************************************************/
	Datum & Datum::operator=(const Datum & rhs)
	{
		if (this != &rhs)
		{
			if (!mExternalStorage)
			{
				Empty();
			}
			mCurrentType = rhs.mCurrentType;

			if (rhs.mExternalStorage)
			{
				mExternalStorage = true;
				mData = rhs.mData;
			}
			else
			{
				mExternalStorage = false;
				Reserve(rhs.mCapacity);
				DeepCopy(rhs);
			}
		}

		return *this;
	}

	/************************************************************************/
	Datum& Datum::operator=(const std::int32_t & rhs)
	{
		Set(rhs);
		return *this;
	}

	/************************************************************************/
	Datum& Datum::operator=(const std::float_t & rhs)
	{
		Set(rhs);
		return *this;
	}

	/************************************************************************/
	Datum& Datum::operator=(const glm::vec4 & rhs)
	{
		Set(rhs);
		return *this;
	}

	/************************************************************************/
	Datum& Datum::operator=(const glm::mat4x4 & rhs)
	{
		Set(rhs);
		return *this;
	}

	/************************************************************************/
	Datum& Datum::operator=(const std::string & rhs)
	{
		Set(rhs);
		return *this;
	}

	/************************************************************************/
	Datum& Datum::operator=(RTTI *const& rhs)
	{
		Set(rhs);
		return *this;
	}

#pragma endregion

#pragma region Opertaor== and operator!=

	/************************************************************************/
	bool Datum::operator==(const Datum & rhs) const
	{
		if (mCurrentType == DatumType::Unknown || rhs.mCurrentType == DatumType::Unknown)
		{
			throw std::exception("Set the Datum type first!");
		}

		bool result = mCurrentType == rhs.mCurrentType && mSize == rhs.mSize;
		if (!result)
		{
			return result;
		}

		switch (mCurrentType)
		{
			case DatumType::Integer:
				for (std::uint32_t i = 0; i < mSize; i++)
				{
					result = mData.integer[i] == rhs.mData.integer[i];
					if (!result)
					{
						break;
					}
				}
				break;

			case DatumType::Float:
				for (std::uint32_t i = 0; i < mSize; i++)
				{
					result = mData.floatt[i] == rhs.mData.floatt[i];
					if (!result)
					{
						break;
					}
				}
				break;

			case DatumType::Vector:
				for (std::uint32_t i = 0; i < mSize; i++)
				{
					result = mData.vect4[i] == rhs.mData.vect4[i];
					if (!result)
					{
						break;
					}
				}
				break;

			case DatumType::Matrix:
				for (std::uint32_t i = 0; i < mSize; i++)
				{
					result = mData.matrix4x4[i] == rhs.mData.matrix4x4[i];
					if (!result)
					{
						break;
					}
				}
				break;

			case DatumType::String:
				for (std::uint32_t i = 0; i < mSize; i++)
				{
					result = mData.string[i] == rhs.mData.string[i];
					if (!result)
					{
						break;
					}
				}
				break;

			case DatumType::Pointer:
				for (std::uint32_t i = 0; i < mSize; i++)
				{
					result = mData.rtti[i] == rhs.mData.rtti[i];
					if (!result)
					{
						break;
					}
				}
				break;
		}

		return result;
	}

	/************************************************************************/
	bool Datum::operator==(const std::int32_t & rhs) const
	{
		if (mCurrentType != DatumType::Integer)
		{
			throw std::exception("Invalid data type!");
		}

		if (IsEmpty())
		{
			throw std::exception("The Datum is empty!");
		}

		return mData.integer[0] == rhs;
	}

	/************************************************************************/
	bool Datum::operator==(const std::float_t & rhs) const
	{
		if (mCurrentType != DatumType::Float)
		{
			throw std::exception("Invalid data type!");
		}

		if (IsEmpty())
		{
			throw std::exception("The Datum is empty!");
		}

		return mData.floatt[0] == rhs;
	}

	/************************************************************************/
	bool Datum::operator==(const glm::vec4 & rhs) const
	{
		if (mCurrentType != DatumType::Vector)
		{
			throw std::exception("Invalid data type!");
		}

		if (IsEmpty())
		{
			throw std::exception("The Datum is empty!");
		}

		return mData.vect4[0] == rhs;
	}

	/************************************************************************/
	bool Datum::operator==(const glm::mat4x4 & rhs) const
	{
		if (mCurrentType != DatumType::Matrix)
		{
			throw std::exception("Invalid data type!");
		}

		if (IsEmpty())
		{
			throw std::exception("The Datum is empty!");
		}

		return mData.matrix4x4[0] == rhs;
	}

	/************************************************************************/
	bool Datum::operator==(const std::string & rhs) const
	{
		if (mCurrentType != DatumType::String)
		{
			throw std::exception("Invalid data type!");
		}

		if (IsEmpty())
		{
			throw std::exception("The Datum is empty!");
		}

		return mData.string[0] == rhs;
	}

	/************************************************************************/
	bool Datum::operator==(const RTTI *const& rhs) const
	{
		if (mCurrentType != DatumType::Pointer)
		{
			throw std::exception("Invalid data type!");
		}

		if (IsEmpty())
		{
			throw std::exception("The Datum is empty!");
		}

		return mData.rtti[0] == rhs;
	}

	/************************************************************************/
	bool Datum::operator!=(const Datum & rhs) const
	{
		return !(operator==(rhs));
	}

	/************************************************************************/
	bool Datum::operator!=(const std::int32_t & rhs) const
	{
		return !(operator==(rhs));
	}

	/************************************************************************/
	bool Datum::operator!=(const std::float_t & rhs) const
	{
		return !(operator==(rhs));
	}

	/************************************************************************/
	bool Datum::operator!=(const glm::vec4 & rhs) const
	{
		return !(operator==(rhs));
	}

	/************************************************************************/
	bool Datum::operator!=(const glm::mat4x4 & rhs) const
	{
		return !(operator==(rhs));
	}

	/************************************************************************/
	bool Datum::operator!=(const std::string & rhs) const
	{
		return !(operator==(rhs));
	}

	/************************************************************************/
	bool Datum::operator!=(const RTTI *const& rhs) const
	{
		return !(operator==(rhs));
	}

#pragma endregion

#pragma region Type and size

	/************************************************************************/
	inline Datum::DatumType Datum::Type() const
	{
		return mCurrentType;
	}

	/************************************************************************/
	void Datum::SetType(const DatumType & type)
	{
		if (mCurrentType != DatumType::Unknown)
		{
			throw std::exception("Invalid data type change!");
		}

		mCurrentType = type;
	}

	/************************************************************************/
	inline std::uint32_t Datum::Size() const
	{
		return mSize;
	}

	/************************************************************************/
	void Datum::SetSize(const std::uint32_t size)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			throw std::exception("Set the Datum type first!");
		}

		if (mExternalStorage)
		{
			throw std::exception("This datum has external storage. Invalid operation.");
		}

		if (size > mCapacity)
		{
			Reserve(size);
		}
		if (size > mSize)
		{
			std::uint32_t diff = size - mSize;

			switch (mCurrentType)
			{
				case DatumType::Integer:
					for (uint32_t i = 0; i < diff; ++i)
					{
						PushBack(std::int32_t());
					}
					break;

				case DatumType::Float:
					for (uint32_t i = 0; i < diff; ++i)
					{
						PushBack(std::float_t());
					}
					break;

				case DatumType::Vector:
					for (uint32_t i = 0; i < diff; ++i)
					{
						PushBack(glm::vec4());
					}
					break;

				case DatumType::Matrix:
					for (uint32_t i = 0; i < diff; ++i)
					{
						PushBack(glm::mat4x4());
					}
					break;

				case DatumType::String:
					for (uint32_t i = 0; i < diff; ++i)
					{
						PushBack(std::string());
					}
					break;

				case DatumType::Pointer:
					for (uint32_t i = 0; i < diff; ++i)
					{
						auto r = new RTTI*();
						PushBack(*r);
					}
					break;
			}
		}
		else
		{
			std::uint32_t diff = mSize - size;

			for (uint32_t i = 0; i < diff; i++)
			{
				PopBack();
			}
		}
	}

#pragma endregion

	/************************************************************************/
	void Datum::Clear()
	{
		if (mExternalStorage)
		{
			throw std::exception("This datum has external storage. Cannot free external storage.");
		}
		Empty();
		free(mData.theVoid);
		mCapacity = 0;
	}

	/************************************************************************/
	void Datum::PopBack()
	{
		if (!IsEmpty())
		{
			switch (mCurrentType)
			{
				case DatumType::Integer:
					mData.integer[--mSize].std::int32_t::~int32_t();
					break;

				case DatumType::Float:
					mData.floatt[--mSize].std::float_t::~float_t();
					break;

				case DatumType::Vector:
					mData.vect4[--mSize].glm::vec4::~vec4();
					break;

				case DatumType::Matrix:
					mData.matrix4x4[--mSize].glm::mat4x4::~mat4x4();
					break;

				case DatumType::String:
					mData.string[--mSize].std::string::~string();
					break;

				case DatumType::Pointer:
					mData.rtti[--mSize]->~RTTI();
					break;
			}
		}
	}

#pragma region Set Storage

	/************************************************************************/
	void Datum::SetStorage(std::int32_t *& table, const std::uint32_t tableSize)
	{
		if (!mExternalStorage)
		{
			Clear();
		}

		mExternalStorage = true;
		mData.integer = table;
		mSize = tableSize;
		mCapacity = tableSize;
	}

	/************************************************************************/
	void Datum::SetStorage(std::float_t *& table, const std::uint32_t tableSize)
	{
		if (!mExternalStorage)
		{
			Clear();
		}

		mExternalStorage = true;
		mData.floatt = table;
		mSize = tableSize;
		mCapacity = tableSize;
	}

	/************************************************************************/
	void Datum::SetStorage(glm::vec4 *& table, const std::uint32_t tableSize)
	{
		if (!mExternalStorage)
		{
			Clear();
		}

		mExternalStorage = true;
		mData.vect4 = table;
		mSize = tableSize;
		mCapacity = tableSize;
	}

	/************************************************************************/
	void Datum::SetStorage(glm::mat4x4 *& table, const std::uint32_t tableSize)
	{
		if (!mExternalStorage)
		{
			Clear();
		}

		mExternalStorage = true;
		mData.matrix4x4 = table;
		mSize = tableSize;
		mCapacity = tableSize;
	}

	/************************************************************************/
	void Datum::SetStorage(std::string *& table, const std::uint32_t tableSize)
	{
		if (!mExternalStorage)
		{
			Clear();
		}

		mExternalStorage = true;
		mData.string = table;
		mSize = tableSize;
		mCapacity = tableSize;
	}

	/************************************************************************/
	void Datum::SetStorage(RTTI **& table, const std::uint32_t tableSize)
	{
		if (!mExternalStorage)
		{
			Clear();
		}

		mExternalStorage = true;
		mData.rtti = table;
		mSize = tableSize;
		mCapacity = tableSize;
	}

#pragma endregion

#pragma region Set

	/************************************************************************/
	void Datum::Set(const std::int32_t & rhs, const std::uint32_t index)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::Integer;
		}

		if (mCurrentType != DatumType::Integer)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (IsEmpty())
		{
			PushBack(rhs);
			return;
		}

		if (index < mSize)
		{
			mData.integer[index] = rhs;
		}
		else
		{
			if (index == mSize)
			{
				PushBack(rhs);
			}
			else
			{
				throw std::exception("Invalid index");
			}
		}
	}

	/************************************************************************/
	void Datum::Set(const std::float_t & rhs, const std::uint32_t index)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::Float;
		}

		if (mCurrentType != DatumType::Float)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (IsEmpty())
		{
			PushBack(rhs);
			return;
		}

		if (index < mSize)
		{
			mData.floatt[index] = rhs;
		}
		else
		{
			if (index == mSize)
			{
				PushBack(rhs);
			}
			else
			{
				throw std::exception("Invalid index");
			}
		}
	}

	/************************************************************************/
	void Datum::Set(const glm::vec4 & rhs, const std::uint32_t index)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::Vector;
		}

		if (mCurrentType != DatumType::Vector)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (IsEmpty())
		{
			PushBack(rhs);
			return;
		}

		if (index < mSize)
		{
			mData.vect4[index] = rhs;
		}
		else
		{
			if (index == mSize)
			{
				PushBack(rhs);
			}
			else
			{
				throw std::exception("Invalid index");
			}
		}
	}

	/************************************************************************/
	void Datum::Set(const glm::mat4x4 & rhs, const std::uint32_t index)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::Matrix;
		}

		if (mCurrentType != DatumType::Matrix)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (IsEmpty())
		{
			PushBack(rhs);
			return;
		}

		if (index < mSize)
		{
			mData.matrix4x4[index] = rhs;
		}
		else
		{
			if (index == mSize)
			{
				PushBack(rhs);
			}
			else
			{
				throw std::exception("Invalid index");
			}
		}
	}

	/************************************************************************/
	void Datum::Set(const std::string & rhs, const std::uint32_t index)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::String;
		}

		if (mCurrentType != DatumType::String)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (IsEmpty())
		{
			PushBack(rhs);
			return;
		}

		if (index < mSize)
		{
			mData.string[index] = rhs;
		}
		else
		{
			if (index == mSize)
			{
				PushBack(rhs);
			}
			else
			{
				throw std::exception("Invalid index");
			}
		}
	}

	/************************************************************************/
	void Datum::Set(RTTI* const& rhs, const std::uint32_t index)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::Pointer;
		}

		if (mCurrentType != DatumType::Pointer)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (IsEmpty())
		{
			PushBack(rhs);
			return;
		}

		if (index < mSize)
		{
			mData.rtti[index] = rhs;
		}
		else
		{
			if (index == mSize)
			{
				PushBack(rhs);
			}
			else
			{
				throw std::exception("Invalid index");
			}
		}
	}

#pragma endregion

	/************************************************************************/
	void Datum::SetFromString(const std::string& str, const std::uint32_t index)
	{
		int result;

		switch (mCurrentType)
		{
			case DatumType::Unknown:
				throw std::exception("Cannot set this datum. Its type should be set first.");
				break;
			
			case DatumType::Integer:
				std::int32_t integer;
				result = sscanf_s(str.c_str(), "%d", &integer);
				if (result == EOF || result != 1)
				{
					throw std::exception("Invalid input.");
				}
				Set(integer, index);
				break;
			
			case DatumType::Float:
				std::float_t f;
				result = sscanf_s(str.c_str(), "%f", &f);
				if (result == EOF || result != 1)
				{
					throw std::exception("Invalid input.");
				}
				Set(f, index);
				break;
			
			case DatumType::Vector:
				glm::vec4 vect;
				std::float_t f1, f2, f3, f4;
				result = sscanf_s(str.c_str(), "(%f, %f, %f, %f)", &f1, &f2, &f3, &f4);

				if (result == EOF || result < 4)
				{
					result = sscanf_s(str.c_str(), "(%f)", &f1);
					if (result == EOF || result != 1)
					{
						throw std::exception("Invalid input.");
					}
					vect = glm::vec4(f1);
				}
				else
				{
					vect = glm::vec4(f1, f2, f3, f4);
				}

				Set(vect, index);
				break;
			
			case DatumType::Matrix:
				glm::mat4x4 mat;
				std::float_t f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16;
				result = sscanf_s(str.c_str(), "(%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f)",
								  &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13, &f14, &f15, &f16);

				if (result == EOF || result < 16)
				{
					result = sscanf_s(str.c_str(), "(%f)", &f1);
					if (result == EOF || result != 1)
					{
						throw std::exception("Invalid input.");
					}
					mat = glm::mat4x4(f1);
				}
				else
				{
					mat = glm::mat4x4(f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12, f13, f14, f15, f16);
				}

				Set(mat, index);
				break;

			case DatumType::String:
				Set(str, index);
				break;

			case DatumType::Pointer:
				RTTI* rtti;
				result = sscanf_s(str.c_str(), "%d", &rtti);
				if (result == EOF || result != 1)
				{
					throw std::exception("Invalid input.");
				}
				Set(rtti, index);
				break;
		}
	}

	/************************************************************************/
	std::string Datum::ToString(const std::uint32_t index)
	{
		std::string str;

		switch (mCurrentType)
		{
			case DatumType::Integer:
				str = std::to_string(Get<std::int32_t>(index));
				break;

			case DatumType::Float:
				str = std::to_string(Get<std::float_t>(index));
				break;

			case DatumType::Vector:
				str = glm::to_string(Get<glm::vec4>(index));
				break;

			case DatumType::Matrix:
				str = glm::to_string(Get<glm::mat4x4>(index));
				break;

			case DatumType::String:
				str = Get<std::string>(index);
				break;

			case DatumType::Pointer:
				str = Get<RTTI*>(index)->ToString();
				break;
		}

		return str;
	}

	/************************************************************************/
	bool Datum::Reserve(const std::uint32_t newCapacity)
	{
		if (mExternalStorage)
		{
			throw std::exception("This datum has external storage. Cannot reserve memory for it.");
		}

		if (newCapacity < mCapacity)
		{
			return false;
		}
		else
		{
			mData.theVoid = realloc(mData.theVoid, newCapacity * sizeof(DataValues));
			mCapacity = newCapacity;
			return true;
		}
	}

#pragma region PushBack

	/************************************************************************/
	void Datum::PushBack(const std::int32_t & rhs)
	{
		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		new(mData.integer + mSize)std::uint32_t(rhs);
		++mSize;
	}

	/************************************************************************/
	void Datum::PushBack(const std::float_t & rhs)
	{
		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		new(mData.floatt + mSize)std::float_t(rhs);
		++mSize;
	}

	/************************************************************************/
	void Datum::PushBack(const glm::vec4 & rhs)
	{
		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		new(mData.vect4 + mSize)glm::vec4(rhs);
		++mSize;
	}

	/************************************************************************/
	void Datum::PushBack(const glm::mat4x4 & rhs)
	{
		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		new(mData.matrix4x4 + mSize)glm::mat4x4(rhs);
		++mSize;
	}

	/************************************************************************/
	void Datum::PushBack(const std::string & rhs)
	{
		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		new(mData.string + mSize)std::string(rhs);
		++mSize;
	}

	/************************************************************************/
	void Datum::PushBack(RTTI * const & rhs)
	{
		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		new(mData.rtti + mSize)RTTI*(rhs);
		++mSize;
	}

#pragma endregion

#pragma region Get

	/************************************************************************/
	template<>
	const std::int32_t& Datum::Get<std::int32_t>(const std::uint32_t index) const
	{
		if (index > mSize)
		{
			throw std::exception("Going out of bounds!");
		}

		return mData.integer[index];
	}

	/************************************************************************/
	template<>
	std::int32_t& Datum::Get<std::int32_t>(const std::uint32_t index)
	{
		if (index > mSize)
		{
			throw std::exception("Going out of bounds!");
		}

		return mData.integer[index];
	}

	/************************************************************************/
	template<>
	const std::float_t& Datum::Get<std::float_t>(const std::uint32_t index) const
	{
		if (index > mSize)
		{
			throw std::exception("Going out of bounds!");
		}

		return mData.floatt[index];
	}

	/************************************************************************/
	template<>
	std::float_t& Datum::Get<std::float_t>(const std::uint32_t index)
	{
		if (index > mSize)
		{
			throw std::exception("Going out of bounds!");
		}

		return mData.floatt[index];
	}

	/************************************************************************/
	template<>
	const glm::vec4& Datum::Get<glm::vec4>(const std::uint32_t index) const
	{
		if (index > mSize)
		{
			throw std::exception("Going out of bounds!");
		}

		return mData.vect4[index];
	}

	/************************************************************************/
	template<>
	glm::vec4& Datum::Get(const std::uint32_t index)
	{
		if (index > mSize)
		{
			throw std::exception("Going out of bounds!");
		}

		return mData.vect4[index];
	}

	/************************************************************************/
	template<>
	const glm::mat4x4& Datum::Get<glm::mat4x4>(const std::uint32_t index) const
	{
		if (index > mSize)
		{
			throw std::exception("Going out of bounds!");
		}

		return mData.matrix4x4[index];
	}

	/************************************************************************/
	template<>
	glm::mat4x4& Datum::Get<glm::mat4x4>(const std::uint32_t index)
	{
		if (index > mSize)
		{
			throw std::exception("Going out of bounds!");
		}

		return mData.matrix4x4[index];
	}

	/************************************************************************/
	template<>
	const std::string& Datum::Get<std::string>(const std::uint32_t index) const
	{
		if (index > mSize)
		{
			throw std::exception("Going out of bounds!");
		}

		return mData.string[index];
	}

	/************************************************************************/
	template<>
	std::string& Datum::Get<std::string>(const std::uint32_t index)
	{
		if (index > mSize)
		{
			throw std::exception("Going out of bounds!");
		}

		return mData.string[index];
	}

	/************************************************************************/
	template<>
	RTTI* const & Datum::Get<RTTI*>(const std::uint32_t index) const
	{
		if (index > mSize)
		{
			throw std::exception("Going out of bounds!");
		}

		return mData.rtti[index];
	}

	/************************************************************************/
	template<>
	RTTI*& Datum::Get<RTTI*>(const std::uint32_t index)
	{
		if (index > mSize)
		{
			throw std::exception("Going out of bounds!");
		}

		return mData.rtti[index];
	}

#pragma endregion

	/************************************************************************/
	inline bool Datum::IsEmpty() const
	{
		return mSize == 0;
	}

	/************************************************************************/
	void Datum::Empty()
	{
		while (!IsEmpty())
		{
			PopBack();
		}
	}

	/************************************************************************/
	void Datum::DeepCopy(const Datum & rhs)
	{
		switch (rhs.mCurrentType)
		{
			case DatumType::Integer:
				for (std::uint32_t i = 0; i < rhs.mSize; i++)
				{
					PushBack(rhs.mData.integer[i]);
				}
				break;

			case DatumType::Float:
				for (std::uint32_t i = 0; i < rhs.mSize; i++)
				{
					PushBack(rhs.mData.floatt[i]);
				}
				break;

			case DatumType::Vector:
				for (std::uint32_t i = 0; i < rhs.mSize; i++)
				{
					PushBack(rhs.mData.vect4[i]);
				}
				break;

			case DatumType::Matrix:
				for (std::uint32_t i = 0; i < rhs.mSize; i++)
				{
					PushBack(rhs.mData.matrix4x4[i]);
				}
				break;

			case DatumType::String:
				for (std::uint32_t i = 0; i < rhs.mSize; i++)
				{
					PushBack(rhs.mData.string[i]);
				}
				break;

			case DatumType::Pointer:
				for (std::uint32_t i = 0; i < rhs.mSize; i++)
				{
					PushBack(rhs.mData.rtti[i]);
				}
				break;
		}
	}
}
