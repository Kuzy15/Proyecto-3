#ifndef MESSAGES_H
#define MESSAGES_H
#include <string>
#include <SDL.h>
#include <list>
#include <Ogre.h>
#include <stdint.h>

class Entity;
class GameComponent;

#pragma region CInputState

typedef enum ButtonState{
	BTT_PRESSED = 1, BTT_NONE = 0
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

typedef enum MessageType{

	MSG_STRING, MSG_INPUT_STATE,
	MSG_CONTROLLER_STATE, MSG_UPDATE_TRANSFORM,
	MSG_PLAYER_MOVE_X, MSG_COLLISION,
	MSG_PLAYER_JUMP,
	MSG_PLAYER_SHOT,
	MSG_RIGIDBODY_MOVE_X,
	MSG_RIGIDBODY_JUMP,
	MSG_COLLISION_TERRAIN


};

typedef enum MessageDestination {
	//Message only meant to reach the components of the entity in which
	//it is created.
	ENTITY,
	//Message ment to reach every other entity in the scene plus the scene,
	// but not the entity in which it is created
	SCENE,
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
	MInputState(int i,MessageDestination d, std::string emmiter);
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
	MControllerState(MessageDestination d, std::string emmiter, int id, int action);
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
	~MUpdateTransform();
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
	MCollisionBegin(uint16_t me, uint16_t contact, std::string emmiter);
	~MCollisionBegin();

	uint16_t GetMyCategory(){ return _myCategory; };
	uint16_t GetContactMask(){ return _contactMask; };


private:
	//Private fields for the new position and quaternion of the entity
	uint16_t _myCategory;
	uint16_t _contactMask;
};


//--------------------------------------------------	PLAYER SHOT MSG		----------------------------------------------------------//
class MPlayerShot : public Message
{
public:
	MPlayerShot(float axisXValue, float axisYValue, std::string emmiter);
	~MPlayerShot();

	float getXValue(){ return _XValue; };
	float getYValue(){ return _YValue; };


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

#endif