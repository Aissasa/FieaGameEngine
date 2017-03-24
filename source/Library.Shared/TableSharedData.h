#pragma once

#include "RTTI.h"
#include "XmlParseMaster.h"
#include "Scope.h"

namespace Library
{
	class TableSharedData final : public XmlParseMaster::SharedData
	{
		RTTI_DECLARATIONS(TableSharedData, XmlParseMaster::SharedData)

	public:

		TableSharedData(XmlParseMaster* xmlParseMaster = nullptr);
		~TableSharedData();
		TableSharedData(const TableSharedData & rhs) = delete;
		TableSharedData& operator=(const TableSharedData& rhs) = delete;

		virtual XmlParseMaster::SharedData* Clone() override;

		virtual void Initialize() override;

		Scope& GetScope() const;
		void SetScope(Scope& scope);

		bool IsParsingElement;

	private:

		Scope* mScope;
	};

}