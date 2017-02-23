#pragma once

#include "RTTI.h"

namespace Library
{
	class Scope;

	/** Datum is a data structure that can store one of multiple types in a contiguous manner.
	 */
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
			Scope** scope;
		};

	public:

		/** This enumeration represents the possible types of data the Datum can store.
		 * Unknown represents the state where no type is used.
		 */
		enum class DatumType
		{
			Unknown,
			Integer,
			Float,
			Vector,
			Matrix,
			String,
			Pointer,
			Table
		};

#pragma region Ctors and dtor
		/** Datum default constructor.
		 * It creates and initializes the datum data structure.
		 */
		Datum(DatumType type = DatumType::Unknown);

		/** Datum destructor.
		 * If it holds internal storage, it clears it.
		 * If it has external storage, it just stops wrapping it.
		 * @see Clear
		 */
		~Datum();
		/** Datum Copy Constructor.
		* It does a deep copy of the passed Datum. The datum takes the same type as the datum to copy.
		* @param rhs: Datum to copy.
		*/

		Datum(const Datum & rhs);

#pragma endregion

#pragma region Operator=

		/** Datum Assignment Operator.
		* If the left hand side has internal storage, that storage will be cleared. If it has external storage, it stops wrapping it.
		* If the right hand side has internal storage, a deep copy of it is made. If it has external storage, the datum becomes a wrapper of that storage.
		* The type becomes identical to the right hand side type.
		* @param rhs: Assignee Datum.
		* @see Clear
		*/
		Datum& operator=(const Datum& rhs);

		/** Datum Integer Scalar Assignment Operator.
		* It sets the first element in the datum to the right hand side Integer.
		* @param rhs: Assignee Integer.
		* @see Set
		*/
		Datum& operator=(const std::int32_t& rhs);

		/** Datum Float Scalar Assignment Operator.
		* It sets the first element in the datum to the right hand side Float.
		* @param rhs: Assignee Float.
		* @see Set
		*/
		Datum& operator=(const std::float_t& rhs);

		/** Datum Vector4 Scalar Assignment Operator.
		* It sets the first element in the datum to the right hand side Vector4.
		* @param rhs: Assignee Vector4.
		* @see Set
		*/
		Datum& operator=(const glm::vec4& rhs);

		/** Datum Matrix4x4 Scalar Assignment Operator.
		* It sets the first element in the datum to the right hand side Matrix4x4.
		* @param rhs: Assignee Matrix4x4.
		* @see Set
		*/
		Datum& operator=(const glm::mat4x4& rhs);

		/** Datum String Scalar Assignment Operator.
		* It sets the first element in the datum to the right hand side String.
		* @param rhs: Assignee String.
		* @see Set
		*/
		Datum& operator=(const std::string& rhs);

		/** Datum RTTI Scalar Assignment Operator.
		* It sets the first element in the datum to the right hand side RTTI.
		* @param rhs: Assignee RTTI.
		* @see Set
		* @see RTTI
		*/
		Datum& operator=(RTTI *const& rhs);

		/** Datum Scope Scalar Assignment Operator.
		* It sets the first element in the datum to the right hand side Scope.
		* @param rhs: Assignee Scope.
		* @see Set
		* @see Scope
		*/
		Datum& operator=(Scope *const& rhs);

#pragma endregion

