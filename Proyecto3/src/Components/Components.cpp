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
gameComponent::gameComponent(componentType id, Entity * father): _id(id), pEnt(father)
{

}
gameComponent::~gameComponent(){

}
componentType gameComponent::getID(){
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
stringComponent::stringComponent(Entity * fath) : gameComponent(Component_Basic, fath), whatSay("HOLA, soy el componente basico"){

}
stringComponent::~stringComponent(){

}
void stringComponent::getMessage(Message * m){
#ifdef _DEBUG

	std::cout << "Basic Component from entity " << pEnt->getID() << " received message!" << std::endl;
#endif
}
void stringComponent::tick(float delta){
#ifdef _DEBUG

	std::cout << pEnt->getID() << " got a TICK!\n";
	std::cout << whatSay << std::endl;
#endif
}

#pragma endregion

//Render Component class. Father to every
//other render component.
#pragma region renderComponent

renderComponent::renderComponent(componentType t, Entity * father, Ogre::SceneManager * scnM)
	: gameComponent(t, father), pSceneMgr(scnM)
{
	pOgreSceneNode = pSceneMgr->getRootSceneNode()->createChildSceneNode();
}
renderComponent::~renderComponent(){
	pSceneMgr->destroySceneNode(pOgreSceneNode);
}

#pragma endregion


//Mesh Render component.
//Takes a string with the name of the mesh to render
//and renders it.
#pragma meshRenderComponent






#pragma endregion