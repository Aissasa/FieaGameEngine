#include "pch.h"
#include "Vector.h"
#include "Scope.h"

using namespace std;

namespace Library
{
	RTTI_DEFINITIONS(Scope);

	typedef HashMap<std::string, Datum>::Iterator HashMapIterator;
	typedef Vector<HashMapIterator>::Iterator VectorIterator;
	typedef std::pair<const string, Datum> StringDatumPair;

	/************************************************************************/
	Scope::Scope(uint32_t tableSize) :
		mTable(HashMap<string, Datum>(tableSize)), mVect(Vector<HashMapIterator>()), mParentScope(nullptr)
	{
	}

	/************************************************************************/
	Scope::~Scope()
	{
		Clear();
	}

	/************************************************************************/
	Scope::Scope(const Scope & rhs) :
		mParentScope(nullptr)
	{
		operator=(rhs);
	}

	/************************************************************************/
	Scope& Scope::operator=(const Scope & rhs)
	{
		if (this != &rhs)
		{
			Clear();
			// go through all the entries in the vector
			for (VectorIterator it = rhs.mVect.begin(); it != rhs.mVect.end(); ++it)
			{
				StringDatumPair& p = *(*it);
				if (p.second.Type() == Datum::DatumType::Table)
				{
					for (uint32_t i = 0; i < p.second.Size(); ++i)
					{
						Scope* scope = new Scope();
						// recursive calls to populate the children scopes
						*scope = p.second[i];
						AppendScope(p.first, scope);
					}
				}
				else
				{
					Datum& dat = Append(p.first, p.second.Type());
					dat = p.second;
				}
			}
		}

		return *this;
	}

	/************************************************************************/
	Datum* Scope::Find(const string & str) const
	{
		if (str.empty())
		{
			throw invalid_argument("The key shouldn't be an empty string.");
		}

		HashMapIterator it = mTable.Find(str);
		return (it == mTable.end()) ? nullptr : &((*it).second);
	}

	/************************************************************************/
	Datum* Scope::Search(const string& str, Scope** foundScope) const
	{
		if (str.empty())
		{
			throw invalid_argument("The key shouldn't be an empty string.");
		}

		Datum* dat = Find(str);
		if (dat)
		{
			if (foundScope)
			{
				*foundScope = const_cast<Scope*>(this);
			}
			return dat;
		}
		else if (mParentScope)
		{
			return mParentScope->Search(str, foundScope);
		}
		else
		{
			if (foundScope)
			{
				*foundScope = nullptr;
			}
			return nullptr;
		}
	}

	/************************************************************************/
	Datum& Scope::Append(const string & str, Datum::DatumType type)
	{
		if (str.empty())
		{
			throw invalid_argument("The key shouldn't be an empty string.");
		}
		bool inserted = false;
		HashMapIterator it = mTable.Insert(StringDatumPair(str, Datum(type)), inserted);

		// push if actually inserted a new entry
		if (inserted)
		{
			mVect.PushBack(it);
		}
		return (*it).second;
	}

	/************************************************************************/
	Scope& Scope::AppendScope(const string & str)
	{
		return AppendScope(str, nullptr);
	}

	/************************************************************************/
	void Scope::Adopt(Scope& scope, const string& str)
	{
		if (this == &scope)
		{
			throw invalid_argument("A scope cannot adopt itself");
		}

		if (str.empty())
		{
			throw invalid_argument("The key shouldn't be an empty string.");
		}

		// make sure it does not belong to another scope anymore
		scope.Orphan();
		AppendScope(str, &scope);
	}

	/************************************************************************/
	Scope* Scope::GetParent() const
	{
		return mParentScope;
	}

	/************************************************************************/
	Datum& Scope::operator[](const std::string str)
	{
		return Append(str);
	}

	/************************************************************************/
	Datum& Scope::operator[](const std::uint32_t index)
	{
		return (*mVect[index]).second;
	}

