#include "Components.h"
#include "Entity.h"


//Component base class, made for inheriting.
//It implements basic behaviours like gets, sets 
//and message sending
#pragma region gameComponent
gameComponent::gameComponent(std::string id, Entity * father): _id(id), pEnt(father){

}

std::string gameComponent::getID(){
	return _id;
}
bool gameComponent::getActive(){
	return _active;
}
void gameComponent::setActive(bool nw){
	_active = nw;
}
void gameComponent::sendMessage(Message * m){
	pEnt->getMessage(m);
}

#pragma endregion