#pragma once
#include "Action.h"
#include "Factory.h"

namespace UnitTestLibraryDesktop
{
	class TestAction final : public Library::Action
	{
		RTTI_DECLARATIONS(TestAction, Library::Action);

	public:

		TestAction();
		~TestAction() = default;
		TestAction(const TestAction& rhs) = delete;
		TestAction& operator=(const TestAction& rhs) = delete;

		virtual void Update(Library::WorldState& worldState) override;


		std::int32_t GetNumber() const;
		void SetNumber(const std::int32_t number);

	protected:

		virtual void InitPrescribedAttributes() override;

	private:

		std::int32_t mNumber;

	};

	ActionConcreteFactory(TestAction);
}
