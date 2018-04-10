#include "InputManager.h"

#ifdef _DEBUG
#include <iostream>
#include <string>
#endif

InputManager* InputManager::_instance = nullptr;	//Definition of the instance 

InputManager::InputManager()
{

	_currentNPlayers = 0;

	//Open all the joystick files conected.
	/*
	int i = 0;
	while( i < SDL_NumJoysticks() && i < MAX_PLAYERS)
	{
		_playersJoystick[i] = ( SDL_JoystickOpen(i));
		if (_playersJoystick[i] == NULL && SDL_IsGameController(i)){
			//ERROR MESSAGE
		}
		_playerController[i] = (SDL_GameControllerOpen(i));
#ifdef _DEBUG
		printf("    %s\n", SDL_JoystickName(_playersJoystick[i]));
#endif
	
		i++;
	}
	*/
	//Enable event flow
	SDL_JoystickEventState(SDL_ENABLE);


#pragma endregion

}


InputManager::~InputManager()
{
	//Close opened Joysticks
	for (size_t i = 0; i < _currentNPlayers; i++){
		if (_playersJoystick[i] != NULL) SDL_JoystickClose(_playersJoystick[i]);
	}

	//Deleten unsended messages
	for (size_t i = 0; i < _myQueue.size(); i++){
		delete _myQueue[i];
		
	}
	//Close SDL
	SDL_Quit();
}

InputManager& InputManager::getInstance(){
	// Lazy initialize.
	if (InputManager::_instance == NULL) InputManager::_instance = new InputManager();
	return *InputManager::_instance;
}


//Send the local queue messages to the scene queue if there's any message
void InputManager::getMessages(std::list<Message*> &sceneQueue){
	for (int i = 0; i < _myQueue.size(); i++){
		sceneQueue.push_back(_myQueue[i]);
	}
	_myQueue.clear();
}

void InputManager::handleInput(){
	
	SDL_Event event;
	/* Other initializtion code goes here */

	//Create main Input message who contains the events and variables to store the values
	for (int i = 0; i < _currentNPlayers; i++){

		_inputMsg[i] = new InputStateMessage(i,BROADCAST, _emitter);

	}
	float axisMotion;

	/* Start main game loop here */
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			
			break;

		case SDL_QUIT:
			
			break;
//Mapping for controller buttons (Press and Release)			
#pragma region CONTROLLERBUTTONDOWN
		case SDL_CONTROLLERBUTTONDOWN:
			switch (event.cbutton.button)
			{
			case SDL_CONTROLLER_BUTTON_A:
				_inputMsg[event.cbutton.which]->getCInputState().Button_A = BTT_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_B:
				_inputMsg[event.cbutton.which]->getCInputState().Button_B = BTT_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_X:
				_inputMsg[event.cbutton.which]->getCInputState().Button_X = BTT_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				_inputMsg[event.cbutton.which]->getCInputState().Button_Y = BTT_PRESSED;
			case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
				_inputMsg[event.cbutton.which]->getCInputState().Left_Shoulder = BTT_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
				_inputMsg[event.cbutton.which]->getCInputState().Right_Shoulder = BTT_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
				_inputMsg[event.cbutton.which]->getCInputState().Right_Stick = BTT_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSTICK:
				_inputMsg[event.cbutton.which]->getCInputState().Left_Stick = BTT_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_START:
				_inputMsg[event.cbutton.which]->getCInputState().Button_Start = BTT_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_BACK:
				_inputMsg[event.cbutton.which]->getCInputState().Button_Back = BTT_PRESSED;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				_inputMsg[event.cbutton.which]->getCInputState().DPad_Down = BTT_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				_inputMsg[event.cbutton.which]->getCInputState().DPad_Up = BTT_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				_inputMsg[event.cbutton.which]->getCInputState().DPad_Left = BTT_PRESSED;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				_inputMsg[event.cbutton.which]->getCInputState().DPad_Right = BTT_PRESSED;
				break;
			default:
				break;
			}
			
			std::cout << "Button pressed" << std::endl;
			break;
#pragma endregion
#pragma region CONTROLLERBUTTONUP
		case SDL_CONTROLLERBUTTONUP:
			switch (event.cbutton.button)
			{
			case SDL_CONTROLLER_BUTTON_A:
				_inputMsg[event.cbutton.which]->getCInputState().Button_A = BTT_RELEASED;
				break;
			case SDL_CONTROLLER_BUTTON_B:
				_inputMsg[event.cbutton.which]->getCInputState().Button_B = BTT_RELEASED;
				break;
			case SDL_CONTROLLER_BUTTON_X:
				_inputMsg[event.cbutton.which]->getCInputState().Button_X = BTT_RELEASED;
				break;
			case SDL_CONTROLLER_BUTTON_Y:
				_inputMsg[event.cbutton.which]->getCInputState().Button_Y = BTT_RELEASED;
			case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
				_inputMsg[event.cbutton.which]->getCInputState().Left_Shoulder = BTT_RELEASED;
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
				_inputMsg[event.cbutton.which]->getCInputState().Right_Shoulder = BTT_RELEASED;
				break;
			case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
				_inputMsg[event.cbutton.which]->getCInputState().Right_Stick = BTT_RELEASED;
				break;
			case SDL_CONTROLLER_BUTTON_LEFTSTICK:
				_inputMsg[event.cbutton.which]->getCInputState().Left_Stick = BTT_RELEASED;
				break;
			case SDL_CONTROLLER_BUTTON_START:
				_inputMsg[event.cbutton.which]->getCInputState().Button_Start = BTT_RELEASED;
				break;
			case SDL_CONTROLLER_BUTTON_BACK:
				_inputMsg[event.cbutton.which]->getCInputState().Button_Back = BTT_RELEASED;
			case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
				_inputMsg[event.cbutton.which]->getCInputState().DPad_Down = BTT_RELEASED;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_UP:
				_inputMsg[event.cbutton.which]->getCInputState().DPad_Up = BTT_RELEASED;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
				_inputMsg[event.cbutton.which]->getCInputState().DPad_Left = BTT_RELEASED;
				break;
			case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
				_inputMsg[event.cbutton.which]->getCInputState().DPad_Right = BTT_RELEASED;
				break;
			default:
				break;
			}
			break;
#pragma endregion
//Mapping for both joysticks
		case SDL_CONTROLLERAXISMOTION:
			axisMotion = event.caxis.value / PARSE_VALUE;
			if ((axisMotion < -DEAD_ZONE) || (axisMotion > DEAD_ZONE))
			{
				if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX){
					_inputMsg[event.caxis.which]->getCInputState().Axis_LeftX = axisMotion;
				}
				else if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTY)
					_inputMsg[event.caxis.which]->getCInputState().Axis_LeftY = axisMotion;
					
				else if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX)
					_inputMsg[event.caxis.which]->getCInputState().Axis_RightX = axisMotion;
					
				else if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY)
					_inputMsg[event.caxis.which]->getCInputState().Axis_RightY = axisMotion;
			}
			break;
