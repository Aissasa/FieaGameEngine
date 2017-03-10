#pragma once

#include "Scope.h"

namespace Library
{
	class Attributed: public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:

		Attributed();
		Attributed(const Attributed & rhs);
		// urgent do the other move semantics first
		//Attributed(const Attributed && rhs);

		Attributed& operator=(const Attributed& rhs);
		// urgent do the other move semantics first
		//Attributed& operator=(const Attributed&& rhs);
		virtual ~Attributed() = default;

		bool IsAttribute(const std::string& str) const;

		bool IsPrescribedAttribute(const std::string& str) const;

		bool IsAuxiliaryAttribute(const std::string& str) const;

		Datum& AppendAuxiliaryAttribute(const std::string& str);

		std::uint32_t AuxiliaryBegin() const;

		static void ClearCashedAttributes();

	protected:

		virtual void InitPrescribedAttributes() = 0;

		Datum& AddInternalAttribute(const std::string& name, const std::int32_t initValue, const std::uint32_t size = 0);
		Datum& AddInternalAttribute(const std::string& name, const std::float_t initValue, const std::uint32_t size = 0);
		Datum& AddInternalAttribute(const std::string& name, const glm::vec4& initValue, const std::uint32_t size = 0);
		Datum& AddInternalAttribute(const std::string& name, const glm::mat4x4& initValue, const std::uint32_t size = 0);
		Datum& AddInternalAttribute(const std::string& name, const std::string& initValue, const std::uint32_t size = 0);
		Datum& AddInternalAttribute(const std::string& name, RTTI*const& initValue, const std::uint32_t size = 0);

		Datum& AddExternalAttribute(const std::string& name, std::int32_t* value, const std::uint32_t size = 0);
		Datum& AddExternalAttribute(const std::string& name, std::float_t* value, const std::uint32_t size = 0);
		Datum& AddExternalAttribute(const std::string& name, glm::vec4* value, const std::uint32_t size = 0);
		Datum& AddExternalAttribute(const std::string& name, glm::mat4x4* value, const std::uint32_t size = 0);
		Datum& AddExternalAttribute(const std::string& name, std::string* value, const std::uint32_t size = 0);
		Datum& AddExternalAttribute(const std::string& name, RTTI** value, const std::uint32_t size = 0);

		Scope& AddNestedScopeAttribute(const std::string& name);
		void AddNestedScopeAttribute(const std::string& name, Scope* nestedScope);

	private:

		static HashMap<std::uint64_t, Vector<std::string>> sPrescribedAttributes;
		template <typename T> void InitializeDatum(Datum& datum, const T& initValue, const std::uint32_t size);
		void AddPrescribedAttributeToHashmap(const std::string & name);
	};

	template<typename T>
	inline void Attributed::InitializeDatum(Datum & datum, const T & initValue, const std::uint32_t size)
	{
		if (size < 2)
		{
			datum = initValue;
		}
		else
		{
			datum.Reserve(size);
			for (std::uint32_t i = 0; i < size; ++i)
			{
				datum.PushBack(initValue);
			}
		}
	}
}