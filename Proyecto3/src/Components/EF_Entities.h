#ifndef ENTITY_FACTORY_ENTITIES
#define ENTITY_FACTORY_ENTITIES

/*
	This header isn't a class, it contains all the enums and functions declarations 
	needed to create new entities. The definitions for the functions are in the 
	.cpp with the same name
*/

#include <string>
#include <Ogre.h>


class Entity;
class GameScene;


//Typedefs to name functions types for entity creation
#pragma region Typedef functions
typedef Entity*(*createGodFunc)(std::string id, GameScene* s, Ogre::Vector3 iniPos);
typedef Entity*(*creteBulletFunc)(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle);

#pragma endregion

//Enum definitions for all entities
#pragma region Enums
typedef enum E_GOD { EG_RA = 0 };
typedef enum E_BULLET { EB_RA = 0 };
#pragma endregion

//Creating methods definitions for all entities
#pragma region Gods 
Entity* createGodRa(std::string id, GameScene* s, Ogre::Vector3 iniPos);

#pragma endregion

#pragma region Bullets
Entity* createBulletRa(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle);
#pragma endregion
#endif