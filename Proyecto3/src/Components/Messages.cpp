#include "Messages.h"
#include "Entity.h"
#include "Components.h"

//Basic Message Class implementation.
#pragma region basic Message 
Message::Message(msgType t, msgDest dst, std::string emmiter): _messageType(t), _messageDestination(dst), _emmiter(emmiter)
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
std::string Message::getEmmiter() {
	return _emmiter;
}
#pragma endregion

#pragma region stringMessage
stringMessage::stringMessage(std::string t, msgDest dest, std::string emmiter): Message(STRING_MSG,BROADCAST,emmiter), _text(t) {

}
stringMessage::~stringMessage() {

}

std::string stringMessage::getText() {
	return _text;
}
#pragma endregion

