#ifndef MESSAGES_H
#define MESSAGES_H
#include <string>
#include <SDL.h>
#include <list>
class Entity;
class gameComponent;




typedef enum msgType{
	STRING_MSG, INPUT_MSG, I_BUTTONDOWN_MSG 
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

//---------------------------------------------------		INPUT MSG		----------------------------------------------------------//
class InputMessage : public Message
{
public:
	InputMessage(msgDest d, std::string emmiter);
	virtual ~InputMessage();
	
	void addMessage(Message* newMsg);
	size_t getNumMessages();
	const std::list<Message*> getMessages();
private:
	std::list<Message*> _inputMessages;

};

//---------------------------------------------------		INPUT BUTTONDOWN MSG		----------------------------------------------------------//
class IButtonDownMessage : public Message
{
public:
	IButtonDownMessage(SDL_GameControllerButton b, SDL_JoystickID i, msgDest d, std::string emmiter);
	virtual ~IButtonDownMessage();

	//Get the Joystick id (Player id)
	inline SDL_JoystickID getId(){ return _id; };
	//Get the Button pressed
	inline SDL_GameControllerButton getButton(){ return _button; };


private:
	SDL_GameControllerButton _button;
	SDL_JoystickID _id;

};



#endif