#pragma region Opertaor== and operator!=

		/** Datum Equality Operator.
		* It compares the type, the size and then each element of the datums to check equality.
		* @exception If either side has an invalid type, an exception is thrown.
		* @param rhs: Datum to compare to.
		*/
		bool operator==(const Datum& rhs) const;

		/** Datum Integer Scalar Equality Operator.
		* It compares the first element in the datum to the right hand side Integer.
		* @exception If the datum type is not Integer, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: Integer to compare to.
		* @see IsEmpty
		*/
		bool operator==(const std::int32_t& rhs) const;

		/** Datum Float Scalar Equality Operator.
		* It compares the first element in the datum to the right hand side Float.
		* @exception If the datum type is not Float, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: Float to compare to.
		* @see IsEmpty
		*/
		bool operator==(const std::float_t& rhs) const;

		/** Datum Vector4 Scalar Equality Operator.
		* It compares the first element in the datum to the right hand side Vector4.
		* @exception If the datum type is not Vector4, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: Vector4 to compare to.
		* @see IsEmpty
		*/
		bool operator==(const glm::vec4& rhs) const;

		/** Datum Matrix4x4 Scalar Equality Operator.
		* It compares the first element in the datum to the right hand side Matrix4x4.
		* @exception If the datum type is not Matrix4x4, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: Matrix4x4 to compare to.
		* @see IsEmpty
		*/
		bool operator==(const glm::mat4x4& rhs) const;

		/** Datum String Scalar Equality Operator.
		* It compares the first element in the datum to the right hand side String.
		* @exception If the datum type is not String, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: String to compare to.
		* @see IsEmpty
		*/
		bool operator==(const std::string& rhs) const;

		/** Datum RTTI Scalar Equality Operator.
		* It compares the first element in the datum to the right hand side RTTI.
		* @exception If the datum type is not RTTI, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: RTTI to compare to.
		* @see IsEmpty
		* @see RTTI
		*/
		bool operator==(const RTTI* const & rhs) const;

		/** Datum Scope Scalar Equality Operator.
		* It compares the first element in the datum to the right hand side Scope.
		* @exception If the datum type is not Scope, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: Scope to compare to.
		* @see IsEmpty
		* @see Scope
		*/
		bool operator==(const Scope* const & rhs) const;

		/** Datum Non Equality Operator.
		* It compares the type, the size and then each element of the datums to check equality.
		* @exception If either side has an invalid type, an exception is thrown.
		* @param rhs: Datum to compare to.
		*/
		bool operator!=(const Datum& rhs) const;

		/** Datum Integer Scalar Non Equality Operator.
		* It compares the first element in the datum to the right hand side Integer.
		* @exception If the datum type is not Integer, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: Integer to compare to.
		* @see IsEmpty
		*/
		bool operator!=(const std::int32_t& rhs) const;

		/** Datum Float Scalar Non Equality Operator.
		* It compares the first element in the datum to the right hand side Float.
		* @exception If the datum type is not Float, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: Float to compare to.
		* @see IsEmpty
		*/
		bool operator!=(const std::float_t& rhs) const;

		/** Datum Vector4 Scalar Non Equality Operator.
		* It compares the first element in the datum to the right hand side Vector4.
		* @exception If the datum type is not Vector4, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: Vector4 to compare to.
		* @see IsEmpty
		*/
		bool operator!=(const glm::vec4& rhs) const;

		/** Datum Matrix4x4 Scalar Non Equality Operator.
		* It compares the first element in the datum to the right hand side Matrix4x4.
		* @exception If the datum type is not Matrix4x4, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: Matrix4x4 to compare to.
		* @see IsEmpty
		*/
		bool operator!=(const glm::mat4x4& rhs) const;

		/** Datum String Scalar Non Equality Operator.
		* It compares the first element in the datum to the right hand side String.
		* @exception If the datum type is not String, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: String to compare to.
		* @see IsEmpty
		*/
		bool operator!=(const std::string& rhs) const;

		/** Datum RTTI Scalar Non Equality Operator.
		* It compares the first element in the datum to the right hand side RTTI.
		* @exception If the datum type is not RTTI, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: RTTI to compare to.
		* @see IsEmpty
		* @see RTTI
		*/
		bool operator!=(const RTTI* const & rhs) const;

		/** Datum Scope Scalar Non Equality Operator.
		* It compares the first element in the datum to the right hand side Scope.
		* @exception If the datum type is not Scope, an exception is thrown.
		* @exception If the datum is empty, an exception is thrown.
		* @param rhs: Scope to compare to.
		* @see IsEmpty
		* @see Scope
		*/
		bool operator!=(const Scope* const & rhs) const;

#pragma endregion

#pragma region Special Operator[]

		/** Grants access to nested scopes.
		* @param index: Index of the nested scope to get.
		* @return Reference to the wanted scope.
		*/
		Scope& operator[](std::uint32_t index);

#pragma endregion

#pragma region Type and size

		/** Returns the current data type of the datum.
		 * @return Datum data type.
		 */
		DatumType Type() const;

		/** Sets the type of the datum data type if it's not set.
		 * @exception If the datum type is already set to a valid type, or the parameter type is invalid, an exception is thrown.
		 * @param type: The datum type to set to.
		 */
		void SetType(const DatumType& type);

		/** Returns the current datum size.
		 * @return Datum size.
		 */
		std::uint32_t Size() const;


		/** Sets the datum size.
		* If the new size is bigger than the old size, the datum grows size by growing the capacity if necessary, and initializing the new slots.
		* If the new size is smaller than the old size, the datum shrinks while deleting any unnecessary elements.
		* @exception If the datum type is not set to a valid type yet, and exception is thrown.
		* @exception If the datum has external storage, an exception is thrown.
		* @param size: The new datum size.
		*/
		void SetSize(const std::uint32_t size);

