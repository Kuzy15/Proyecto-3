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
#ifdef _DEBUG
		printf("    %s\n", SDL_JoystickName(_playersJoystick[i]));
#endif
		if (_playersJoystick[i] == NULL){
			//ERROR MESSAGE
		}
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
	_inputMsgQ.push(newMessage);	
}

//Send messages to the scene queue if there's any message
void InputManager::getMessages(std::queue<Message*> &sceneQueue){
	while (!(_inputMsgQ.empty())){
		sceneQueue.push(_inputMsgQ.front());
		_inputMsgQ.pop();
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
			
			/* handle keyboard stuff here */
			break;

		case SDL_QUIT:
			/* Set whatever flags are necessary to */
			/* end the main game loop here */
			break;
		case SDL_JOYBUTTONDOWN:  /* Handle Joystick Button Presses */
			

			//printf(event.jbutton.button);
			if (event.jbutton.button == 0)
			{
				/* code goes here */
			}
			break;
		case SDL_JOYHATMOTION:
			if (event.jhat.value & SDL_HAT_UP)
			{
				/* Do up stuff here */
			}

			if (event.jhat.value & SDL_HAT_LEFT)
			{
				/* Do left stuff here */
			}

			if (event.jhat.value & SDL_HAT_RIGHTDOWN)
			{
				/* Do right and down together stuff here */
			}
			

			break;
		case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
			
			if ((event.jaxis.value < -3200) || (event.jaxis.value > 3200))
			{
				/* code goes here */
			}
			break;
		default:
			break;
		}
#ifdef _DEBUG
		//PRINT DEBUG FOR CONSOLE:
			Uint8 uP = event.jbutton.button;
			printf(std::to_string(uP).c_str());
		
#endif

	}
}

int InputManager::numMessages(){
	return _inputMsgQ.size();
}
