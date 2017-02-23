#pragma once

namespace Library
{
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI);

	public:

		typedef HashMap<const std::string, Datum>::Iterator HashMapIterator;
		typedef Vector<HashMapIterator>::Iterator VectorIterator;
		typedef std::pair<const std::string, Datum> StringDatumPair;

		Scope(std::uint32_t tableSize = DEFAULT_INITIAL_TABLE_SIZE);
		virtual ~Scope();

		Scope(const Scope & rhs);
		Scope& operator=(const Scope& rhs);

		Datum* Find(const std::string & str) const;
		Datum* Search(const std::string& str, Scope** foundScope = nullptr) const;
		Datum& Append(const std::string& str);
		Scope& AppendScope(const std::string& str);
		void Adopt(Scope& scope, const std::string& str);
		Scope* GetParent() const;
		Datum& operator[](const std::string str);
		Datum& operator[](const std::uint32_t index);

		bool operator==(const Scope& rhs) const;
		bool operator!=(const Scope& rhs) const;
		std::string FindName(const Scope* scope) const;

		void Clear();
		void Orphan(); 

		std::string ToString() const override;
		bool Equals(const RTTI* rhs) const override;

	private:

		HashMap<const std::string, Datum> mTable;
		Vector<HashMapIterator> mVect;
		Scope* mParentScope;
		const static std::uint32_t DEFAULT_INITIAL_TABLE_SIZE = 16U;

		Scope& AppendScope(const std::string& str, Scope* scope);
		bool RemoveScope(const Scope* scope);
	};
}