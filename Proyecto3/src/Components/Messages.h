#ifndef MESSAGES_H
#define MESSAGES_H
#include <string>
#include <SDL.h>
#include <list>
#include <Ogre.h>
#include <stdint.h>

class Entity;
class GameComponent;
class b2Vec2;
enum ComponentType;

#pragma region CInputState



enum ButtonState{
	BTT_PRESSED = 1, BTT_NONE = 0, BTT_RELEASED = 2
};

struct ControllerInputState{

	int nEvents = 0;

	ButtonState Button_A = BTT_NONE;
	ButtonState Button_B = BTT_NONE;
	ButtonState Button_X = BTT_NONE;
	ButtonState Button_Y = BTT_NONE;

	ButtonState Right_Shoulder = BTT_NONE;
	ButtonState Left_Shoulder = BTT_NONE;

	ButtonState Right_Stick = BTT_NONE;
	ButtonState Left_Stick = BTT_NONE;

	ButtonState Button_Start = BTT_NONE;
	ButtonState Button_Back = BTT_NONE;

	ButtonState DPad_Right = BTT_NONE;
	ButtonState DPad_Left = BTT_NONE;
	ButtonState DPad_Up = BTT_NONE;
	ButtonState DPad_Down = BTT_NONE;

	float Trigger_Left = 0.0f;
	float Trigger_Right = 0.0f;

	float Axis_LeftX = 0.0f;
	float Axis_LeftY = 0.0f;
	float Axis_RightX = 0.0f;
	float Axis_RightY = 0.0f;
};

#pragma endregion

typedef enum MessageType {

	MSG_STRING, MSG_INPUT_STATE,
	MSG_CONTROLLER_STATE, MSG_UPDATE_TRANSFORM,
	MSG_PLAYER_MOVE_X, MSG_COLLISION,
	MSG_PLAYER_JUMP,
	MSG_PLAYER_SHOT,
	MSG_RIGIDBODY_MOVE_X,
	MSG_RIGIDBODY_MOVE_Y,
	MSG_RIGIDBODY_JUMP,
	MSG_COLLISION_TERRAIN,
	MSG_ADD_ENTITY,
	MSG_SHOT,
	MSG_MOD_DMG,
	MSG_MOD_VEL,
	MSG_MOD_VEL_JUMP,
	MSG_MOD_FIRERATE,
	MSG_MOD_INVS,
	MSG_MOD_VELBULLETS,
	MSG_PASSMOD_DES,
	MSG_ACTIVEMOD_RES,
	MSG_RESTORE_LIFE_CARDS,
	MSG_DAMAGE,
	MSG_DASH,
	MSG_DIE,
	MSG_GUI_BUTTON_ACTIVE,
	MSG_GUI_BUTTON_CLICK,
	MSG_BULLET_HIT,
	MSG_DAMAGE_ARMATURE,
	MSG_CAMERA_FOLLOW,
	MSG_LIFE_STATE,
	MSG_ABILITY_SETTER,
	MSG_UPDATE_SCENETIMER,
	MSG_UPDATE_ACTIVETIMER,
	MSG_PASSIVE_DEAD,
	MSG_ACTIVE_DEAD,
	MSG_ROUND_FINISHED

	

	


};

typedef enum MessageDestination {
	//Message only meant to reach the components of the entity in which
	//it is created.
	ENTITY,
	//Message ment to reach every other entity in the scene plus the scene,
	// but not the entity in which it is created
	SCENE,
	//Message wich is read only by the scene class. ****Luego se cambia el nombre****
	SCENE_ONLY,
	//This message will reach everyone in the scene. The entity in which it
	//was created AND other entities in the scene.
	BROADCAST	
};

class Message
{
public:
	
	virtual ~Message();
	MessageType getType();
	MessageDestination getDestination();
	std::string getEmmiter();
	
protected:
	Message(MessageType type, MessageDestination destination, std::string emmiter);
	MessageType _messageType;
	MessageDestination _messageDestination;
	std::string _emmiter;
};

class MString : public Message
{
public:
	MString(std::string message, MessageDestination d, std::string emmiter);
	~MString();
	std::string getText();

private:
	std::string _text;

};

