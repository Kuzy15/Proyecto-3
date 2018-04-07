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
InputStateMessage::InputStateMessage(msgDest d, std::string emmiter) :Message(INPUT_STATE_MSG, BROADCAST, emmiter){

	
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

