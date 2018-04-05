#ifndef MESSAGES_H
#define MESSAGES_H
#include <string>
#include <SDL.h>
#include <list>
class Entity;
class gameComponent;

#pragma region ControllerState

typedef enum ButtonState{
	PRESSED = 0, RELEASED = 1, NONE = 2
};

struct ControllerState{

	int nEvents = 0;

	ButtonState Button_A = NONE;
	ButtonState Button_B = NONE;
	ButtonState Button_X = NONE;
	ButtonState Button_Y = NONE;

	ButtonState Right_Shoulder = NONE;
	ButtonState Left_Shoulder = NONE;

	ButtonState Right_Stick = NONE;
	ButtonState Left_Stick = NONE;

	ButtonState Button_Start = NONE;
	ButtonState Button_Back = NONE;

	ButtonState DPad_Right = NONE;
	ButtonState DPad_Left = NONE;
	ButtonState DPad_Up = NONE;
	ButtonState DPad_Down = NONE;

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
	CONTROLLER_STATE_MSG
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
	InputStateMessage(msgDest d, std::string emmiter);
	virtual ~InputStateMessage();

	ControllerState& getControllerState();

	inline int getNumMessages() { return _cState.nEvents; };

private:
	ControllerState _cState;
 
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






#endif