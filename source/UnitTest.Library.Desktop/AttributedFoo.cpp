#include "pch.h"
#include "RTTI.h"
#include "Scope.h"
#include "AttributedFoo.h"

using namespace std;
using namespace glm;
using namespace Library;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(AttributedFoo);

	/************************************************************************/
	AttributedFoo::AttributedFoo() :
		Attributed(), mExternalInteger(0), mExternalFloat(0.0f), mExternalVector(vec4()), mExternalMatrix(mat4x4()), mExternalRTTI(this),
		mExternalIntegerArray{ 0 }, mExternalFloatArray{ 0.0f }, mExternalVectorArray{ vec4() }, mExternalMatrixArray{ mat4x4() }, mExternalRTTIArray{ this },
		mNestedScope(new Scope())
	{
		InitPrescribedAttributes();
	}

	/************************************************************************/
	AttributedFoo::AttributedFoo(const AttributedFoo & rhs) :
		Attributed(rhs)
	{
	}

	/************************************************************************/
	AttributedFoo & AttributedFoo::operator=(const AttributedFoo & rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(rhs);
		}

		return *this;
	}

	/************************************************************************/
	void AttributedFoo::InitPrescribedAttributes()
	{
		AddExternalAttribute("mExternalInteger", &mExternalInteger, 1);
		AddExternalAttribute("mExternalFloat", &mExternalFloat, 1);
		AddExternalAttribute("mExternalVector", &mExternalVector, 1);
		AddExternalAttribute("mExternalMatrix", &mExternalMatrix, 1);
		AddExternalAttribute("mExternalString", &mExternalString, 1);
		auto pointer = static_cast<RTTI*>(mExternalRTTI);
		AddExternalAttribute("mExternalRTTI", &pointer, 1);

		AddExternalAttribute("mExternalIntegerArray", mExternalIntegerArray, ARRAY_SIZE);
		AddExternalAttribute("mExternalFloatArray", mExternalFloatArray, ARRAY_SIZE);
		AddExternalAttribute("mExternalVectorArray", mExternalVectorArray, ARRAY_SIZE);
		AddExternalAttribute("mExternalMatrixArray", mExternalMatrixArray, ARRAY_SIZE);
		AddExternalAttribute("mExternalStringArray", mExternalStringArray, ARRAY_SIZE);

		AddInternalAttribute("InternalInteger", 0, 1);
		AddInternalAttribute("InternalFloat", 0.0f, 1);
		AddInternalAttribute("InternalVector", vec4(), 1);
		AddInternalAttribute("InternalMatrix", mat4x4(), 1);
		AddInternalAttribute("InternalString", "", 1);
		AddInternalAttribute("InternalRTTI", this, 1);

		AddInternalAttribute("InternalIntegerArray", 0, ARRAY_SIZE);
		AddInternalAttribute("InternalFloatArray", 0.0f, ARRAY_SIZE);
		AddInternalAttribute("InternalVectorArray", vec4(), ARRAY_SIZE);
		AddInternalAttribute("InternalMatrixArray", mat4x4(), ARRAY_SIZE);
		AddInternalAttribute("InternalStringArray", "", ARRAY_SIZE);
		AddInternalAttribute("InternalRTTIArray", this, ARRAY_SIZE);

		AddNestedScopeAttribute("NestedScope");
		AddNestedScopeAttribute("NestedScope", mNestedScope);

		AddEmptyNestedScopeAttribute("EmptyNestedScope");
	}

}