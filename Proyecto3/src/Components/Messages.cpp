#include "Messages.h"
#include "Entity.h"
#include "Components.h"

//Basic Message Class implementation.
#pragma region basic Message 
Message::Message(MessageType t, MessageDestination dst, std::string emmiter): _messageType(t), _messageDestination(dst), _emmiter(emmiter)
{
}
Message::~Message()
{
}
MessageType Message::getType() {
	return _messageType;
}
MessageDestination Message::getDestination() {
	return _messageDestination;
}
std::string Message::getEmmiter() {
	return _emmiter;
}
#pragma endregion

#pragma region stringMessage
MString::MString(std::string t, MessageDestination dest, std::string emmiter): Message(MSG_STRING,BROADCAST,emmiter), _text(t) {

}
MString::~MString() {

}

std::string MString::getText() {
	return _text;
}
#pragma endregion

//Input Message implementation 
#pragma region InputStateMessage
MInputState::MInputState(int i, MessageDestination d, std::string emmiter) :Message(MSG_INPUT_STATE, BROADCAST, emmiter), _controllerId(i){

	
}

MInputState::~MInputState(){ 

}

ControllerInputState& MInputState::getCInputState(){
	_cState.nEvents++;
	return _cState;
}
#pragma endregion

#pragma region ControllerStateMessage
MControllerState::MControllerState(MessageDestination d, std::string emmiter, int id, int action) :Message(MSG_CONTROLLER_STATE, SCENE, emmiter){
	_controllerId = id;
	_action = action;
}

MControllerState::~MControllerState(){

}



#pragma endregion


#pragma region UpdateTransform Message 

MUpdateTransform::MUpdateTransform(Ogre::Vector3 newPos, float newRotation, float h, float w, std::string emmiter) :Message(MSG_UPDATE_TRANSFORM, BROADCAST, emmiter), _nPos(newPos), _nRotation(newRotation)
, _w(w), _h(h){

};
MUpdateTransform::~MUpdateTransform() {

}
Ogre::Vector3 MUpdateTransform::GetPos() { return _nPos; }
float MUpdateTransform::getRotation() { return _nRotation; }

#pragma endregion

#pragma region MessageInputPlayer

MPlayerInput::MPlayerInput(int i, std::string emmiter): Message(MSG_PLAYER_MOVE_X,ENTITY,emmiter), _id(i){}
MPlayerInput::~MPlayerInput(){}
#pragma endregion

#pragma region MessagePlayerMoveX
MPlayerMoveX::MPlayerMoveX(float v, int i, std::string emmiter) : MPlayerInput(i, emmiter),_value(v){}
MPlayerMoveX::~MPlayerMoveX(){};
#pragma endregion 


#pragma region MessagePlayerJump
MJump::MJump( std::string emmiter) : Message(MSG_PLAYER_JUMP, ENTITY,emmiter){}
MJump::~MJump(){};
#pragma endregion 

#pragma region MessageCollision

MCollisionBegin::MCollisionBegin(uint16_t me, uint16_t contact, std::string emmiter) : Message(MSG_COLLISION, ENTITY, emmiter),
	_myCategory(me), _contactMask(contact){}
MCollisionBegin::~MCollisionBegin(){}
#pragma endregion


//--------------------------------------------------	RIGIDBODY MOVE X MSG		----------------------------------------------------------//
#pragma region MessageRigidBodyMoveX

MRigidbodyMoveX::MRigidbodyMoveX(float v, std::string emmiter) : Message(MSG_RIGIDBODY_MOVE_X, ENTITY, emmiter), _XValue(v){}
MRigidbodyMoveX::~MRigidbodyMoveX(){}

#pragma endregion


//--------------------------------------------------	RIGIDBODY JUMP MSG		----------------------------------------------------------//
#pragma region MessageRigidBodyJump

MRigidbodyJump::MRigidbodyJump(float f, std::string emmiter) : Message(MSG_RIGIDBODY_JUMP, ENTITY, emmiter), _force(f){}
MRigidbodyJump::~MRigidbodyJump(){}

#pragma endregion

//--------------------------------------------------	RIGIDBODY JUMP MSG		----------------------------------------------------------//
#pragma region MessageCollisionTerrain

MCollisionTerrain::MCollisionTerrain(std::string emmiter) : Message(MSG_COLLISION_TERRAIN, ENTITY, emmiter){}
MCollisionTerrain::~MCollisionTerrain(){}

#pragma endregion
