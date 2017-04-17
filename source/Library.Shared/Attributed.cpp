#include "pch.h"
#include "RTTI.h"
#include "Datum.h"
#include "HashMap.h"
#include "Attributed.h"

using namespace std;
using namespace glm;

namespace Library
{
	RTTI_DEFINITIONS(Attributed);

	HashMap<uint64_t, Vector<string>> Attributed::sPrescribedAttributes = HashMap<uint64_t, Vector<string>>();

	/************************************************************************/
	Attributed::Attributed() :
		Scope()
	{
		AddInternalAttribute("this", static_cast<RTTI*>(this));
	}

	/************************************************************************/
	Attributed::Attributed(const Attributed & rhs) :
		Scope(rhs)
	{
		(*this)["this"] = static_cast<RTTI*>(this);
	}

	/************************************************************************/
	Attributed& Attributed::operator=(const Attributed & rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(rhs);
			(*this)["this"] = static_cast<RTTI*>(this);
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

		return Find(str) != nullptr;
	}

	/************************************************************************/
	bool Attributed::IsPrescribedAttribute(const std::string & str) const
	{
		if (!str.empty())
		{
			Vector<std::string>& prescribedAttributes = sPrescribedAttributes[TypeIdInstance()];
			return IsAttribute(str) && prescribedAttributes.Find(str) != prescribedAttributes.end();
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
	void Attributed::ClearCashedPrescribedAttributes()
	{
		sPrescribedAttributes.Clear();
	}

	/************************************************************************/
	void Attributed::CopyAuxiliaryAttributes(const Attributed& attributed)
	{
		auto& table = attributed.GetTable();

		for (auto& pair : table)
		{
			if (attributed.IsAuxiliaryAttribute(pair.first))
			{
				auto& dat = AppendAuxiliaryAttribute(pair.first);
				dat = pair.second;
			}
		}
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
		Scope& scope = AppendScope(name);
		AddPrescribedAttributeToHashmap(name);

		return scope;
	}

	/************************************************************************/
	void Attributed::AddNestedScopeAttribute(const std::string & name, Scope* nestedScope)
	{
		// todo change nestedscope to a ref
		Adopt(*nestedScope, name);
		AddPrescribedAttributeToHashmap(name);
	}

	/************************************************************************/
	Datum& Attributed::AddEmptyNestedScopeAttribute(const string & name)
	{
		auto& dat = Append(name, Datum::DatumType::Table);
		if (dat.Type() != Datum::DatumType::Table)
		{
			throw exception("The Attribute already exist under a non table type.");
		}

		AddPrescribedAttributeToHashmap(name);

		return dat;
	}

	/************************************************************************/
	void Attributed::AddPrescribedAttributeToHashmap(const string & name)
	{
		auto& vect = sPrescribedAttributes[TypeIdInstance()];
		if (vect.Find(name) == vect.end())
		{
			vect.PushBack(name);
		}
	}
}