//---------------------------------------------------		INPUT STATE MSG		----------------------------------------------------------//
//Message who contains all the input taken from the current frame. One for controller/player.
class MInputState : public Message
{
public:
	MInputState(int i, std::string emmiter);
	virtual ~MInputState();

	ControllerInputState& getCInputState();
	inline int getId(){ return _controllerId; };
	inline int getNumMessages() { return _cState.nEvents; };

private:
	ControllerInputState _cState;
	int _controllerId;
};


//---------------------------------------------------		INPUT CONTROLLER CONNECTED MSG		----------------------------------------------------------//
//Notice that a new controller has connected or disconnected. Values: 1 for connected, 0 for disconnected.
class MControllerState : public Message
{
public:
	MControllerState(std::string emmiter, int id, int action);
	virtual ~MControllerState();

	inline int getId(){ return _controllerId; };

	inline int getAction(){ return _action; };
private:
	int _controllerId;
	int _action;
};
//--------------------------------------------------	UPDATETRANSFORM MSG		----------------------------------------------------------//
class MUpdateTransform: public Message
{
public:
	MUpdateTransform(Ogre::Vector3 newPos, float nRotation,float h, float w, std::string emmiter);
	virtual ~MUpdateTransform();

	Ogre::Vector3 GetPos();
	float getRotation();
	inline float getW(){ return _w; }
	inline float getH(){ return _h; }

private:
	//Private fields for the new position and quaternion of the entity
	Ogre::Vector3 _nPos;
	float _nRotation;
	float _w;
	float _h;
};


//--------------------------------------------------	CAMERA FOLLOW MSG		----------------------------------------------------------//
class MCameraFollow : public Message
{
public:
	MCameraFollow(Ogre::Vector3 newPos, std::string emmiter);
	virtual ~MCameraFollow();

	Ogre::Vector3 GetPos();

private:
	//Private fields for the new position and quaternion of the entity
	Ogre::Vector3 _nPos;
};



//--------------------------------------------------	INPUT PLAYER MSG	 (ABSTRACT)	----------------------------------------------------------//
class MPlayerInput : public Message
{
public:
	MPlayerInput(int i, std::string emmiter);
	~MPlayerInput();
	inline int GetId(){ return _id; };


private:
	//Private fields for the new position and quaternion of the entity
	int _id;
};



//--------------------------------------------------	PLAYER MOVEMENT X MSG		----------------------------------------------------------//
class MPlayerMoveX : public MPlayerInput
{
public:
	MPlayerMoveX(float v, int i, std::string emmiter);
	~MPlayerMoveX();
	inline float GetValue(){ return _value; };
	

private:
	//Private fields for the new position and quaternion of the entity
	float _value;
};




//--------------------------------------------------	PLAYER JUMP MSG		----------------------------------------------------------//
class MJump : public Message
{
public:
	MJump(std::string emmiter);
	~MJump();
	inline bool GetJump(){ return _jump; };


private:
	//Private fields for the new position and quaternion of the entity
	bool _jump;
};






//--------------------------------------------------	COLLISION MSG		----------------------------------------------------------//
class MCollisionBegin : public Message
{
public:
	MCollisionBegin(uint16_t me, uint16_t contact, Entity* who, std::string emmiter);
	~MCollisionBegin();

	uint16_t GetMyCategory(){ return _myCategory; };
	uint16_t GetContactMask(){ return _contactMask; };
	Entity* GetWho(){ return _who; };

private:
	//Private fields for the new position and quaternion of the entity
	uint16_t _myCategory;
	uint16_t _contactMask;
	Entity* _who;
};


//--------------------------------------------------	PLAYER SHOT MSG		----------------------------------------------------------//
class MPlayerShot : public Message
{
public:
	MPlayerShot(float axisXValue, float axisYValue, std::string emmiter);
	~MPlayerShot();

	inline float getXValue(){ return _XValue; };
	inline float getYValue(){ return _YValue; };


private:
	
	float _XValue;						//Shot direction
	float _YValue;
};


