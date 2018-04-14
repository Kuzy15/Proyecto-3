#include "Messages.h"
#include "Entity.h"
#include "Components.h"

//Basic Message Class implementation.
#pragma region basic Message 
Message::Message(msgType t, msgDest dst, std::string emmiter): _messageType(t), _messageDestination(dst), _emmiter(emmiter)
{
}
Message::~Message()
{
}
msgType Message::getType() {
	return _messageType;
}
msgDest Message::getDestination() {
	return _messageDestination;
}
std::string Message::getEmmiter() {
	return _emmiter;
}
#pragma endregion

#pragma region stringMessage
stringMessage::stringMessage(std::string t, msgDest dest, std::string emmiter): Message(STRING_MSG,BROADCAST,emmiter), _text(t) {

}
stringMessage::~stringMessage() {

}

std::string stringMessage::getText() {
	return _text;
}
#pragma endregion

//Input Message implementation 
#pragma region InputStateMessage
InputStateMessage::InputStateMessage(int i, msgDest d, std::string emmiter) :Message(INPUT_STATE_MSG, BROADCAST, emmiter), _controllerId(i){

	
}

InputStateMessage::~InputStateMessage(){ 

}

CInputState& InputStateMessage::getCInputState(){
	_cState.nEvents++;
	return _cState;
}
#pragma endregion

#pragma region ControllerStateMessage
ControllerStateMessage::ControllerStateMessage(msgDest d, std::string emmiter, int id, int action) :Message(CONTROLLER_STATE_MSG, SCENE, emmiter){
	_controllerId = id;
	_action = action;
}

ControllerStateMessage::~ControllerStateMessage(){

}



#pragma endregion


#pragma region UpdateTransform Message 

UpdateTransformMessage::UpdateTransformMessage(Ogre::Vector3 newPos, Ogre::Quaternion newQuaternion, std::string emmiter):Message(ENTITY_UPDATETRANSFORM, ENTITY,emmiter), _nPos(newPos), _nQuat(newQuaternion){

};
UpdateTransformMessage::~UpdateTransformMessage() {

}
Ogre::Vector3 UpdateTransformMessage::GetPos() { return _nPos; }
Ogre::Quaternion UpdateTransformMessage::GetQuat() { return _nQuat; }

#pragma endregion

#pragma region MessageInputPlayer

MessagePlayerInput::MessagePlayerInput(int i, std::string emmiter): Message(MSG_PLAYER_MOVE_X,ENTITY,emmiter), _id(i){}
MessagePlayerInput::~MessagePlayerInput(){}
#pragma endregion

#pragma region MessagePlayerMoveX
MessagePlayerMoveX::MessagePlayerMoveX(float v, int i, std::string emmiter) : MessagePlayerInput(i, emmiter),_value(v){}
MessagePlayerMoveX::~MessagePlayerMoveX(){};

#pragma endregion 

#pragma region MessageCollision

MessageCollision::MessageCollision(uint16_t me, uint16_t contact, std::string emmiter) : Message(MSG_COLLISION, ENTITY, emmiter),
	_myCategory(me), _contactMask(contact){}
MessageCollision::~MessageCollision(){}
#pragma endregion

 

