#include "Messages.h"
#include "Entity.h"
#include "Components.h"
#include "DebugNew.h"
#include "Box2D.h"
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
MInputState::MInputState(int i, std::string emmiter) :Message(MSG_INPUT_STATE, SCENE, emmiter), _controllerId(i){

	
}

MInputState::~MInputState(){ 

}

ControllerInputState& MInputState::getCInputState(){
	_cState.nEvents++;
	return _cState;
}
#pragma endregion

#pragma region ControllerStateMessage
MControllerState::MControllerState(std::string emmiter, int id, int action) :Message(MSG_CONTROLLER_STATE, SCENE, emmiter){
	_controllerId = id;
	_action = action;
}

MControllerState::~MControllerState(){

}



#pragma endregion


#pragma region UpdateTransform Message 

MUpdateTransform::MUpdateTransform(Ogre::Vector3 newPos, float newRotation, float h, float w, std::string emmiter) :Message(MSG_UPDATE_TRANSFORM, ENTITY, emmiter), _nPos(newPos), _nRotation(newRotation)
, _w(w), _h(h){

};
MUpdateTransform::~MUpdateTransform() {

}
Ogre::Vector3 MUpdateTransform::GetPos() { return _nPos; }
float MUpdateTransform::getRotation() { return _nRotation; }

#pragma endregion

#pragma region Camera Follow Message 

MCameraFollow::MCameraFollow(Ogre::Vector3 newPos, std::string emmiter) :Message(MSG_CAMERA_FOLLOW, SCENE, emmiter), _nPos(newPos){

};
MCameraFollow::~MCameraFollow() {

}
Ogre::Vector3 MCameraFollow::GetPos() { return _nPos; }


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

	MModInvisibility::MModInvisibility(std::string emmiter) : Message(MSG_MOD_INVS, ENTITY, emmiter){}
	MModInvisibility::~MModInvisibility(){}

	MModDmg::MModDmg(std::string emmiter, float value) :Message(MSG_MOD_DMG, ENTITY, emmiter), _value(value){}
	MModDmg::~MModDmg(){}
	
	MModVel::MModVel(std::string emmiter, float value) : Message(MSG_MOD_VEL, ENTITY, emmiter), _value(value){}
	MModVel::~MModVel(){}

	MModVelAndJump::MModVelAndJump(std::string emmiter, float valueVel, float valueJump) : Message(MSG_MOD_VEL_JUMP, ENTITY, emmiter), _valueVel(valueVel), _valueJump(valueJump){}
	MModVelAndJump::~MModVelAndJump(){}

	MModFireRate::MModFireRate(std::string emmiter, float valueFireRate) : Message(MSG_MOD_FIRERATE, ENTITY, emmiter), _valueFireRate(valueFireRate){}
	MModFireRate::~MModFireRate(){}

	MModVelBullets::MModVelBullets(std::string emmiter, float valueVelBullets) : Message(MSG_MOD_VELBULLETS, ENTITY, emmiter), _valueVelBullets(valueVelBullets){}
	MModVelBullets::~MModVelBullets(){}

	MDeactivate::MDeactivate(std::string emmiter) : Message(MSG_PASSMOD_DES, ENTITY, emmiter){}
	MDeactivate::~MDeactivate(){}


#pragma endregion


	//--------------------------------------------------	DASH MSG		----------------------------------------------------------//
#pragma region Dash

	MDash::MDash(std::string emmiter, b2Vec2 *impulse) : Message(MSG_DASH, ENTITY, emmiter), _dashValue(impulse){}
	MDash::~MDash(){
		delete _dashValue;
	}

#pragma endregion


	//--------------------------------------------------	HERA´S RUNE MSG		----------------------------------------------------------//
#pragma region Hera´s rune
	//restore life of cards
	MRestoreLifeCards::MRestoreLifeCards(std::string emmiter) : Message(MSG_RESTORE_LIFE_CARDS, ENTITY, emmiter){}
	MRestoreLifeCards::~MRestoreLifeCards(){}

#pragma endregion

#pragma region Reset Actives

	MReset::MReset(std::string emmiter) : Message(MSG_ACTIVEMOD_RES, ENTITY, emmiter){}
	MReset::~MReset(){}

