#include "InputManager.h"

#ifdef _DEBUG
#include <iostream>
#include <string>
#endif

InputManager* InputManager::_instance = nullptr;	//Definition of the instance 

InputManager::InputManager()
{

	//Open all the joystick files conected.
	int i = 0;
	while( i < SDL_NumJoysticks() && i < MAX_PLAYERS)
	{
		_playersJoystick.push_back( SDL_JoystickOpen(i));
		if (_playersJoystick[i] == NULL && SDL_IsGameController(i)){
			//ERROR MESSAGE
		}
		_playerController.push_back(SDL_GameControllerOpen(i));
#ifdef _DEBUG
		printf("    %s\n", SDL_JoystickName(_playersJoystick[i]));
#endif
		i++;
	}

	//Enable event flow
	SDL_JoystickEventState(SDL_ENABLE);


#pragma endregion

	

}


InputManager::~InputManager()
{
	

	//Close opened Joysticks
	for (size_t i = 0; i < _playersJoystick.size(); i++){
		if (_playersJoystick[i] != NULL) SDL_JoystickClose(_playersJoystick[i]);
	}

	//Close SDL
	SDL_Quit();
}

InputManager& InputManager::getInstance(){
	// Lazy initialize.
	if (InputManager::_instance == NULL) InputManager::_instance = new InputManager();
	return *InputManager::_instance;
}

//Push new input message to the queue
void InputManager::pushMessage(Message* newMessage){
	_inputMsgQ.push_back(newMessage);	
}

//Send messages to the scene queue if there's any message
void InputManager::getMessages(std::list<Message*> &sceneQueue){
	while (!(_inputMsgQ.empty())){
		sceneQueue.push_back(_inputMsgQ.front());
		_inputMsgQ.pop_front();
	}
}

void InputManager::handleInput(){
	
	SDL_Event event;
	/* Other initializtion code goes here */

	/* Start main game loop here */
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			
			break;

		case SDL_QUIT:
			
			break;

		case SDL_CONTROLLERBUTTONDOWN:
			/*SEND MESSAGE LIKE: new InputMessage(SDL_GameControllerButton type, SDL_JoystickID id...)*/
			break;

		case SDL_CONTROLLERBUTTONUP:
			break;

		case SDL_CONTROLLERAXISMOTION:
			if ((event.caxis.value < -20000) || (event.caxis.value > 20000))
			{
				if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX){
					
				}
				else if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY){}

				else if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX){}

				else if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY){}
			
			}
			break;
		case SDL_CONTROLLERDEVICEADDED:
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			//event.cdevice.which
			break;
		
		default:
			break;
		}
#ifdef _DEBUG
		//PRINT DEBUG FOR CONSOLE:
		//printf(std::to_string(event.cbutton.button).c_str());
		
#endif

	}
}

int InputManager::numMessages(){
	return _inputMsgQ.size();
}
