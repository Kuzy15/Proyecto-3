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

	//Base method wich returns a new entity depending on the type and the class. For example: TYPE = GOD, CLASS = RA.
	Entity* createEntity(E_TYPE typeE, int classE, GameScene* s);
	
private:
	EntityFactory();

	//Data structures that stores the diferents functions which creates entities
	map<E_GOD, createGodFunc> _createGodFuncs;


	static EntityFactory* _instance;	//The unique instance of the class
	unsigned int _idCount;				//The id variable to put the diferent id to new entities 

	//Methods for create gods and define their behaviour (Components, etc)
	Entity* createGod(E_GOD god ,std::string id, GameScene* s);

};

#endif