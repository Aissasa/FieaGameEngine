#pragma once

#include "pch.h"
#include "Factory.h"
#include "AbstractProductTest.h"
#include "Product1.h"
#include "Product2.h"
#include "Bar.h"

namespace UnitTestLibraryDesktop
{
	ConcreteFactory(AbstractProductTest, Product1)
	
	ConcreteFactory(AbstractProductTest, Product2)

	ConcreteFactory(Bar, Bar)
}