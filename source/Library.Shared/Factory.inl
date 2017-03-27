
#include "Factory.h"

namespace Library
{
	template<typename AbstractProductT>
	typename HashMap<std::string, Factory<AbstractProductT>*> Factory<AbstractProductT>::sFactoryMap = HashMap<std::string, Factory<AbstractProductT>*>();

	/************************************************************************/
	template<typename AbstractProductT>
	Factory<AbstractProductT>* Factory<AbstractProductT>::Find(const std::string & className)
	{
		MapIterator iterator = sFactoryMap.Find(className);

		if (iterator == end())
		{
			return nullptr;
		}
		return (*iterator).second;
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
		// urgent think about if the factory already exists
		sFactoryMap.Insert(std::pair<std::string, Factory<AbstractProductT>*>(factory.ClassName(), &factory));
	}

	/************************************************************************/
	template<typename AbstractProductT>
	void Factory<AbstractProductT>::Remove(Factory<AbstractProductT>& factory)
	{
		sFactoryMap.Remove(factory.ClassName());
	}

	/************************************************************************/
	template<typename AbstractProductT>
	typename HashMap<std::string, Factory<AbstractProductT>*>::Iterator Factory<AbstractProductT>::begin()
	{
		return sFactoryMap.begin();
	}

	/************************************************************************/
	template<typename AbstractProductT>
	typename HashMap<std::string, Factory<AbstractProductT>*>::Iterator Factory<AbstractProductT>::end()
	{
		return sFactoryMap.end();
	}
}