//--------------------------------------------------	PLAYER SHOT MSG		----------------------------------------------------------//
class MShot : public Message
{
public:
	MShot(float axisXValue, float axisYValue, std::string emmiter);
	~MShot();

	inline float getXValue(){ return _XValue; };
	inline float getYValue(){ return _YValue; };


private:

	float _XValue;						//Shot direction
	float _YValue;
};


//--------------------------------------------------	RIGIDBODY MOVE X MSG		----------------------------------------------------------//
class MRigidbodyMoveX : public Message
{
public:
	MRigidbodyMoveX(float axisXValue, std::string emmiter);
	~MRigidbodyMoveX();

	float getXValue(){ return _XValue; };

private:

	float _XValue;						//Shot direction

};

//--------------------------------------------------	RIGIDBODY MOVE Y MSG		----------------------------------------------------------//
class MRigidbodyMoveY : public Message
{
public:
	MRigidbodyMoveY(float axisYValue, std::string emmiter);
	~MRigidbodyMoveY();

	float getYValue(){ return _YValue; };

private:

	float _YValue;						//Shot direction

};





//--------------------------------------------------	RIGIDBODY JUMP MSG		----------------------------------------------------------//
class MRigidbodyJump : public Message
{
public:
	MRigidbodyJump(float f, std::string emmiter);
	~MRigidbodyJump();

	float getForce(){ return _force; };

private:

	float _force;						//Shot direction

};

//--------------------------------------------------	COLLISION TERRAIN  MSG		----------------------------------------------------------//
class MCollisionTerrain : public Message
{
public:
	MCollisionTerrain( std::string emmiter);
	~MCollisionTerrain();

};

//--------------------------------------------------	ADD ENTITY  MSG		----------------------------------------------------------//
class MAddEntity : public Message
{
public:
	MAddEntity(std::string emmiter, Entity* newEntity);
	~MAddEntity();

	inline Entity* getEntity(){ return _newEntity; };
private:
	Entity* _newEntity;
};


//--------------------------------------------------	DAMAGE MSG		----------------------------------------------------------//
class MDamage : public Message
{
public:
	MDamage(float damage, std::string emmiter);
	~MDamage();

	inline float getDamage(){ return _damage; };

private:
	float _damage;
	
};


class MBulletHit : public Message
{
public:
	MBulletHit(float damage, uint16_t targetMask, std::string emmiter);
	~MBulletHit();

	inline float getDamage(){ return _damage; };
	inline uint16_t getTargetMask(){ return _targetMask; }

private:
	float _damage;
	uint16_t _targetMask;
};


//--------------------------------------------------	DAMAGE ARMATURE MSG		----------------------------------------------------------//

class MDamageArmature : public Message
{
public:
	MDamageArmature(float damage, uint16_t whereDmg, std::string emmiter);
	~MDamageArmature();

	inline float getDamage(){ return _damage; };
	inline uint16_t getWhere(){ return _whereDmg; };

private:
	float _damage;
	uint16_t _whereDmg;
};





//--------------------------------------------------	PASSIVE SKILL MSG		----------------------------------------------------------//
//modify damage
class MModDmg : public Message{
public:
	MModDmg(std::string emmiter, float value);
	~MModDmg();

	inline float getValue(){ return _value; };
private:
	float _value;
};

//modify vel
class MModVel : public Message{
public:
	MModVel(std::string emmiter, float value);
	~MModVel();

	inline float getValue(){ return _value; };
private:
	float _value;
};

//modify vel and jump
class MModVelAndJump : public Message{
public:
	MModVelAndJump(std::string emmiter, float valueVel, float valueJump);
	~MModVelAndJump();

	inline float getVelValue(){ return _valueVel; };
	inline float getJumpValue(){ return _valueJump; };

private:
	float _valueVel;
	float _valueJump;
};


//modify firerate
class MModFireRate : public Message{
public:
	MModFireRate(std::string emmiter, float valueFireRate);
	~MModFireRate();

	inline float getFireRateValue(){ return _valueFireRate; };

private:
	float _valueFireRate;
};


//modify vel of bullets
class MModVelBullets : public Message{
public:
	MModVelBullets(std::string emmiter, float _valueVelBullets);
	~MModVelBullets();

