#include "Messages.h"
#include "Entity.h"
#include "Components.h"
#include "DebugNew.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

MCollisionBegin::MCollisionBegin(uint16_t me, uint16_t contact, Entity* who, std::string emmiter) : Message(MSG_COLLISION, ENTITY, emmiter),
	_myCategory(me), _contactMask(contact), _who(who){}
MCollisionBegin::~MCollisionBegin(){}
#pragma endregion


//--------------------------------------------------	RIGIDBODY MOVE X MSG		----------------------------------------------------------//
#pragma region MessageRigidBodyMoveX

MRigidbodyMoveX::MRigidbodyMoveX(float v, std::string emmiter) : Message(MSG_RIGIDBODY_MOVE_X, ENTITY, emmiter), _XValue(v){}
MRigidbodyMoveX::~MRigidbodyMoveX(){}

#pragma endregion

//--------------------------------------------------	RIGIDBODY MOVE X MSG		----------------------------------------------------------//
#pragma region MessageRigidBodyMoveY

MRigidbodyMoveY::MRigidbodyMoveY(float v, std::string emmiter) : Message(MSG_RIGIDBODY_MOVE_Y, ENTITY, emmiter), _YValue(v){}
MRigidbodyMoveY::~MRigidbodyMoveY(){}

#pragma endregion


//--------------------------------------------------	RIGIDBODY JUMP MSG		----------------------------------------------------------//
#pragma region MessageRigidBodyJump

MRigidbodyJump::MRigidbodyJump(float f, std::string emmiter) : Message(MSG_RIGIDBODY_JUMP, ENTITY, emmiter), _force(f){}
MRigidbodyJump::~MRigidbodyJump(){}

#pragma endregion

//--------------------------------------------------	 COLLISION TERRAIN MSG		----------------------------------------------------------//
#pragma region MessageCollisionTerrain

MCollisionTerrain::MCollisionTerrain(std::string emmiter) : Message(MSG_COLLISION_TERRAIN, ENTITY, emmiter){}
MCollisionTerrain::~MCollisionTerrain(){}

#pragma endregion

//--------------------------------------------------	ADD ENTITY MSG		----------------------------------------------------------//
#pragma region MessageAddEntity

MAddEntity::MAddEntity(std::string emmiter, Entity* newEntity) : Message(MSG_ADD_ENTITY, SCENE_ONLY, emmiter), _newEntity(newEntity){}
MAddEntity::~MAddEntity(){}

#pragma endregion

//--------------------------------------------------	PLAYER SHOT MSG		----------------------------------------------------------//
#pragma region MessagePlayerShot

MPlayerShot::MPlayerShot(float axisXValue, float axisYValue, std::string emmiter) : Message(MSG_PLAYER_SHOT, ENTITY, emmiter), _XValue(axisXValue), _YValue(axisYValue){}
MPlayerShot::~MPlayerShot(){}

#pragma endregion

#pragma region MessageShot

MShot::MShot(float axisXValue, float axisYValue, std::string emmiter) : Message(MSG_SHOT, ENTITY, emmiter), _XValue(axisXValue), _YValue(axisYValue){}
MShot::~MShot(){}

#pragma endregion


//--------------------------------------------------	DAMAGE MSG		----------------------------------------------------------//
#pragma region DamageShot

MDamage::MDamage(float damage, std::string emmiter) : Message(MSG_DAMAGE, ENTITY, emmiter), _damage(damage){}
MDamage::~MDamage(){}

#pragma endregion


//--------------------------------------------------	PASSIVE SKILL MSG		----------------------------------------------------------//
#pragma region MessagePassiveSkill


	MModDmg::MModDmg(std::string emmiter, float value) :Message(MSG_MOD_DMG, ENTITY, emmiter), _value(value){}
	MModDmg::~MModDmg(){}
	
	MModVel::MModVel(std::string emmiter, float value) : Message(MSG_MOD_VEL, ENTITY, emmiter), _value(value){}
	MModVel::~MModVel(){}

	MModVelAndJump::MModVelAndJump(std::string emmiter, float valueVel, float valueJump) : Message(MSG_MOD_VEL_JUMP, ENTITY, emmiter), _valueVel(valueVel), _valueJump(valueJump){}
	MModVelAndJump::~MModVelAndJump(){}

	MModFireRate::MModFireRate(std::string emmiter, float valueFireRate) : Message(MSG_MOD_FIRERATE, ENTITY, emmiter), _valueFireRate(valueFireRate){}
	MModFireRate::~MModFireRate(){}

	MDeactivate::MDeactivate(std::string emmiter) : Message(MSG_PASSMOD_DES, ENTITY, emmiter){}
	MDeactivate::~MDeactivate(){}


#pragma endregion


