#ifndef MESSAGES_H
#define MESSAGES_H
#include <string>

class Entity;
class gameComponent;

typedef enum msgType{
	STRING_MSG
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
	
	~Message();
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



#endif