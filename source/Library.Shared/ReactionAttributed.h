#pragma once

#include "Reaction.h"

namespace Library
{
	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);

	public:

		ReactionAttributed(const std::string& subType = "");
		virtual ~ReactionAttributed() = default;
		ReactionAttributed(const ReactionAttributed & rhs) = delete;
		ReactionAttributed& operator=(const ReactionAttributed& rhs) = delete;

		virtual void Notify(EventPublisher& eventPublisher) override;


		std::string GetSubType() const;
		void SetSubType(const std::string& subType);
	protected:

		virtual void Update(WorldState& worldState) override;
		virtual void InitPrescribedAttributes() override;

	private:

		static const std::string SUB_TYPE_KEY;

		std::string mSubType;
	};

}
