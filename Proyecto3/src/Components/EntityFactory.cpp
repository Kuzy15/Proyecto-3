#include "EntityFactory.h"


EntityFactory* EntityFactory::_instance = nullptr;

EntityFactory::EntityFactory()
{
	_idCount = 0;
}


EntityFactory::~EntityFactory()
{

}

EntityFactory& EntityFactory::getInstance(){
	if (EntityFactory::_instance == nullptr) EntityFactory::_instance = new EntityFactory();
	return *EntityFactory::_instance;
}