	/************************************************************************/
	bool Scope::operator==(const Scope & rhs) const
	{
		if (this == &rhs)
		{
			return true;
		}

		if (mParentScope != rhs.mParentScope || mVect.Size() != rhs.mVect.Size())
		{
			return false;
		}
		bool same = true;

		auto it1 = mVect.begin();
		auto it2 = rhs.mVect.begin();

		for (; it1 != mVect.end(); ++it1)
		{
			auto v1 = *(*it1);
			auto v2 = *(*it2);

			// urgent think of a better way to compare datums of scopes
			if (v1.second.Type() == Datum::DatumType::Table && v2.second.Type() == Datum::DatumType::Table
				&& v1.second.Size() == v2.second.Size())
			{
				if (v1.first != v2.first)
				{
					same = false;
					break;
				}

				for (uint32_t i = 0; i < v1.second.Size(); ++i)
				{
					if (v1.second[i] != v2.second[i])
					{
						return false;
					}
				}
			}
			if (v1 != v2)
			{
				same = false;
				break;
			}
			++it2;
		}
		return same;
	}

	/************************************************************************/
	bool Scope::operator!=(const Scope & rhs) const
	{
		return !(operator==(rhs));
	}

	/************************************************************************/
	string Scope::FindName(const Scope* scope) const
	{
		for (VectorIterator it = mVect.begin(); it != mVect.end(); ++it)
		{
			if ((*(*it)).second == scope)
			{
				return (*(*it)).first;
			}
		}
		return "";
	}

	/************************************************************************/
	void Scope::Clear()
	{
		Orphan();

		for (VectorIterator it = mVect.begin(); it != mVect.end(); ++it)
		{
			auto& p = *(*it);
			if (p.second.Type() == Datum::DatumType::Table)
			{
				auto size = p.second.Size();
				for (uint32_t i = 0; i < size; ++i)
				{
					// todo figure this out
					delete &(p.second[0]);
				}
			}
			p.second.Clear();
		}

		mVect.Empty();
		mTable.Clear();
	}

	/************************************************************************/
	void Scope::Orphan()
	{
		if (mParentScope)
		{
			mParentScope->RemoveScope(this);
			mParentScope = nullptr;
		}
	}

	/************************************************************************/
	std::string Scope::ToString() const
	{
		string str;
		for (VectorIterator it = mVect.begin(); it != mVect.end(); ++it)
		{
			auto& dat = (*(*it)).second;
			uint32_t size = dat.Size();
			for (uint32_t i = 0; i < size; i++)
			{
				if (dat.Type() == Datum::DatumType::Table)
				{
					str = str + "\n" + dat.ToString(i);

				}
				else
				{
					str = str + dat.ToString(i) + "\n";
				}
			}
		}
		return str;
	}

	/************************************************************************/
	bool Scope::Equals(const RTTI * rhs) const
	{
		if (!rhs)
		{
			return false;
		}
		return operator==(*rhs->As<Scope>());
	}

	/************************************************************************/
	Scope& Scope::AppendScope(const string& str, Scope* scope)
	{
		if (str.empty())
		{
			throw invalid_argument("The key shouldn't be an empty string.");
		}

		auto hashIt = mTable.Find(str);
		if (hashIt != mTable.end())
		{
			if ((*hashIt).second.Type() != Datum::DatumType::Table)
			{
				throw exception("Cannot append scope to a non scope datum!");
			}

			// replace the existent scope with a new one
			if (!scope)
			{
				scope = new Scope();
			}
			scope->mParentScope = this;
			mTable[str].PushBack(scope);

			return *scope;
		}
		else
		{
			// add a new scope
			if (!scope)
			{
				scope = new Scope();
			}
			scope->mParentScope = this;
			Datum& dat = Append(str);
			dat.PushBack(scope);

			return *scope;
		}
	}

	/************************************************************************/
	bool Scope::RemoveScope(const Scope* scope)
	{
		bool removed = false;
		// loop through the hashmap
		for (HashMapIterator it = mTable.begin(); it != mTable.end(); ++it)
		{
			if ((*it).second.Type() == Datum::DatumType::Table)
			{
				// loop through the scope datum
				for (uint32_t i = 0; i < (*it).second.Size(); i++)
				{
					if ((*it).second[i] == *scope)
					{
						(*it).second.Remove(i, false);
						removed = true;
						break;
					}
				}
			}
			if (removed)
			{
				break;
			}
		}
		return removed;
	}
}