	inline float getVelBulletsValue(){ return _valueVelBullets; };

private:
	float _valueVelBullets;
};


//invisibility
class MModInvisibility : public Message{
public:
	MModInvisibility(std::string emmiter);
	~MModInvisibility();

};


//deactivate mod
class MDeactivate : public Message{
public:
	MDeactivate(std::string emmiter);
	~MDeactivate();
private:
	float _valueVel;
	float _valueJump;
};




//--------------------------------------------------	ACTIVE MSG		----------------------------------------------------------//

//Dash Message
class MDash : public Message{
public:
	MDash(std::string emmiter, b2Vec2* impulse);
	~MDash();

	inline b2Vec2* getDashValue(){ return _dashValue; }
private:
	b2Vec2* _dashValue;
	
};



//Hera´s Rune
class MRestoreLifeCards : public Message{
public:
	MRestoreLifeCards(std::string emmiter);
	~MRestoreLifeCards();

};

//reset mod
class MReset : public Message{
public:
	MReset(std::string emmiter);
	~MReset();

};

//--------------------------------------------------	DIE MSG		----------------------------------------------------------//
class MDie : public Message
{
public:
	MDie(std::string emmiter);
	~MDie();

	//inline float getDamage(){ return _damage; };

private:
	//float timeWhenDie
};

//--------------------------------------------------	BUTTON ACTIVE MSG		----------------------------------------------------------//
class MButtonAct: public Message
{
public:
	MButtonAct(std::string emmiter, unsigned int index);
	~MButtonAct();
	unsigned int getActiveButtonIndex();

private:
	unsigned int _activeIndex;
};
//--------------------------------------------------	BUTTON CLICK MSG		----------------------------------------------------------//
class MButtonClick: public Message
{
public:
	MButtonClick(std::string emmiter, int id);
	~MButtonClick();
	inline int getId(){ return _idClicked; };

private:
	int _idClicked;
};
//--------------------------------------------------	LIFE STATE MSG		----------------------------------------------------------//
class MLifeState: public Message
{
public:
	MLifeState(std::string emmiter, size_t lifeValue);
	~MLifeState();
	size_t getLifeState();
private:
	size_t _life;
};


//--------------------------------------------------	ABILITY SETTER MSG		----------------------------------------------------------//
class MAbilitySet : public Message
{
public:
	MAbilitySet(std::string emmiter, int playerId, ComponentType compId, int type);
	~MAbilitySet();
	inline int getId(){ return _playerId; };
	inline ComponentType getComponentType(){ return _c; };
	inline int getType(){ return _type; };
private:
	int _playerId;
	ComponentType _c;
	int _type;
};


//--------------------------------------------------	SCENE TIMER UPDATE MSG		----------------------------------------------------------//
class MUpdateSceneTimer : public Message
{
public:
	MUpdateSceneTimer(std::string emmiter, size_t value);
	~MUpdateSceneTimer();
	size_t getSceneTimer();
private:
	size_t _value;
};


//--------------------------------------------------	ACTIVE TIMER UPDATE MSG		----------------------------------------------------------//
class MUpdateActiveTimer : public Message
{
public:
	MUpdateActiveTimer(std::string emmiter, size_t value);
	~MUpdateActiveTimer();
	size_t getActiveTimer();
private:
	size_t _value;
};


//--------------------------------------------------	PASSIVE DEAD MSG		----------------------------------------------------------//
class MPassiveDead : public Message
{
public:
	MPassiveDead(std::string emmiter);
	~MPassiveDead();

};

//--------------------------------------------------	ACTIVE DEAD MSG		----------------------------------------------------------//
class MActiveDead : public Message
{
public:
	MActiveDead(std::string emmiter);
	~MActiveDead();

};


//--------------------------------------------------	ROUND FINISHED MSG		----------------------------------------------------------//
class MRoundFinished : public Message
{
public:
	MRoundFinished(std::string emmiter, std::string winnerId);
	~MRoundFinished();
	std::string getWinnerId();
private:
	std::string _winnerId;
};









#endif