//Events for Controller recognize: connect and disconnect
		case SDL_CONTROLLERDEVICEADDED:
			addJoystick();
			_myQueue.push_back(new ControllerStateMessage(BROADCAST, _emitter, event.cdevice.which, 1));
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			deleteJoystick(event.cdevice.which);
			_myQueue.push_back(new ControllerStateMessage(BROADCAST, _emitter, event.cdevice.which, 0));
			break;
		
		default:
			break;
		}
#ifdef _DEBUG
		//PRINT DEBUG FOR CONSOLE:
		//printf(std::to_string(event.cbutton.button).c_str());
		
#endif

	}
	//Loop that takes the input mssages from the controllers if there's any and push them to the local queue.
	for (int i = 0; i < _currentNPlayers; i++){
		if (_inputMsg[i] != nullptr && _inputMsg[i]->getNumMessages() > 0) {
			_myQueue.push_back(_inputMsg[i]);
		}
		else{
			delete _inputMsg[i];
		}
		_inputMsg[i] = nullptr;
	}
	
}

int InputManager::numMessages(){

	int nMessages = 0;

	for (int i = 0; i < _currentNPlayers; i++){
		if (_inputMsg[i] != nullptr)
			nMessages += (int)_inputMsg[i]->getNumMessages();
	}
	return nMessages;
}

//Put new Joystick and GameController into the arrays
void InputManager::addJoystick(){
	int joyStickId = _currentNPlayers;
	_playersJoystick[joyStickId] = (SDL_JoystickOpen(joyStickId));
	if (_playersJoystick[joyStickId] == NULL && SDL_IsGameController(joyStickId)){
		//ERROR MESSAGE
	}
	_playerController[joyStickId] = (SDL_GameControllerOpen(joyStickId));
	_currentNPlayers++;
}

//Delete the disconnected Joystick
void InputManager::deleteJoystick(int wich){
	//Close opened Joysticks
	if (_playersJoystick[wich] != NULL){
		SDL_JoystickClose(_playersJoystick[wich]);
		SDL_GameControllerClose(_playerController[wich]);
		_currentNPlayers--;
	}
}