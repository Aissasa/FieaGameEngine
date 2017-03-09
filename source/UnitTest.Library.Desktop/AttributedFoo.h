#pragma once

namespace UnitTestLibraryDesktop
{

	class AttributedFoo : public Library::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed);

	public:

		AttributedFoo();
		~AttributedFoo() = default;
		AttributedFoo(const AttributedFoo & rhs);
		AttributedFoo& operator=(const AttributedFoo& rhs);

	protected:

		void InitPrescribedAttributes() override;

	private:

		void UpdateExternalAttributes();

		const static std::uint32_t ARRAY_SIZE = 3;

		std::int32_t mExternalInteger;
		std::float_t mExternalFloat;
		glm::vec4 mExternalVector;
		glm::mat4x4 mExternalMatrix;
		std::string mExternalString;
		AttributedFoo* mExternalRTTI;

		std::int32_t mExternalIntegerArray[ARRAY_SIZE];
		std::float_t mExternalFloatArray[ARRAY_SIZE];
		glm::vec4 mExternalVectorArray[ARRAY_SIZE];
		glm::mat4x4 mExternalMatrixArray[ARRAY_SIZE];
		std::string mExternalStringArray[ARRAY_SIZE];
		AttributedFoo* mExternalRTTIArray[ARRAY_SIZE];

		Scope* mNestedScope;

	};
}