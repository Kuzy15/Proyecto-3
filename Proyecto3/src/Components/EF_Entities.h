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
class GameComponent;


//Typedefs to name functions types for entity creation
#pragma region Typedef functions
typedef Entity*(*createGodFunc)(std::string id, GameScene* s, Ogre::Vector3 iniPos, int controllerId);
typedef Entity*(*creteBulletFunc)(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage, std::string iD);
typedef std::vector<Entity*>*(*createStageFunc)(GameScene* s);
typedef GameComponent*(*createAbilityFunc)(Entity* father, int id);


#pragma endregion

//Enum definitions for all entities
#pragma region Enums
typedef enum E_GOD { EG_RA = 0, EG_AHPUCH = 1, EG_ZEUS = 2, EG_HACHIMAN = 3 };
typedef enum E_BULLET { EB_RA = 0, EB_AHPUCH = 1, EB_ZEUS = 2, EB_HACHIMAN = 3};
typedef enum E_STAGE {ES_TEMPLE = 0, ES_ISLANDS = 1};
#pragma endregion

//Creating methods definitions for all entities
#pragma region Gods 
Entity* createGodRa(std::string id, GameScene* s, Ogre::Vector3 iniPos, int controllerId);
Entity* createGodAhPuch(std::string id, GameScene* s, Ogre::Vector3 iniPos, int controllerId);
Entity* createGodZeus(std::string id, GameScene* s, Ogre::Vector3 iniPos, int controllerId);
Entity* createGodHachiman(std::string id, GameScene* s, Ogre::Vector3 iniPos, int controllerId);

#pragma endregion

#pragma region Bullets
Entity* createBulletRa(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage, std::string iD);
Entity* createBulletAhPuch(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage, std::string iD);
Entity* createBulletZeus(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage, std::string iD);
Entity* createBulletHachiman(std::string id, GameScene* s, Ogre::Vector3 iniPos, float angle, float damage, std::string iD);
#pragma endregion

#pragma region Stages
std::vector<Entity*>* createStageTemple(GameScene* s);
std::vector<Entity*>* createStageIslands(GameScene* s);
#pragma endregion 

#pragma region Abilities
GameComponent* createAbility(Entity* father, int id);
#pragma endregion
#endif