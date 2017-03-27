
#include "Factory.h"

namespace Library
{
	/************************************************************************/
	template<typename AbstractProductT>
	Factory<AbstractProductT>* Factory<AbstractProductT>::Find(const std::string & className)
	{
		MapIterator iterator = sFactoryMap.Find(className);

		if (iterator == end())
		{
			return nullptr;
		}
		return *iterator;
	}

	/************************************************************************/
	template<typename AbstractProductT>
	AbstractProductT* Factory<AbstractProductT>::Create(const std::string & className)
	{
		Factory<AbstractProductT>* factory = Find(className);

		if (!factory)
		{
			return nullptr;
		}

		return factory->Create();
	}

	/************************************************************************/
	template<typename AbstractProductT>
	void Factory<AbstractProductT>::Add(Factory<AbstractProductT>& factory)
	{
		// todo think about if the factory already exists
		sFactoryMap.Insert(&factory);
	}

	/************************************************************************/
	template<typename AbstractProductT>
	void Factory<AbstractProductT>::Remove(Factory<AbstractProductT>& factory)
	{
		sFactoryMap.Remove(factory.ClassName());
	}

	/************************************************************************/
	template<typename AbstractProductT>
	MapIterator Factory<AbstractProductT>::begin()
	{
		return sFactoryMap.begin();
	}

	/************************************************************************/
	template<typename AbstractProductT>
	MapIterator Factory<AbstractProductT>::end()
	{
		return sFactoryMap.end();
	}
}