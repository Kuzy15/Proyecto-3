#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <map>
#include <string>
#include "EF_Entities.h"

using namespace std;

class EntityFactory
{
public:
	~EntityFactory();
	static EntityFactory& getInstance();
	static void resetInstance();

	//Methods for create gods and define their behaviour (Components, etc)
	Entity* createGod(E_GOD god, GameScene* s, Ogre::Vector3 iniPos, int controllerId);
	Entity* createBullet(E_BULLET bullet, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage);
	std::vector<Entity*>* createStage(E_STAGE stage, GameScene* s);
	inline void newEntity(){ _idCount++ ; }

private:
	EntityFactory();

	//Data structures that stores the diferents functions which creates entities
	map<E_GOD, createGodFunc> _createGodFuncs;
	map<E_BULLET, creteBulletFunc> _createBulletFuncs;
	map<E_STAGE, createStageFunc> _createStageFuncs;


	static EntityFactory* _instance;	//The unique instance of the class
	unsigned int _idCount;				//The id variable to put the diferent id to new entities 

	

};

#endif