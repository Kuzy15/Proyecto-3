#include "EntityFactory.h"
#include "DebugNew.h"
#include <iostream>
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
	_createGodFuncs.emplace(EG_AHPUCH, &createGodAhPuch);
	_createGodFuncs.emplace(EG_ZEUS, &createGodZeus);
	_createGodFuncs.emplace(EG_HACHIMAN, &createGodHachiman);

	

	//Bullets
	_createBulletFuncs.emplace(EB_RA, &createBulletRa);
	_createBulletFuncs.emplace(EB_AHPUCH, &createBulletAhPuch);
	_createBulletFuncs.emplace(EB_ZEUS, &createBulletZeus);
	_createBulletFuncs.emplace(EB_HACHIMAN, &createBulletHachiman);

	//Stages
	_createStageFuncs.emplace(ES_TEMPLE, &createStageTemple);
	_createStageFuncs.emplace(ES_ISLANDS, &createStageIslands);

	//Abilities
	_createAbilityFuncs.emplace(CMP_PASSIVE_VIDAR, &createAbilityVidar);
	_createAbilityFuncs.emplace(CMP_PASSIVE_HADES, &createAbilityHades);
	_createAbilityFuncs.emplace(CMP_PASSIVE_ULL, &createAbilityUll);
	_createAbilityFuncs.emplace(CMP_PASSIVE_VALI, &createAbilityVali);
	_createAbilityFuncs.emplace(CMP_PASSIVE_HERMES, &createAbilityHermes);
	_createAbilityFuncs.emplace(CMP_PASSIVE_SYN, &createAbilitySyn);
	_createAbilityFuncs.emplace(CMP_HERA_RUNE, &createAbilityHeraRune);
	_createAbilityFuncs.emplace(CMP_SHU_HEADDRESS, &createAbilityShuHeaddress);
	_createAbilityFuncs.emplace(CMP_JONSU_MOON, &createAbilityJonsuMoon);
	_createAbilityFuncs.emplace(CMP_KHEPRI_BEETLE, &createAbilityKhepriBeetle);
	_createAbilityFuncs.emplace(CMP_HERIS_MARK, &createAbilityHerisMark);
}


EntityFactory::~EntityFactory()
{
	_createBulletFuncs.clear();
	_createGodFuncs.clear();
	_createStageFuncs.clear();

	
}

void EntityFactory::resetInstance(){
	delete EntityFactory::_instance;
	_instance == nullptr;
}


EntityFactory& EntityFactory::getInstance(){
	if (EntityFactory::_instance == nullptr) EntityFactory::_instance = new EntityFactory();
	return *EntityFactory::_instance;
}




//The create god methods search in the map the func that returns the entity that E_GOD variable indicates
Entity* EntityFactory::createGod(E_GOD cE,  GameScene* s, Ogre::Vector3 iniPos, int controllerId){
	
	newEntity();

	
	createGodFunc c = _createGodFuncs.at(cE);
	if (c != nullptr)
		return c(to_string(_idCount), s, iniPos, controllerId);

#ifdef _DEBUG
	std::cout << "No existe esa funcion de creacion" << std::endl;
#endif
	
}

//The create bullet methods return a new bullet
Entity* EntityFactory::createBullet(E_BULLET cE,  GameScene* s, Ogre::Vector3 iniPos, float angle, float damage, std::string iD){

	newEntity();
	creteBulletFunc c = _createBulletFuncs.at(cE);
	if (c != nullptr)
		return c(to_string(_idCount), s, iniPos, angle, damage, iD);

#ifdef _DEBUG
	std::cout << "No existe esa funcion de creacion" << std::endl;
#endif
}

std::vector<Entity*>* EntityFactory::createStage(E_STAGE cE, GameScene* s){


	createStageFunc c = _createStageFuncs.at(cE);
	if (c != nullptr)
		return c(s);
#ifdef _DEBUG
	std::cout << "No existe esa funcion de creacion" << std::endl;
#endif
}

GameComponent* EntityFactory::createAbility(ComponentType cE, Entity* father, int id){

	createAbilityFunc c = _createAbilityFuncs.at(cE);
	if (c != nullptr)
		return c(father, id);

#ifdef _DEBUG
	std::cout << "No existe esa funcion de creacion" << std::endl;
#endif


};