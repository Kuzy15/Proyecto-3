#include "EntityFactory.h"


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


//Depend on the type of the entity, we call one or other function passing it the class 
Entity* EntityFactory::createEntity(E_TYPE typE, int classE, GameScene* s ){

	
	Entity* e;											//The pointer to the entity who will be returned
	std::string idS = to_string(_idCount++);			//The unique id	

	switch (typE)
	{
	case ET_GOD:
		e = createGod((E_GOD)classE, idS, s);
		break;
	case ET_BULLET:
		e = createBullet((E_BULLET)classE, idS, s);
	default:
		break;
	}

	return e;
}

//The create god methods search in the map the func that returns the entity that E_GOD variable indicates
Entity* EntityFactory::createGod(E_GOD cE, std::string id, GameScene* s){
	
	createGodFunc c = _createGodFuncs.at(cE);
	Entity* debug = c(id, s);
	return debug;
}

//The create bullet methods return a new bullet
Entity* EntityFactory::createBullet(E_BULLET cE, std::string id, GameScene* s){

	creteBulletFunc c = _createBulletFuncs.at(cE);
	return c(id, s);
}