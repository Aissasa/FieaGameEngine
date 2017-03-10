#include "pch.h"
#include "Datum.h"

namespace Library
{

#pragma region Ctors and dtor

	/************************************************************************/
	Datum::Datum(DatumType type) :
		mCurrentType(type), mSize(0), mCapacity(0), mExternalStorage(false)
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
		else
		{
			mSize = 0;
			mCapacity = 0;
			mExternalStorage = false;
		}
	}

	/************************************************************************/
	Datum::Datum(const Datum & rhs) :
		Datum()
	{
		mCurrentType = rhs.mCurrentType;
		if (mCurrentType != DatumType::Unknown)
		{
			Reserve(rhs.mCapacity);
			DeepCopy(rhs);
		}
	}

#pragma endregion

#pragma region Operator=

	/************************************************************************/
	Datum& Datum::operator=(const Datum & rhs)
	{
		if (this != &rhs)
		{
			if (!mExternalStorage)
			{
				Clear();
			}
			else
			{
				mSize = 0;
				mCapacity = 0;
				mData.theVoid = nullptr;
			}

			mCurrentType = rhs.mCurrentType;
			mExternalStorage = rhs.mExternalStorage;

			if (rhs.mExternalStorage)
			{
				mData = rhs.mData;
				mSize = rhs.mSize;
				mCapacity = rhs.mCapacity;
			}
			else
			{
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

	/************************************************************************/
	Datum & Datum::operator=(Scope * const & rhs)
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
					// urgent change this
					// result = mData.rtti[i]->Equals(rhs.mData.rtti[i]);
					result = mData.rtti[i] == rhs.mData.rtti[i];
					if (!result)
					{
						break;
					}
				}
				break;

			case DatumType::Table:
				for (std::uint32_t i = 0; i < mSize; i++)
				{
					result = mData.scope[i] == rhs.mData.scope[i];
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
	bool Datum::operator==(const Scope * const & rhs) const
	{
		if (mCurrentType != DatumType::Table)
		{
			throw std::exception("Invalid data type!");
		}

		if (IsEmpty())
		{
			throw std::exception("The Datum is empty!");
		}

		return mData.scope[0] == rhs;

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

	/************************************************************************/
	bool Datum::operator!=(const Scope * const & rhs) const
	{
		return !(operator==(rhs));
	}

#pragma endregion

#pragma region Special Operator[]

	/************************************************************************/
	Scope& Datum::operator[](std::uint32_t index)
	{
		return *Get<Scope*>(index);
	}

#pragma endregion

#pragma region Type and size

	/************************************************************************/
	Datum::DatumType Datum::Type() const
	{
		return mCurrentType;
	}

	/************************************************************************/
	void Datum::SetType(const DatumType & type)
	{
		if (mCurrentType != DatumType::Unknown || type == DatumType::Unknown)
		{
			throw std::exception("Invalid data type change!");
		}

		mCurrentType = type;
	}

	/************************************************************************/
	std::uint32_t Datum::Size() const
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
						RTTI* r = nullptr;
						PushBack(r);
					}
					break;

				case DatumType::Table:
					for (uint32_t i = 0; i < diff; ++i)
					{
						auto r = new Scope*();
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

#pragma region Storage Clear

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
		mData.theVoid = nullptr;
	}

	/************************************************************************/
	void Datum::PopBack()
	{
		if (mExternalStorage)
		{
			throw std::exception("This datum has external storage. Cannot free external storage.");
		}

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
					--mSize;
					break;

				case DatumType::Table:
					--mSize;
					break;
			}
		}
	}

	/************************************************************************/
	void Datum::Remove(std::uint32_t index, bool deleteIt)
	{
		if (mExternalStorage)
		{
			throw std::exception("This datum has external storage. Cannot free external storage.");
		}

		if (index >= mSize)
		{
			throw std::out_of_range("Going out of range!");
		}

		std::uint32_t size = 0;

		if (!IsEmpty())
		{
			switch (mCurrentType)
			{
				case DatumType::Integer:
					size = (mSize - index - 1) * sizeof(*mData.integer);
					if (size > 0)
					{
						memmove(&mData.integer[index], &mData.integer[index + 1], size);
					}
					--mSize;
					break;

				case DatumType::Float:
					size = (mSize - index - 1) * sizeof(*mData.floatt);
					if (size > 0)
					{
						memmove(&mData.floatt[index], &mData.floatt[index + 1], size);
					}
					--mSize;
					break;

				case DatumType::Vector:
					if (deleteIt)
					{
						mData.vect4[index].glm::vec4::~vec4();
					}
					size = (mSize - index - 1) * sizeof(*mData.vect4);
					if (size > 0)
					{
						memmove(&mData.vect4[index], &mData.vect4[index + 1], size);
					}
					--mSize;

					break;

				case DatumType::Matrix:
					if (deleteIt)
					{
						mData.matrix4x4[index].glm::mat4x4::~mat4x4();
					}
					size = (mSize - index - 1) * sizeof(*mData.matrix4x4);
					if (size > 0)
					{
						memmove(&mData.matrix4x4[index], &mData.matrix4x4[index + 1], size);
					}
					--mSize;

					break;

				case DatumType::String:
					if (deleteIt)
					{
						mData.string[index].std::string::~string();
					}
					size = (mSize - index - 1) * sizeof(*mData.string);
					if (size > 0)
					{
						memmove(&mData.string[index], &mData.string[index + 1], size);
					}
					--mSize;

					break;

				case DatumType::Pointer:
					if (deleteIt)
					{
						mData.rtti[index]->~RTTI();
					}
					size = (mSize - index - 1) * sizeof(*mData.rtti);
					if (size > 0)
					{
						memmove(&mData.rtti[index], &mData.rtti[index + 1], size);
					}
					--mSize;

					break;

				case DatumType::Table:
					// cannot call dtor from here
					size = (mSize - index - 1) * sizeof(*mData.scope);
					if (size > 0)
					{
						memmove(&mData.scope[index], &mData.scope[index + 1], size);
					}
 					else
 					{
 						mData.scope[index] = nullptr;
 					}
					--mSize;
					break;
			}
		}
	}

#pragma endregion

#pragma region Set Storage

	/************************************************************************/
	void Datum::SetStorage(std::int32_t *& table, const std::uint32_t tableSize)
	{
		if (!mExternalStorage)
		{
			Clear();
		}

		mExternalStorage = true;
		mCurrentType = DatumType::Integer;
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
		mCurrentType = DatumType::Float;
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
		mCurrentType = DatumType::Vector;
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
		mCurrentType = DatumType::Matrix;
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
		mCurrentType = DatumType::String;
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
		mCurrentType = DatumType::Pointer;
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

	/************************************************************************/
	void Datum::Set(Scope * const & rhs, const std::uint32_t index)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::Table;
		}

		if (mCurrentType != DatumType::Table)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (index < mSize)
		{
			mData.scope[index] = rhs;
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

#pragma region String related methods

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
			{
				glm::vec4 vect(0);
				std::float_t fv1, fv2, fv3, fv4;
				result = sscanf_s(str.c_str(), "(%f, %f, %f, %f)", &fv1, &fv2, &fv3, &fv4);

				if (result == EOF || result < 4)
				{
					result = sscanf_s(str.c_str(), "(%f)", &fv1);
					if (result == EOF || result != 1)
					{
						throw std::exception("Invalid input.");
					}
					vect = glm::vec4(fv1);
				}
				else
				{
					vect = glm::vec4(fv1, fv2, fv3, fv4);
				}

				Set(vect, index);
				break;
			}

			case DatumType::Matrix:
			{
				glm::mat4x4 mat(0);
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
			}
			case DatumType::String:
				Set(str, index);
				break;

			case DatumType::Pointer:
				throw std::exception("Cannot set a value of this datum from a string.");
				break;

			case DatumType::Table:
				throw std::exception("Cannot set a value of this datum from a string.");
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

			case DatumType::Table:
				str = Get<RTTI*>(index)->ToString();
				break;
		}

		return str;
	}

