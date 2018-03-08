#include "Components.h"
#include "Entity.h"



//Debug 
#ifdef _DEBUG
#include <iostream>
#endif



//Component base class, made for inheriting.
//It implements basic behaviours like gets, sets 
//and message sending
#pragma region gameComponent
gameComponent::gameComponent(std::string id, Entity * father): _id(id), pEnt(father)
{

}
gameComponent::~gameComponent(){

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

//DEBUG COMPONENT.
//PRINTS A STRING WHEN TICKED
#pragma region stringComponent
stringComponent::stringComponent(std::string id, Entity * fath) : gameComponent(id, fath), whatSay("HOLA, soy la entidad con id" + id){

}
stringComponent::~stringComponent(){

}
void stringComponent::getMessage(Message * m){
	std::cout << "Component " << _id << "Received message!" << std::endl;
}
void stringComponent::tick(float delta){
	std::cout << _id << " got a TICK!\n";
}

#pragma endregion