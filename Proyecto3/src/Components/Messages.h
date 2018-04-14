#ifndef MESSAGES_H
#define MESSAGES_H
#include <string>
#include <SDL.h>
#include <list>
#include <Ogre.h>

class Entity;
class gameComponent;

#pragma region CInputState

typedef enum ButtonState{
	BTT_PRESSED = 0, BTT_RELEASED = 1, BTT_NONE = 2
};

struct CInputState{

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

typedef enum msgType{

	STRING_MSG, INPUT_STATE_MSG,
	CONTROLLER_STATE_MSG, ENTITY_UPDATETRANSFORM,
	MSG_PLAYER_MOVE_X


};

typedef enum msgDest {
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
	msgType getType();
	msgDest getDestination();
	std::string getEmmiter();
	
protected:
	Message(msgType type, msgDest destination, std::string emmiter);
	msgType _messageType;
	msgDest _messageDestination;
	std::string _emmiter;
};

class stringMessage : public Message
{
public:
	stringMessage(std::string message, msgDest d, std::string emmiter);
	~stringMessage();
	std::string getText();

private:
	std::string _text;

};

//---------------------------------------------------		INPUT STATE MSG		----------------------------------------------------------//
//Message who contains all the input taken from the current frame. One for controller/player.
class InputStateMessage : public Message
{
public:
	InputStateMessage(int i,msgDest d, std::string emmiter);
	virtual ~InputStateMessage();

	CInputState& getCInputState();
	inline int getId(){ return _controllerId; };
	inline int getNumMessages() { return _cState.nEvents; };

private:
	CInputState _cState;
	int _controllerId;
};


//---------------------------------------------------		INPUT CONTROLLER CONNECTED MSG		----------------------------------------------------------//
//Notice that a new controller has connected or disconnected. Values: 1 for connected, 0 for disconnected.
class ControllerStateMessage : public Message
{
public:
	ControllerStateMessage(msgDest d, std::string emmiter, int id, int action);
	virtual ~ControllerStateMessage();

	inline int getId(){ return _controllerId; };

	inline int getAction(){ return _action; };
private:
	int _controllerId;
	int _action;
};
//--------------------------------------------------	UPDATETRANSFORM MSG		----------------------------------------------------------//
class UpdateTransformMessage: public Message
{
public:
	UpdateTransformMessage(Ogre::Vector3 newPos, Ogre::Quaternion newQuaternion, std::string emmiter);
	~UpdateTransformMessage();
	Ogre::Vector3 GetPos();
	Ogre::Quaternion GetQuat();

private:
	//Private fields for the new position and quaternion of the entity
	Ogre::Vector3 _nPos;
	Ogre::Quaternion _nQuat;
};


//--------------------------------------------------	INPUT PLAYER MSG	 (ABSTRACT)	----------------------------------------------------------//
class MessagePlayerInput : public Message
{
public:
	MessagePlayerInput(int i, std::string emmiter);
	~MessagePlayerInput();
	inline int GetId(){ return _id; };


private:
	//Private fields for the new position and quaternion of the entity
	int _id;
};



//--------------------------------------------------	PLAYER MOVEMENT X MSG		----------------------------------------------------------//
class MessagePlayerMoveX : public MessagePlayerInput
{
public:
	MessagePlayerMoveX(float v, int i, std::string emmiter);
	~MessagePlayerMoveX();
	inline float GetValue(){ return _value; };
	

private:
	//Private fields for the new position and quaternion of the entity
	float _value;
};





#endif