#pragma endregion

#pragma region Storage Clear

		/** Clears the datum by going through its elements and deleting them.
		* @exception If the datum has external storage, an exception is thrown.
		* @see Reserve
		* @see PushBack
		*/
		void Clear();

		/** Deletes the back element of the datum.
		* If the datum is empty, nothing happens.
		* @exception If the datum has external storage, an exception is thrown.
		*/
		void PopBack();

		/** Removes a certain element from the datum.
		* If the datum is empty, nothing happens.
		* @param index: Index of the entry to remove.
		* @param deleteIt: Boolean determining if the entry should be deleted or not.
		* @exception If the datum has external storage, an exception is thrown.
		* @exception If the index is invalid, an exception is thrown.
		*/
		void Remove(std::uint32_t index ,bool deleteIt = true);


#pragma endregion

#pragma region Set Storage

		/** Sets an external storage to the datum.
		* If the datum has internal storage already, it clears it.
		* It changes the type of the datum to Integer, and the storage type to external.
		* @param table: Integer table representing the storage.
		* @param size: Storage size.
		* @see Clear
		*/
		void SetStorage(std::int32_t*& table, const std::uint32_t tableSize);

		/** Sets an external storage to the datum.
		* If the datum has internal storage already, it clears it.
		* It changes the type of the datum to Float, and the storage type to external.
		* @param table: Float table representing the storage.
		* @param size: Storage size.
		* @see Clear
		*/
		void SetStorage(std::float_t*& table, const std::uint32_t tableSize);

		/** Sets an external storage to the datum.
		* If the datum has internal storage already, it clears it.
		* It changes the type of the datum to Vector4, and the storage type to external.
		* @param table: Vector4 table representing the storage.
		* @param size: Storage size.
		* @see Clear
		*/
		void SetStorage(glm::vec4*& table, const std::uint32_t tableSize);

		/** Sets an external storage to the datum.
		* If the datum has internal storage already, it clears it.
		* It changes the type of the datum to Matrix4x4, and the storage type to external.
		* @param table: Matrix4x4 table representing the storage.
		* @param size: Storage size.
		* @see Clear
		*/
		void SetStorage(glm::mat4x4*& table, const std::uint32_t tableSize);

		/** Sets an external storage to the datum.
		* If the datum has internal storage already, it clears it.
		* It changes the type of the datum to String, and the storage type to external.
		* @param table: String table representing the storage.
		* @param size: Storage size.
		* @see Clear
		*/
		void SetStorage(std::string*& table, const std::uint32_t tableSize);

		/** Sets an external storage to the datum.
		* If the datum has internal storage already, it clears it.
		* It changes the type of the datum to RTTI, and the storage type to external.
		* @param table: RTTI table representing the storage.
		* @param size: Storage size.
		* @see Clear
		*/
		void SetStorage(RTTI**& table, const std::uint32_t tableSize);

#pragma endregion

