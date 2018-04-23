#include "EntityFactory.h"
#include "DebugNew.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

EntityFactory* EntityFactory::_instance = nullptr;

//The constructor should set up the maps for all the functions
EntityFactory::EntityFactory()
{
	_idCount = 0;

	//Define all create Entity methods
	//Gods
	_createGodFuncs.emplace(EG_RA, &createGodRa);

	//Bullets
	_createBulletFuncs.emplace(EB_RA, &createBulletRa);
}


EntityFactory::~EntityFactory()
{

}

EntityFactory& EntityFactory::getInstance(){
	if (EntityFactory::_instance == nullptr) EntityFactory::_instance = new EntityFactory();
	return *EntityFactory::_instance;
}




//The create god methods search in the map the func that returns the entity that E_GOD variable indicates
Entity* EntityFactory::createGod(E_GOD cE,  GameScene* s, Ogre::Vector3 iniPos){
	
	newEntity();

	createGodFunc c = _createGodFuncs.at(cE);
	return c(to_string(_idCount), s, iniPos);
	
}

//The create bullet methods return a new bullet
Entity* EntityFactory::createBullet(E_BULLET cE,  GameScene* s, Ogre::Vector3 iniPos, float angle){

	newEntity();

	creteBulletFunc c = _createBulletFuncs.at(cE);
	return c(to_string(_idCount), s, iniPos, angle);
}