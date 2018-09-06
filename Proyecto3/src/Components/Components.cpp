#include "Components.h"
#include "Entity.h"
#include <OgreEntity.h>
#include <OgreSceneNode.h>
//#include "Messages.h"
#include "Game.h"
#include "EntityFactory.h"
#include "DebugNew.h"
#include "Scenes.h"


/*#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgreFontManager.h>
#include <OgreOverlaySystem.h>
*/
//Debug
#ifdef _DEBUG
#include <iostream>
#define new DEBUG_NEW
#endif





std::string compToString(ComponentType t, int &type){

	switch (t){
	case CMP_PASSIVE_HADES:
		type = 1;
		return "PASSIVE_HADES";
	case CMP_PASSIVE_HERMES:
		type = 1;
		return "PASSIVE_HERMES";
	case CMP_PASSIVE_SYN:
		type = 1;
		return "PASSIVE_SYN";
	case CMP_PASSIVE_ULL:
		type = 1;
		return "PASSIVE_ULL";
	case CMP_PASSIVE_VALI:
		type = 1;
		return "PASSIVE_VALI";
	case CMP_PASSIVE_VIDAR:
		type = 1;
		return "PASSIVE_VIDAR";
	case CMP_HERA_RUNE:
		type = 0;
		return "HERA_RUNE";
	case CMP_KHEPRI_BEETLE:
		type = 0;
		return "KHEPRI_BEETLE";
	case CMP_JONSU_MOON:
		type = 0;
		return "JONSU_MOON";
	case CMP_HERIS_MARK:
		type = 0;
		return "HERIS_MARK";
	case CMP_SHU_HEADDRESS:
		type = 0;
		return "SHU_HEADDRESS";
	default:
		break;
	}

};


/*-------------------------BASIC GAME COMPONENT------------------------------------*/
//Component base class, made for inheriting.
//It implements basic behaviours like gets, sets
//and message sending
#pragma region gameComponent
GameComponent::GameComponent(ComponentType componentType, Entity * father) : _componentType(componentType), pEnt(father)
{

}
GameComponent::~GameComponent(){

}
ComponentType GameComponent::getComponentType(){
	return _componentType;
}
bool GameComponent::getActive(){
	return _active;
}
void GameComponent::setActive(bool nw){
	_active = nw;
}
void GameComponent::sendMessage(Message * m){
	pEnt->getMessage(m);
}

#pragma endregion

/*-------------------------DEBUG COMPONENTS------------------------------------*/
//PRINTS A STRING WHEN RECEIVEING A STRING_MESSAGE
#pragma region stringComponent
CString::CString(Entity * fath) : GameComponent(CMP_STRING, fath), whatSay("HOLA, soy el componente basico"){

}
CString::~CString(){

}
void CString::getMessage(Message * m){
#ifdef _DEBUG
	//if (m->getType() == STRING_MSG)std::cout << "MESSAGE SAID: " << static_cast<stringMessage*>(m)->getText() << std::endl;

#endif
}
void CString::tick(float delta){
#ifdef _DEBUG
#endif
}

#pragma endregion

#pragma region messageSendComponent
CMessageSend::CMessageSend(Entity * father):GameComponent(CMP_MESSAGE_SEND, father) {
	i = 0;
}
CMessageSend::~CMessageSend() {
}
void CMessageSend::tick(float delta) {

}
void CMessageSend::getMessage(Message * m) {

}

#pragma endregion



