#include "Messages.h"
#include "Entity.h"
#include "Components.h"

//Basic Message Class implementation.
#pragma region basic Message 
Message::Message(msgType t, msgDest dst): _messageType(t), _messageDestination(dst) 
{
}
Message::~Message()
{
}
msgType Message::getType() {
	return _messageType;
}
msgDest Message::getDestination() {
	return _messageDestination;
}
#pragma endregion

#pragma region stringMessage
stringMessage::stringMessage(std::string t, msgDest dest): Message(STRING_MSG,BROADCAST), _text(t) {

}
stringMessage::~stringMessage() {

}

std::string stringMessage::getText() {
	return _text;
}
#pragma endregion

