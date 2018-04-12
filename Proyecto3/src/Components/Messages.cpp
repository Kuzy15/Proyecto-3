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

//Input Message implementation 
#pragma region InputMessage
InputMessage::InputMessage(msgDest d, std::string emmiter) :Message(INPUT_MSG, BROADCAST, emmiter){

	
}

InputMessage::~InputMessage(){ 

	while (!(_inputMessages.empty())){
		Message * aux = _inputMessages.front();
		_inputMessages.pop_front();
		delete aux;
	}
}

void InputMessage::addMessage(Message* newMsg){

	_inputMessages.push_back(newMsg);
}

size_t InputMessage::getNumMessages(){

	return _inputMessages.size();
}

const std::list<Message*> InputMessage::getMessages(){
	return _inputMessages;
}

#pragma endregion

//InputButtonDown implementation
#pragma region IButtonDown Message
IButtonDownMessage::IButtonDownMessage(SDL_GameControllerButton b, SDL_JoystickID i, msgDest d, std::string emmiter) :Message(I_BUTTONDOWN_MSG, BROADCAST, emmiter), _button(b), _id(i){

}

IButtonDownMessage::~IButtonDownMessage(){}



#pragma endregion


#pragma region UpdateTransform Message 

UpdateTransformMessage::UpdateTransformMessage(Ogre::Vector3 newPos, float newRotation, std::string emmiter):Message(MSG_UPDATETRANSFORM, BROADCAST, emmiter), _nPos(newPos), _nRotation(newRotation){

};
UpdateTransformMessage::~UpdateTransformMessage() {

}
Ogre::Vector3 UpdateTransformMessage::GetPos() { return _nPos; }
float UpdateTransformMessage::getRotation() { return _nRotation; }

#pragma endregion
