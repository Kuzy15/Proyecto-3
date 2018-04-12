#include "EF_Entities.h"
#include "Entity.h"
#include "Components.h"

//-------------------------------------------    GODS    -------------------------------------------//
#pragma region Gods
//Ra
Entity* createGodRa(std::string id, gameScene* s){
	Entity * test1 = new Entity(id, s);
	test1->addComponent(new messageSendComponent(test1));
	
	return test1;;
}

#pragma endregion