#pragma endregion

#pragma region PushBack

	/************************************************************************/
	void Datum::PushBack(const std::int32_t & rhs)
	{

		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::Integer;
		}

		if (mCurrentType != DatumType::Integer)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		new(mData.integer + mSize++)std::uint32_t(rhs);
	}

	/************************************************************************/
	void Datum::PushBack(const std::float_t & rhs)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::Float;
		}

		if (mCurrentType != DatumType::Float)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		new(mData.floatt + mSize++)std::float_t(rhs);
	}

	/************************************************************************/
	void Datum::PushBack(const glm::vec4 & rhs)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::Vector;
		}

		if (mCurrentType != DatumType::Vector)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		new(mData.vect4 + mSize++)glm::vec4(rhs);
	}

	/************************************************************************/
	void Datum::PushBack(const glm::mat4x4 & rhs)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::Matrix;
		}

		if (mCurrentType != DatumType::Matrix)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		new(mData.matrix4x4 + mSize++)glm::mat4x4(rhs);
	}

	/************************************************************************/
	void Datum::PushBack(const std::string & rhs)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::String;
		}

		if (mCurrentType != DatumType::String)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		new(mData.string + mSize++)std::string(rhs);
	}

	/************************************************************************/
	void Datum::PushBack(RTTI * const rhs)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::Pointer;
		}

		if (mCurrentType != DatumType::Pointer)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		mData.rtti[mSize++] = rhs;
	}

	/************************************************************************/
	void Datum::PushBack(Scope * const & rhs)
	{
		if (mCurrentType == DatumType::Unknown)
		{
			mCurrentType = DatumType::Table;
		}

		if (mCurrentType != DatumType::Table)
		{
			throw std::exception("Invalid data type assignment!");
		}

		if (mSize == mCapacity)
		{
			Reserve(std::max(mCapacity * 2, 1u));
		}
		mData.scope[mSize++] = rhs;
	}