#pragma region Set

		/** Sets a certain element in the datum.
		* If the datum type is not set to a valid type yet, it is set to Integer.
		* If the index points to the slot after the last element, rhs is pushed back.
		* @exception If the datum type is not Integer, an exception is thrown.
		* @exception If the index is invalid, an exception is thrown.
		* @param rhs: The element to set.
		* @param index: The slot to set. 0 by default
		* @see PushBack
		*/
		void Set(const std::int32_t& rhs, const std::uint32_t index = 0);

		/** Sets a certain element in the datum.
		* If the datum type is not set to a valid type yet, it is set to Float.
		* If the index points to the slot after the last element, rhs is pushed back.
		* @exception If the datum type is not Float, an exception is thrown.
		* @exception If the index is invalid, an exception is thrown.
		* @param rhs: The element to set.
		* @param index: The slot to set. 0 by default
		* @see PushBack
		*/
		void Set(const std::float_t& rhs, const std::uint32_t index = 0);

		/** Sets a certain element in the datum.
		* If the datum type is not set to a valid type yet, it is set to Vector4.
		* If the index points to the slot after the last element, rhs is pushed back.
		* @exception If the datum type is not Vector4, an exception is thrown.
		* @exception If the index is invalid, an exception is thrown.
		* @param rhs: The element to set.
		* @param index: The slot to set. 0 by default
		* @see PushBack
		*/
		void Set(const glm::vec4& rhs, const std::uint32_t index = 0);

		/** Sets a certain element in the datum.
		* If the datum type is not set to a valid type yet, it is set to Matrix4x4.
		* If the index points to the slot after the last element, rhs is pushed back.
		* @exception If the datum type is not Matrix4x4, an exception is thrown.
		* @exception If the index is invalid, an exception is thrown.
		* @param rhs: The element to set.
		* @param index: The slot to set. 0 by default
		* @see PushBack
		*/
		void Set(const glm::mat4x4& rhs, const std::uint32_t index = 0);

		/** Sets a certain element in the datum.
		* If the datum type is not set to a valid type yet, it is set to String.
		* If the index points to the slot after the last element, rhs is pushed back.
		* @exception If the datum type is not String, an exception is thrown.
		* @exception If the index is invalid, an exception is thrown.
		* @param rhs: The element to set.
		* @param index: The slot to set. 0 by default
		* @see PushBack
		*/
		void Set(const std::string& rhs, const std::uint32_t index = 0);

		/** Sets a certain element in the datum.
		* If the datum type is not set to a valid type yet, it is set to Pointer.
		* If the index points to the slot after the last element, rhs is pushed back.
		* @exception If the datum type is not Pointer, an exception is thrown.
		* @exception If the index is invalid, an exception is thrown.
		* @param rhs: The element to set.
		* @param index: The slot to set. 0 by default
		* @see PushBack
		*/
		void Set(RTTI*const & rhs, const std::uint32_t index = 0);

		/** Sets a certain element in the datum.
		* If the datum type is not set to a valid type yet, it is set to Table.
		* If the index points to the slot after the last element, rhs is pushed back.
		* @exception If the datum type is not Table, an exception is thrown.
		* @exception If the index is invalid, an exception is thrown.
		* @param rhs: The element to set.
		* @param index: The slot to set. 0 by default
		* @see PushBack
		*/
		void Set(Scope*const & rhs, const std::uint32_t index = 0);

#pragma endregion

#pragma region Get Templates


		/** Gets an element from the datum.
		* This is the templated version.
		* @exception If the index is invalid, an exception is thrown.
		* @param index: The slot to get.
		* @return The wanted Element.
		*/
		template<typename T>
		const T& Get(const std::uint32_t index = 0) const;

		/** Gets an element from the datum.
		* This is the templated version.
		* @exception If the index is invalid, an exception is thrown.
		* @param index: The slot to get.
		* @return The wanted Element.
		*/
		template<typename T>
		T& Get(const std::uint32_t index = 0);

#pragma endregion

#pragma region String related methods


		/** Sets a certain element in the datum by parsing a string.
		* @exception If the datum type is not valid, an exception is thrown.
		* @exception If the input string is invalid(non parsable), an exception is thrown.
		* @param rhs: The string to parse.
		* @param index: The slot to set. 0 by default
		* @see Set
		*/
		void SetFromString(const std::string& str, const std::uint32_t index = 0);

		/** Return a string representing a certain data in the datum.
		* @param index: The slot to print. 0 by default
		* @see Get
		*/
		std::string ToString(const std::uint32_t index = 0);

#pragma endregion

#pragma region PushBack

		/** Add an element to the back of the datum.
		* It grows the datum capacity if needed.
		* If the datum type is not set to a valid type yet, it is set to Integer.
		* @exception If the datum type is not Integer, an exception is thrown.
		* @param rhs: The new Integer to add.
		*/
		void PushBack(const std::int32_t& rhs);

		/** Add an element to the back of the datum.
		* It grows the datum capacity if needed.
		* If the datum type is not set to a valid type yet, it is set to Float.
		* @exception If the datum type is not Float, an exception is thrown.
		* @param rhs: The new Float to add.
		*/
		void PushBack(const std::float_t& rhs);

		/** Add an element to the back of the datum.
		* It grows the datum capacity if needed.
		* If the datum type is not set to a valid type yet, it is set to Vector4.
		* @exception If the datum type is not Vector4, an exception is thrown.
		* @param rhs: The new Vector4 to add.
		*/
		void PushBack(const glm::vec4& rhs);

		/** Add an element to the back of the datum.
		* It grows the datum capacity if needed.
		* If the datum type is not set to a valid type yet, it is set to Matrix4x4.
		* @exception If the datum type is not Matrix4x4, an exception is thrown.
		* @param rhs: The new Matrix4x4 to add.
		*/
		void PushBack(const glm::mat4x4& rhs);

		/** Add an element to the back of the datum.
		* It grows the datum capacity if needed.
		* If the datum type is not set to a valid type yet, it is set to String.
		* @exception If the datum type is not String, an exception is thrown.
		* @param rhs: The new String to add.
		*/
		void PushBack(const std::string& rhs);

		/** Add an element to the back of the datum.
		* It grows the datum capacity if needed.
		* If the datum type is not set to a valid type yet, it is set to Pointer.
		* @exception If the datum type is not Pointer, an exception is thrown.
		* @param rhs: The new Pointer to add.
		*/
		void PushBack(RTTI *const & rhs);

		/** Add an element to the back of the datum.
		* It grows the datum capacity if needed.
		* If the datum type is not set to a valid type yet, it is set to Pointer.
		* @exception If the datum type is not Table, an exception is thrown.
		* @param rhs: The new Table to add.
		*/
		void PushBack(Scope *const & rhs);

