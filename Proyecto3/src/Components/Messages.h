#ifndef MESSAGES_H
#define MESSAGES_H
#include <string>

class Entity;
class gameComponent;

typedef enum msgType{
	STRING_MSG
};

typedef enum msgDest {
	SCENE,
	ENTITY,
	BROADCAST	
};

class Message
{
public:
	
	~Message();
	msgType getType();
	msgDest getDestination();
	
protected:
	Message(msgType type, msgDest destination);
	msgType _messageType;
	msgDest _messageDestination;
};

class stringMessage : public Message
{
public:
	stringMessage(std::string message, msgDest d);
	~stringMessage();
	std::string getText();

private:
	std::string _text;

};



#endif