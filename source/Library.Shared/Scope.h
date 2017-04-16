#pragma once

#include "RTTI.h"
#include "HashMap.h"
#include "Datum.h"

namespace Library
{
	/** Scope forms a hierarchical database with datum.
	* @see Datum
	* @see RTTI
	*/
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);

	public:

		/** Scope Default Constructor.
		* It creates and initializes the scope data structures.
		* @param tableSize: The size of the scope HashMap.
		* @see HashMap
		*/
		explicit Scope(std::uint32_t tableSize = DEFAULT_INITIAL_TABLE_SIZE);

		/** Scope Destructor.
		* It clears out the scope and its children scopes.
		*/
		virtual ~Scope();

		/** Scope copy constructor.
		* It makes a deep copy of the passed scope.
		* @param rhs: Scope to copy.
		*/
		Scope(const Scope & rhs);

		/** Scope assignment operator.
		* It clears out the scope and does a deep copy of the passed Scope.
		* @param rhs: Scope to copy.
		*/
		Scope& operator=(const Scope& rhs);

		/** Finds an element in the scope.
		* @exception : An invalid argument exception is thrown if the passed string is empty.
		* @param str: The key of the wanted element.
		* @return A pointer to the found element, or a null pointer if there is no such element.
		*/
		Datum* Find(const std::string & str) const;

		/** Searches for an element in the scope and its ancestors.
		* @exception : An invalid argument exception is thrown if the passed string is empty.
		* @param str: The key of the wanted element.
		* @param foundScope: The scope that the element was found in. It is defaulted to null pointer.
		* @return A pointer to the found datum, or a null pointer if there is no such key.
		* @see Datum
		* @see Find
		*/
		Datum* Search(const std::string& str, Scope** foundScope = nullptr) const;

		/** Creates a new entry in the scope and returns it, or just returns an entry if the key already exists.
		* @exception : An invalid argument exception is thrown if the passed string is empty.
		* @param str: The key of the new datum.
		* @param type: The type of the element to create.
		* @return A reference to the added or already existent Datum.
		* @see Datum
		*/
		Datum& Append(const std::string& str, Datum::DatumType type = Datum::DatumType::Unknown);

		/** Creates a new child scope and appends it to the scope.
		* If the key already exits, the child scope is added to that datum.
		* @exception : An invalid argument exception is thrown if the passed string is empty.
		* @exception : An exception is thrown if the already existent datum is not of type table.
		* @param str: The key of the scope to create.
		* @return A reference to the added Scope.
		*/
		Scope& AppendScope(const std::string& str);

		/** Adds a scope as a child to the current scope.
		* The adopted scope is orphaned from its old parent, if it had one.
		* @exception : An invalid argument exception is thrown if the scope tries to adopt itself.
		* @exception : An invalid argument exception is thrown if the passed string is empty.
		* @param scope: The scope to adopt.
		* @param str: The key where the scope is appended.
		*/
		void Adopt(Scope& scope, const std::string& str);

		/** Gets the scope parent.
		* @return Pointer to the parent scope.
		* @see Scope
		*/
		Scope* GetParent() const;

		/** Brackets operator for Scope.
		* Returns a certain Datum, or creates a new entry in the scope and returns, it if none exists, with the passed key.
		* @exception : An invalid argument exception is thrown if the passed string is empty.
		* @param str: The key of the datum to get.
		* @return A reference to the wanted or newly added Datum.
		* @see Datum
		*/
		Datum& operator[](const std::string str);

		/** Brackets operator for Scope.
		* Returns a certain Datum based on the order the datums where added to the scope.
		* @param index: The index of the datum to get.
		* @return A reference to the wanted Datum.
		* @see Datum
		*/
		Datum& operator[](const std::uint32_t index);

		/** Equality operator for Scope.
		* @param rhs: The scope to compare to.
		* @return Boolean to express the equality between the scopes.
		*/
		bool operator==(const Scope& rhs) const;

		/** Non Equality operator for Scope.
		* @param rhs: The scope to compare to.
		* @return Boolean to express the non equality between the scopes.
		*/
		bool operator!=(const Scope& rhs) const;

		/** Returns the name of the key of a child scope.
		* Returns an empty string if the scope does not exist within this scope.
		* @param scope: The scope to search the name of.
		* @return The name of the key of the child scope.
		*/
		std::string FindName(const Scope* scope) const;

		/** Clears the scope.
		* It orphans the scope from its parent.
		*/
		void Clear();

		/** Returns a string representing the data in the scope.
		* @return The string representing the data.
		* @see RTTI
		*/
		std::string ToString() const override;

		/** Compares the scope with an RTTI object.
		* The rhs RTTI is considered as a Scope.
		* @param rhs: RTTI to compare to.
		* @return Boolean to express the equality.
		* @see RTTI
		*/
		bool Equals(const RTTI* rhs) const override;

	protected:

		HashMap<std::string, Datum>& GetTable();

	private:

		HashMap<std::string, Datum> mTable;
		Vector<HashMap<std::string, Datum>::Iterator> mVect;

		Scope* mParentScope;
		const static std::uint32_t DEFAULT_INITIAL_TABLE_SIZE = 16U;

		Scope& AppendScope(const std::string& str, Scope* scope);
		void Orphan();
		bool RemoveScope(const Scope* scope);
	};
}