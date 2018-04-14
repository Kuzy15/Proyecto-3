#ifndef ENTITY_FACTORY_ENTITIES
#define ENTITY_FACTORY_ENTITIES

/*
	This header isn't a class, it contains all the enums and functions declarations 
	needed to create new entities. The definitions for the functions are in the 
	.cpp with the same name
*/

#include <string>

class Entity;
class gameScene;

//Typedefs to name functions types for entity creation
#pragma region Typedef functions
typedef Entity*(*createGodFunc)(std::string id, gameScene* s);

#pragma endregion

//Enum definitions for all entities
#pragma region Enums
typedef enum E_TYPE {ET_GOD = 0, ET_BULLET = 1};
typedef enum E_GOD { EG_RA = 0 };
typedef enum E_BULLET { EB_RA = 0 };
#pragma endregion

//Creating methods definitions for all entities
#pragma region Methods definition 
Entity* createGodRa(std::string id, gameScene* s);

#pragma endregion

#endif