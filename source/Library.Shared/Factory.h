#pragma once

#include "HashMap.h"

namespace Library
{
	/** Templated Abstract Factory class.
	* This class is meant to be implemented by concrete factories. Each of these latter must be associated with a family of objects.
	*/
	template<typename AbstractProductT>
	class Factory
	{
	public:

		typedef typename HashMap<std::string, Factory<AbstractProductT>*>::Iterator MapIterator;


		/** Default destructor.
		 *  
		 */
		virtual ~Factory() = default;

		/** Finds a concrete factory by name.
		* If the factory does not exist, a nullptr is returned.
		* @param className: Name of the factory to look for.
		* @return Found factory pointer.
		*/
		static Factory<AbstractProductT>* Find(const std::string& className);

		/** Creates an instance of the product linked to the specified factory.
		* If the factory does not exist, a nullptr is returned.
		* @param className: Name of the factory to use for creating the object.
		* @return Created product instance pointer.
		*/
		static AbstractProductT* Create(const std::string& className);

		/** Creates an instance of the product linked to the current factory.
		* If the factory does not exist, a nullptr is returned.
		* This method is pure virtual.
		* @return Created product instance pointer.
		*/
		virtual AbstractProductT* Create() const = 0;

		/** Returns the name of the class the factory represents.
		* This method is pure virtual.
		* @return Name of the class the factory represents.
		*/
		virtual std::string ClassName() const = 0;

		/** Returns the begin iterator of the map of factories.
		* @return Begin iterator of the map of factories
		*/
		static MapIterator begin();

		/** Returns the end iterator of the map of factories.
		* @return End iterator of the map of factories
		*/
		static MapIterator end();

	protected:

		/** Adds a factory to the map of factories of the current family of objects.
		* @param factory: The factory to add.
		*/
		static void Add(Factory<AbstractProductT>& factory);

		/** Removes a factory of the map of factories of the current family of objects.
		* @param factory: The factory to remove.
		*/
		static void Remove(Factory<AbstractProductT>& factory);

	private:

		static HashMap<std::string, Factory<AbstractProductT>*> sFactoryMap;
	};

#define ConcreteFactory( AbstractProductT, ConcreteProductT )                    \
    class ConcreteProductT##Factory final : public Library::Factory<AbstractProductT>  \
    {                                                                            \
        public:                                                                  \
             ConcreteProductT##Factory()  { Add( *this ) ; }                     \
	                                                                             \
            ~ConcreteProductT##Factory()  { Remove( *this ) ; }                  \
            	                                                                 \
            virtual std::string ClassName() const override                       \
			{                                                                    \
	             return std::string(#ConcreteProductT);                          \
	        }                                                                    \
                                                                                 \
            virtual AbstractProductT* Create() const override                    \
            {                                                                    \
                AbstractProductT * product = new ConcreteProductT() ;            \
                assert( product != nullptr ) ;                                   \
                return product ;                                                 \
            }                                                                    \
    };
}

#include "Factory.inl"