#pragma endregion

#pragma region Get

	/************************************************************************/
	template<>
	const std::int32_t& Datum::Get<std::int32_t>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.integer[index];
	}

	/************************************************************************/
	template<>
	std::int32_t& Datum::Get<std::int32_t>(const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.integer[index];
	}

	/************************************************************************/
	template<>
	const std::float_t& Datum::Get<std::float_t>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.floatt[index];
	}

	/************************************************************************/
	template<>
	std::float_t& Datum::Get<std::float_t>(const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.floatt[index];
	}

	/************************************************************************/
	template<>
	const glm::vec4& Datum::Get<glm::vec4>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.vect4[index];
	}

	/************************************************************************/
	template<>
	glm::vec4& Datum::Get(const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.vect4[index];
	}

	/************************************************************************/
	template<>
	const glm::mat4x4& Datum::Get<glm::mat4x4>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.matrix4x4[index];
	}

	/************************************************************************/
	template<>
	glm::mat4x4& Datum::Get<glm::mat4x4>(const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.matrix4x4[index];
	}

	/************************************************************************/
	template<>
	const std::string& Datum::Get<std::string>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.string[index];
	}

	/************************************************************************/
	template<>
	std::string& Datum::Get<std::string>(const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.string[index];
	}

	/************************************************************************/
	template<>
	RTTI* const & Datum::Get<RTTI*>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.rtti[index];
	}

	/************************************************************************/
	template<>
	RTTI*& Datum::Get<RTTI*>(const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.rtti[index];
	}

	/************************************************************************/
	template<>
	Scope* const & Datum::Get<Scope*>(const std::uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.scope[index];
	}

	/************************************************************************/
	template<>
	Scope*& Datum::Get<Scope*>(const std::uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Going out of bounds!");
		}

		return mData.scope[index];
	}


#pragma endregion

#pragma region Storage Checks

	/************************************************************************/
	std::uint32_t Datum::Capacity() const
	{
		return mCapacity;
	}

	/************************************************************************/
	bool Datum::IsEmpty() const
	{
		return mSize == 0;
	}

	/************************************************************************/
	bool Datum::HasExternalStorage() const
	{
		return mExternalStorage;
	}

#pragma endregion

#pragma region Helper Methods

	/************************************************************************/
	bool Datum::Reserve(const std::uint32_t newCapacity)
	{
		if (newCapacity == 0)
		{
			return false;
		}
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
			switch (mCurrentType)
			{
				case DatumType::Unknown:
					throw std::exception("The type needs to be determined before allocating memory.");
					break;

				case DatumType::Integer:
					mData.theVoid = realloc(mData.theVoid, newCapacity * sizeof(std::int32_t));
					break;

				case DatumType::Float:
					mData.theVoid = realloc(mData.theVoid, newCapacity * sizeof(std::float_t));
					break;

				case DatumType::Vector:
					mData.theVoid = realloc(mData.theVoid, newCapacity * sizeof(glm::vec4));
					break;

				case DatumType::Matrix:
					mData.theVoid = realloc(mData.theVoid, newCapacity * sizeof(glm::mat4x4));
					break;

				case DatumType::String:
					mData.theVoid = realloc(mData.theVoid, newCapacity * sizeof(std::string));
					break;

				case DatumType::Pointer:
					mData.theVoid = realloc(mData.theVoid, newCapacity * sizeof(RTTI*));
					break;

				case DatumType::Table:
					mData.theVoid = realloc(mData.theVoid, newCapacity * sizeof(Scope*));
					break;
			}
			mCapacity = newCapacity;
			return true;
		}
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

#pragma endregion

}
