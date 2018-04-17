#include "EF_Entities.h"
#include "Entity.h"
#include "Components.h"

//-------------------------------------------    GODS    -------------------------------------------//
#pragma region Gods
//Ra
Entity* createGodRa(std::string id, GameScene* s){
	Entity * test1 = new Entity(id, s);
	test1->addComponent(new CMessageSend(test1));
	
	return test1;;
}

#pragma endregion

