#pragma once

#include "Attributed.h"

namespace Library
{
	class WorldState;

	class EventMessageAttributed : public Attributed
	{	
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);

	public:

		EventMessageAttributed(const std::string& subType = "", WorldState* worldState = nullptr);
		~EventMessageAttributed() = default;
		EventMessageAttributed(const EventMessageAttributed & rhs) = default;
		EventMessageAttributed& operator=(const EventMessageAttributed& rhs) = default;


		std::string GetSubType() const;
		void SetSubType(const std::string& subType);
		WorldState* GetWorldState() const;
		void SetWorldState(WorldState& worldState);

	protected:

		virtual void InitPrescribedAttributes() override;

	private:

		static const std::string SUB_TYPE_KEY;
		static const std::string WORLD_KEY;

		std::string mSubType;
		WorldState* mWorldState;

	};
}