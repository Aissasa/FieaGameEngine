#include "pch.h"
#include "RTTI.h"
#include "Scope.h"
#include "Datum.h"
#include "HashMap.h"
#include "Attributed.h"

using namespace std;
using namespace glm;

namespace Library
{	
	RTTI_DEFINITIONS(Attributed);

	/************************************************************************/
	Attributed::Attributed() :
		Scope()
	{
		(*this)["this"] = this;
	}

	/************************************************************************/
	Attributed::Attributed(const Attributed & rhs):
		Scope(rhs)
	{
		(*this)["this"] = this;
	}

	Attributed& Attributed::operator=(const Attributed & rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(rhs);
			(*this)["this"] = this;
		}
		return *this;
	}

	/************************************************************************/
	bool Attributed::IsAttribute(const std::string & str) const
	{
		if (str.empty())
		{
			throw invalid_argument("The string shouldn't be empty.");
		}

		return Scope::Find(str) != nullptr;
	}

	/************************************************************************/
	bool Attributed::IsPrescribedAttribute(const std::string & str) const
	{
		if (str.empty())
		{
			throw invalid_argument("The string shouldn't be empty.");
		}

		for (auto& i : sPrescribedAttributes)
		{
			// check current vector if it has the attribute
			if (i.second.Find(str) == i.second.end())
			{
				return true;
			} 
		}
		return false;
	}

	/************************************************************************/
	bool Attributed::IsAuxiliaryAttribute(const std::string & str) const
	{
		return IsAttribute(str) && !IsPrescribedAttribute(str);
	}

	/************************************************************************/
	Datum& Attributed::AppendAuxiliaryAttribute(const std::string & str)
	{
		if (str.empty())
		{
			throw invalid_argument("The string shouldn't be an empty.");
		}

		if (IsPrescribedAttribute(str))
		{
			throw exception("This string represents a prescribed attribute.");
		}

		return Append(str);
	}

	/************************************************************************/
	std::uint32_t Attributed::AuxiliaryBegin() const
	{
		uint32_t index = 1;
		for (auto& i : sPrescribedAttributes)
		{
			index += i.second.Size();
		}
		return index;
	}

	/************************************************************************/
	Datum & Attributed::AddInternalAttribute(const std::string & name, const std::int32_t initValue, const std::uint32_t size)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Datum& datum = Append(name, Datum::DatumType::Integer);
		InitializeDatum(datum, initValue, size);
		AddPrescribedAttributeToHashmap(name);
		
		return datum;
	}

	/************************************************************************/
	Datum & Attributed::AddInternalAttribute(const std::string & name, const std::float_t initValue, const std::uint32_t size)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Datum& datum = Append(name, Datum::DatumType::Float);
		InitializeDatum(datum, initValue, size);
		AddPrescribedAttributeToHashmap(name);

		return datum;
	}

	/************************************************************************/
	Datum & Attributed::AddInternalAttribute(const std::string & name, const glm::vec4 & initValue, const std::uint32_t size)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Datum& datum = Append(name, Datum::DatumType::Vector);
		InitializeDatum(datum, initValue, size);
		AddPrescribedAttributeToHashmap(name);

		return datum;
	}

	/************************************************************************/
	Datum & Attributed::AddInternalAttribute(const std::string & name, const glm::mat4x4 & initValue, const std::uint32_t size)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Datum& datum = Append(name, Datum::DatumType::Matrix);
		InitializeDatum(datum, initValue, size);
		AddPrescribedAttributeToHashmap(name);

		return datum;
	}

	/************************************************************************/
	Datum & Attributed::AddInternalAttribute(const std::string & name, const std::string & initValue, const std::uint32_t size)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Datum& datum = Append(name, Datum::DatumType::String);
		InitializeDatum(datum, initValue, size);
		AddPrescribedAttributeToHashmap(name);

		return datum;
	}

	/************************************************************************/
	Datum & Attributed::AddInternalAttribute(const std::string & name, RTTI *const& initValue, const std::uint32_t size)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Datum& datum = Append(name, Datum::DatumType::Pointer);
		InitializeDatum(datum, initValue, size);
		AddPrescribedAttributeToHashmap(name);

		return datum;
	}

	/************************************************************************/
	Datum & Attributed::AddExternalAttribute(const std::string & name, std::int32_t * value, const std::uint32_t size)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Datum& datum = Append(name, Datum::DatumType::Integer);
		datum.SetStorage(value, size);
		AddPrescribedAttributeToHashmap(name);

		return datum;
	}

	/************************************************************************/
	Datum & Attributed::AddExternalAttribute(const std::string & name, std::float_t * value, const std::uint32_t size)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Datum& datum = Append(name, Datum::DatumType::Float);
		datum.SetStorage(value, size);
		AddPrescribedAttributeToHashmap(name);

		return datum;
	}

	/************************************************************************/
	Datum & Attributed::AddExternalAttribute(const std::string & name, glm::vec4 * value, const std::uint32_t size)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Datum& datum = Append(name, Datum::DatumType::Vector);
		datum.SetStorage(value, size);
		AddPrescribedAttributeToHashmap(name);

		return datum;
	}

	/************************************************************************/
	Datum & Attributed::AddExternalAttribute(const std::string & name, glm::mat4x4 * value, const std::uint32_t size)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Datum& datum = Append(name, Datum::DatumType::Matrix);
		datum.SetStorage(value, size);
		AddPrescribedAttributeToHashmap(name);

		return datum;
	}

	/************************************************************************/
	Datum & Attributed::AddExternalAttribute(const std::string & name, std::string * value, const std::uint32_t size)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Datum& datum = Append(name, Datum::DatumType::String);
		datum.SetStorage(value, size);
		AddPrescribedAttributeToHashmap(name);

		return datum;
	}

	/************************************************************************/
	Datum & Attributed::AddExternalAttribute(const std::string & name, RTTI ** value, const std::uint32_t size)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Datum& datum = Append(name, Datum::DatumType::Pointer);
		datum.SetStorage(value, size);
		AddPrescribedAttributeToHashmap(name);

		return datum;
	}

	/************************************************************************/
	Scope & Attributed::AddNestedScopeAttribute(const std::string & name)
	{
		if (name.empty())
		{
			throw invalid_argument("The name shouldn't be an empty.");
		}

		Scope& scope = AppendScope(name);
		AddPrescribedAttributeToHashmap(name);

		return scope;
	}

	/************************************************************************/
	void Attributed::AddNestedScopeAttribute(const std::string & name, Scope* nestedScope)
	{
		Adopt(*nestedScope, name);
		AddPrescribedAttributeToHashmap(name);
	}

	/************************************************************************/
	void Attributed::AddPrescribedAttributeToHashmap(const std::string & name)
	{
		auto& vect = sPrescribedAttributes[Attributed::TypeIdClass()];
		if (vect.Find(name) == vect.end())
		{
			vect.PushBack(name);
		}
	}
}