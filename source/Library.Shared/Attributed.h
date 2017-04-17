#pragma once

#include "Scope.h"

namespace Library
{
	/** Attributed is an abstract class used to express native data types to the scripting language.
	* @see Scope
	*/
	class Attributed: public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope);

	public:

		/** Attributed constructor.
		* It creates and initializes the scope data structures.
		*/
		Attributed();

		/** Attributed copy constructor.
		* It makes a deep copy of the passed Attributed.
		* @param rhs: Attributed to copy.
		*/
		Attributed(const Attributed & rhs);

		/** Attributed assignment operator.
		* It clears out the Attributed and does a deep copy of the passed Attributed.
		* @param rhs: Attributed to copy.
		*/
		Attributed& operator=(const Attributed& rhs);

		/** Attributed Destructor.
		*/
		virtual ~Attributed() = default;

		/** Determines if the passed string is an attribute.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param str: String to check.
		* @return Boolean expressing the result.
		*/
		bool IsAttribute(const std::string& str) const;

		/** Determines if the passed string is a prescribed attribute.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param str: String to check.
		* @return Boolean expressing the result.
		*/
		bool IsPrescribedAttribute(const std::string& str) const;

		/** Determines if the passed string is an auxiliary attribute.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param str: String to check.
		* @return Boolean expressing the result.
		*/
		bool IsAuxiliaryAttribute(const std::string& str) const;

		/** Appends an auxiliary attribute to the attributed.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @exception An exception is thrown if a prescribed attribute with the same name already exists.
		* @param str: Name of the attribute to append.
		* @return Reference to the datum representing the appended attribute.
		*/
		Datum& AppendAuxiliaryAttribute(const std::string& str);

		/** Clears out the cashed prescribed attributes.
		* This method needs to be called after we're done with any instances to avoid a memory leak.
		*/
		static void ClearCashedPrescribedAttributes();

		/** Copies the auxialary attributes of an Attribued.
		* @param attributed: Attributed to copy from.
		*/
		virtual void CopyAuxiliaryAttributes(const Attributed& attributed);

	protected:

		/** Initializes the attributed prescribed attributes.
		* This method is pure virtual so it needs to be implemented in derived classes.
		*/
		virtual void InitPrescribedAttributes() = 0;

		/** Adds an internal attribute to the attributed.
		* The datum representing the data of the attribute has internal storage.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param name: Name of the attribute to add.
		* @param initValue: Value used to initialize the data in the attribute.
		* @param size: Size of the datum of the attribute. It's defaulted to zero, which considered as one.
		* @return Reference to the datum representing the added attribute.
		* @see Datum
		*/
		Datum& AddInternalAttribute(const std::string& name, const std::int32_t initValue, const std::uint32_t size = 0);

		/** Adds an internal attribute to the attributed.
		* The datum representing the data of the attribute has internal storage.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param name: Name of the attribute to add.
		* @param initValue: Value used to initialize the data in the attribute.
		* @param size: Size of the datum of the attribute. It's defaulted to zero, which considered as one.
		* @return Reference to the datum representing the added attribute.
		* @see Datum
		*/
		Datum& AddInternalAttribute(const std::string& name, const std::float_t initValue, const std::uint32_t size = 0);

		/** Adds an internal attribute to the attributed.
		* The datum representing the data of the attribute has internal storage.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param name: Name of the attribute to add.
		* @param initValue: Value used to initialize the data in the attribute.
		* @param size: Size of the datum of the attribute. It's defaulted to zero, which considered as one.
		* @return Reference to the datum representing the added attribute.
		* @see Datum
		*/
		Datum& AddInternalAttribute(const std::string& name, const glm::vec4& initValue, const std::uint32_t size = 0);

		/** Adds an internal attribute to the attributed.
		* The datum representing the data of the attribute has internal storage.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param name: Name of the attribute to add.
		* @param initValue: Value used to initialize the data in the attribute.
		* @param size: Size of the datum of the attribute. It's defaulted to zero, which considered as one.
		* @return Reference to the datum representing the added attribute.
		* @see Datum
		*/
		Datum& AddInternalAttribute(const std::string& name, const glm::mat4x4& initValue, const std::uint32_t size = 0);

		/** Adds an internal attribute to the attributed.
		* The datum representing the data of the attribute has internal storage.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param name: Name of the attribute to add.
		* @param initValue: Value used to initialize the data in the attribute.
		* @param size: Size of the datum of the attribute. It's defaulted to zero, which considered as one.
		* @return Reference to the datum representing the added attribute.
		*/
		Datum& AddInternalAttribute(const std::string& name, const std::string& initValue, const std::uint32_t size = 0);

		/** Adds an internal attribute to the attributed.
		* The datum representing the data of the attribute has internal storage.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param name: Name of the attribute to add.
		* @param initValue: Value used to initialize the data in the attribute.
		* @param size: Size of the datum of the attribute. It's defaulted to zero, which considered as one.
		* @return Reference to the datum representing the added attribute.
		* @see Datum
		* @see RTTI
		*/
		Datum& AddInternalAttribute(const std::string& name, RTTI*const& initValue, const std::uint32_t size = 0);

		/** Adds an external attribute to the attributed.
		* The datum representing the data of the attribute has external storage.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param name: Name of the attribute to add.
		* @param value: External storage to link to the datum.
		* @param size: Size of the datum of the attribute. It's defaulted to zero, which considered as one.
		* @return Reference to the datum representing the added attribute.
		* @see Datum
		*/
		Datum& AddExternalAttribute(const std::string& name, std::int32_t* value, const std::uint32_t size = 0);

		/** Adds an external attribute to the attributed.
		* The datum representing the data of the attribute has external storage.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param name: Name of the attribute to add.
		* @param value: External storage to link to the datum.
		* @param size: Size of the datum of the attribute. It's defaulted to zero, which considered as one.
		* @return Reference to the datum representing the added attribute.
		* @see Datum
		*/
		Datum& AddExternalAttribute(const std::string& name, std::float_t* value, const std::uint32_t size = 0);

		/** Adds an external attribute to the attributed.
		* The datum representing the data of the attribute has external storage.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param name: Name of the attribute to add.
		* @param value: External storage to link to the datum.
		* @param size: Size of the datum of the attribute. It's defaulted to zero, which considered as one.
		* @return Reference to the datum representing the added attribute.
		* @see Datum
		*/
		Datum& AddExternalAttribute(const std::string& name, glm::vec4* value, const std::uint32_t size = 0);

		/** Adds an external attribute to the attributed.
		* The datum representing the data of the attribute has external storage.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param name: Name of the attribute to add.
		* @param value: External storage to link to the datum.
		* @param size: Size of the datum of the attribute. It's defaulted to zero, which considered as one.
		* @return Reference to the datum representing the added attribute.
		* @see Datum
		*/
		Datum& AddExternalAttribute(const std::string& name, glm::mat4x4* value, const std::uint32_t size = 0);

		/** Adds an external attribute to the attributed.
		* The datum representing the data of the attribute has external storage.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param name: Name of the attribute to add.
		* @param value: External storage to link to the datum.
		* @param size: Size of the datum of the attribute. It's defaulted to zero, which considered as one.
		* @return Reference to the datum representing the added attribute.
		* @see Datum
		*/
		Datum& AddExternalAttribute(const std::string& name, std::string* value, const std::uint32_t size = 0);

		/** Adds an external attribute to the attributed.
		* The datum representing the data of the attribute has external storage.
		* @exception An invalid argument exception is thrown if the passed string is empty.
		* @param name: Name of the attribute to add.
		* @param value: External storage to link to the datum.
		* @param size: Size of the datum of the attribute. It's defaulted to zero, which considered as one.
		* @return Reference to the datum representing the added attribute.
		* @see Datum
		* @see RTTI
		*/
		Datum& AddExternalAttribute(const std::string& name, RTTI** value, const std::uint32_t size = 0);

		/** Adds a new nested scope attribute to the attributed.
		* @param name: Name of the attribute to add.
		* @return Reference to the scope representing the added attribute.
		* @see Scope
		*/
		Scope& AddNestedScopeAttribute(const std::string& name);

		/** Adds a nested scope attribute to the attributed.
		* @param name: Name of the attribute to add.
		* @param nestedScope: Scope to be added.
		* @see Scope
		*/
		void AddNestedScopeAttribute(const std::string& name, Scope* nestedScope);

		/** Adds an empty scope attribute to the attributed.
		* @param name: Name of the attribute to add.
		* @return Reference to the created datum.
		* @see Scope
		*/
		Datum& AddEmptyNestedScopeAttribute(const std::string& name);

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