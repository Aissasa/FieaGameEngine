#pragma once

#include <cstdint>
#include <cstring>
#include <exception>
#include "HashMap.h"

namespace Library
{
	template<typename AbstractProductT>
	class Factory
	{
	public:

		typename typedef HashMap<std::string, Factory<AbstractProductT>*>::Iterator MapIterator;

		static Factory<AbstractProductT>* Find(const std::string& className);

		static AbstractProductT* Create(const std::string& className);

		virtual AbstractProductT* Create() = 0;

		virtual std::string ClassName() = 0;

		static MapIterator begin();

		static MapIterator end();

	protected:

		static void Add(Factory<AbstractProductT>& factory);

		static void Remove(Factory<AbstractProductT>& factory);

	private:

		static HashMap<std::string, Factory<AbstractProductT>*> sFactoryMap;
	};

#define ConcreteFactory( AbstractProductT, ConcreteProductT )                    \
    class ConcreteProductT##Factory : public Library::Factory<AbstractProductT>  \
    {                                                                            \
        public:                                                                  \
             ConcreteProductT##Factory()  { Add( *this ) ; }                     \
	                                                                             \
            ~ConcreteProductT##Factory()  { Remove( *this ) ; }                  \
            	                                                                 \
            virtual std::string ClassName() override                             \
			{                                                                    \
	             return std::string(#ConcreteProductT);                          \
	        }                                                                    \
                                                                                 \
            virtual AbstractProductT* Create() override                          \
            {                                                                    \
                AbstractProductT * product = new ConcreteProductT() ;            \
                assert( product != nullptr ) ;                                   \
                return product ;                                                 \
            }                                                                    \
    };
}

#include "Factory.inl"