#pragma endregion

#pragma region Storage Checks


		/** Returns the current datum capacity.
		* @return Datum Capacity.
		*/
		std::uint32_t Capacity() const;

		/** Checks if the datum is empty.
		* @return Boolean expressing if the datum is empty.
		*/
		bool IsEmpty() const;

		/** Checks if the datum has external storage.
		* @return Boolean expressing if the datum has external storage.
		*/
		bool HasExternalStorage() const;

#pragma endregion

	private:

		bool Reserve(const std::uint32_t newCapacity);

		void Empty();
		void DeepCopy(const Datum& rhs);

		DataValues mData;
		DatumType mCurrentType;
		std::uint32_t mSize;
		std::uint32_t mCapacity;
		bool mExternalStorage;
	};

	/** Gets an element from the datum.
	* This is the Integer template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted Integer.
	*/
	template<>
	const std::int32_t& Datum::Get<std::int32_t>(const std::uint32_t index) const;

	/** Gets an element from the datum.
	* This is the Integer template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted Integer.
	*/
	template<>
	std::int32_t& Datum::Get<std::int32_t>(const std::uint32_t index);

	/** Gets an element from the datum.
	* This is the Float template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted Float.
	*/
	template<>
	const std::float_t& Datum::Get<std::float_t>(const std::uint32_t index) const;

	/** Gets an element from the datum.
	* This is the Float template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted Float.
	*/
	template<>
	std::float_t& Datum::Get<std::float_t>(const std::uint32_t index);

	/** Gets an element from the datum.
	* This is the Vector4 template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted Vector4.
	*/
	template<>
	const glm::vec4& Datum::Get<glm::vec4>(const std::uint32_t index) const;

	/** Gets an element from the datum.
	* This is the Vector4 template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted Vector4.
	*/
	template<>
	glm::vec4& Datum::Get(const std::uint32_t index);

	/** Gets an element from the datum.
	* This is the Matrix4x4 template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted Matrix4x4.
	*/
	template<>
	const glm::mat4x4& Datum::Get<glm::mat4x4>(const std::uint32_t index) const;

	/** Gets an element from the datum.
	* This is the Matrix4x4 template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted Matrix4x4.
	*/
	template<>
	glm::mat4x4& Datum::Get<glm::mat4x4>(const std::uint32_t index);

	/** Gets an element from the datum.
	* This is the String template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted String.
	*/
	template<>
	const std::string& Datum::Get<std::string>(const std::uint32_t index) const;

	/** Gets an element from the datum.
	* This is the String template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted String.
	*/
	template<>
	std::string& Datum::Get<std::string>(const std::uint32_t index);

	/** Gets an element from the datum.
	* This is the RTTI template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted RTTI.
	* @see RTTI
	*/
	template<>
	const RTTI* const & Datum::Get<const RTTI*>(const std::uint32_t index) const;

	/** Gets an element from the datum.
	* This is the RTTI template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted RTTI.
	* @see RTTI
	*/
	template<>
	RTTI*& Datum::Get<RTTI*>(const std::uint32_t index);

	/** Gets an element from the datum.
	* This is the Scope template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted Scope.
	* @see Scope
	*/
	template<>
	const Scope* const & Datum::Get<const Scope*>(const std::uint32_t index) const;

	/** Gets an element from the datum.
	* This is the Scope template specialization of get.
	* @exception If the index is invalid, an exception is thrown.
	* @param index: The slot to get.
	* @return The wanted Scope.
	* @see Scope
	*/
	template<>
	Scope*& Datum::Get<Scope*>(const std::uint32_t index);


}
