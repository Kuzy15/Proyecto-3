#ifndef ENTITY_FACTORY_H
#define ENTITY_FACTORY_H

#include <map>
#include <string>


using namespace std;

class Entity;





class EntityFactory
{
public:
	~EntityFactory();

	static EntityFactory& getInstance();

	Entity* createEntityGod(E_GOD g);
	Entity* createEntityBullet(E_BULLET b);





private:
	EntityFactory();

	map<string, void*> createFuncs;

	static EntityFactory* _instance;
	static unsigned int _idCount;





	//Methods for create gods and define their behaviour (Components, etc)
	Entity* createGodRa();
	

};

#endif