#pragma endregion



	//--------------------------------------------------	DIE MSG		----------------------------------------------------------//
#pragma region Die

	MDie::MDie(std::string emmiter) : Message(MSG_DIE, SCENE_ONLY, emmiter){}
	MDie::~MDie(){
		
	}

#pragma endregion


	//--------------------------------------------------	DAMAGE ARMATURE MSG		----------------------------------------------------------//
#pragma region MDamageArmature

	MDamageArmature::MDamageArmature(float damage, uint16_t mask, std::string emmiter) : Message(MSG_DAMAGE_ARMATURE, ENTITY, emmiter), _damage(damage), _whereDmg(mask){}
	MDamageArmature::~MDamageArmature(){}

#pragma endregion

	//--------------------------------------------------	DAMAGE ARMATURE MSG		----------------------------------------------------------//
#pragma region MBulletHit

	MBulletHit::MBulletHit(float damage, uint16_t targetMask, std::string emmiter) : Message(MSG_BULLET_HIT, ENTITY, emmiter), _damage(damage), _targetMask(targetMask){}
	MBulletHit::~MBulletHit(){}

#pragma endregion

	//-------------------------------------------------- GUI MSGS -----------------------------------------------------------------------------
#pragma region Button Active
	MButtonAct::MButtonAct(std::string emmiter, unsigned int index) :Message(MSG_GUI_BUTTON_ACTIVE, SCENE, emmiter), _activeIndex(index) {}
	MButtonAct::~MButtonAct() {};
	unsigned int MButtonAct::getActiveButtonIndex() { return _activeIndex; };
#pragma endregion

#pragma region Button Click
	MButtonClick::MButtonClick(std::string emmiter) :Message(MSG_GUI_BUTTON_CLICK, SCENE, emmiter) {};
	MButtonClick::~MButtonClick() {};

#pragma endregion

#pragma region Life State
	MLifeState::MLifeState(std::string emmiter, size_t lifeval) : Message(MSG_LIFE_STATE, SCENE, emmiter), _life(lifeval) {};
	MLifeState::~MLifeState() {};
	size_t MLifeState::getLifeState() { return _life; };

#pragma endregion

#pragma region Ability Set
	MAbilitySet::MAbilitySet(std::string emmiter, int playerId, ComponentType compId, int type) : Message(MSG_ABILITY_SETTER, SCENE_ONLY, emmiter), _playerId(playerId),_c(compId), _type(type) {};
	MAbilitySet::~MAbilitySet() {};

#pragma endregion

#pragma region Update Scene Timer 
	MUpdateSceneTimer::MUpdateSceneTimer(std::string emmiter, size_t value) : Message(MSG_UPDATE_SCENETIMER, SCENE_ONLY, emmiter), _value(value) {};
	MUpdateSceneTimer::~MUpdateSceneTimer() {};
	float MUpdateSceneTimer::getSceneTimer() { return _value; };

#pragma endregion

#pragma region Update Active Timer 
	MUpdateActiveTimer::MUpdateActiveTimer(std::string emmiter, size_t value) : Message(MSG_UPDATE_ACTIVETIMER, SCENE_ONLY, emmiter), _value(value) {};
	MUpdateActiveTimer::~MUpdateActiveTimer() {};
	float MUpdateActiveTimer::getActiveTimer() { return _value; };

#pragma endregion


#pragma region Passive Dead 
	MPassiveDead::MPassiveDead(std::string emmiter) : Message(MSG_PASSIVE_DEAD, SCENE_ONLY, emmiter) {};
	MPassiveDead::~MPassiveDead() {};	
#pragma endregion

#pragma region Active Dead 
	MActiveDead::MActiveDead(std::string emmiter) : Message(MSG_ACTIVE_DEAD, SCENE_ONLY, emmiter) {};
	MActiveDead::~MActiveDead() {};
#pragma endregion

#pragma region Round MRoundFinished
	MRoundFinished::MRoundFinished(std::string emmiter, std::string winnerId) : Message(MSG_ROUND_FINISHED, SCENE_ONLY, emmiter), _winnerId(winnerId) {};
	MRoundFinished::~MRoundFinished() {};
	std::string MRoundFinished::getWinnerId() { return _winnerId; };
#pragma endregion
