#include "InputManager.h"
#include "DebugNew.h"
#ifdef _DEBUG
#include <iostream>
#include <string>
#define new DEBUG_NEW
#endif


InputManager* InputManager::_instance = nullptr;	//Definition of the instance 

InputManager::InputManager()
{

	_currentNPlayers = 0;

	//Open all the joystick files conected.
	
	//Enable event flow
	SDL_JoystickEventState(SDL_ENABLE);

	lastState = std::vector<ControllerInputState>(2);

	receiving = true;
#pragma endregion

}


InputManager::~InputManager()
{
	//Close opened Joysticks
	for (size_t i = 0; i < MAX_PLAYERS; i++){
		if (_playersJoystick[i] != NULL) SDL_JoystickClose(_playersJoystick[i]);
	}

	//Deleten unsended messages
	for (size_t i = 0; i < _myQueue.size(); i++){
		delete _myQueue[i];
		
	}

	for (int i = 0; i < MAX_PLAYERS; i++){

		delete _inputMsg[i];

	}

	//Close SDL
	SDL_Quit();
}


void InputManager::stopReceiving(bool b){

	receiving = b;
}

InputManager& InputManager::getInstance(){
	// Lazy initialize.
	if (InputManager::_instance == nullptr) InputManager::_instance = new InputManager();
	return *InputManager::_instance;
}

void InputManager::resetInstance(){
	delete InputManager::_instance;
	_instance == nullptr;
}

//Send the local queue messages to the scene queue if there's any message
void InputManager::getMessages(std::list<Message*> &sceneQueue){
	for (int i = 0; i < _myQueue.size(); i++){
		sceneQueue.push_back(_myQueue[i]);
	}
	_myQueue.clear();
}

void InputManager::handleInput(){

	if (receiving){

		SDL_Event event;
		/* Other initializtion code goes here */

		//Create main Input message who contains the events and variables to store the values
		for (int i = 0; i < MAX_PLAYERS; i++){
			if (_playerController[i] != nullptr || i == 1) // PROBLEMA: si no está conectado el 2o mando no se crearan mensajes de input ni el struct ControllerInputState aunque esté el teclado. (|| i == 1)
				_inputMsg[i] = new MInputState(i, _emitter);

		}

		/* Start main game loop here */
		while (SDL_PollEvent(&event))
		{
			std::cout << "ahh";
			switch (event.type)
			{
			case SDL_KEYDOWN:
				std::cout << "ahh";
				break;
			case SDL_QUIT:
				break;
				//Events for Controller recognize: connect and disconnect
			case SDL_CONTROLLERDEVICEADDED:
				addJoystick(event.cdevice.which);
				_myQueue.push_back(new MControllerState(_emitter, event.cdevice.which, 1));
				break;
			case SDL_CONTROLLERDEVICEREMOVED:
				_myQueue.push_back(new MControllerState(_emitter, event.cdevice.which, 0));
				deleteJoystick(event.cdevice.which);
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
		for (int i = 0; i < MAX_PLAYERS; i++){
			if (_inputMsg[i] != nullptr && (_playerController[i] != nullptr || i == 1)) { // PROBLEMA TAMBIEN
				updateControllersState(_inputMsg[i]->getCInputState(), i);
				_myQueue.push_back(_inputMsg[i]);
			}
			else{
				delete _inputMsg[i];
			}
			_inputMsg[i] = nullptr;
		}
	}
}

int InputManager::numMessages(){

	int nMessages = 0;

	for (int i = 0; i < MAX_PLAYERS; i++){
		if (_inputMsg[i] != nullptr && _playerController[i] != nullptr)
			nMessages += (int)_inputMsg[i]->getNumMessages();
	}
	return nMessages;
}

//Put new Joystick and GameController into the arrays
void InputManager::addJoystick(int w){
	int joyStickId = w;
	_playersJoystick[joyStickId] = (SDL_JoystickOpen(joyStickId));
	if (_playersJoystick[joyStickId] == NULL && SDL_IsGameController(joyStickId)){
		//ERROR MESSAGE
	}
	_playerController[joyStickId] = (SDL_GameControllerOpen(joyStickId));
	
}

//Delete the disconnected Joystick
void InputManager::deleteJoystick(int wich){
	//Close opened Joysticks
	if (_playersJoystick[wich] != NULL){
		SDL_JoystickClose(_playersJoystick[wich]);
		_playersJoystick[wich] = nullptr;
		SDL_GameControllerClose(_playerController[wich]);
		_playerController[wich] = nullptr;
		
	}
}


void InputManager::updateControllersState(ControllerInputState &cState, int id){

	//evento de teclado false y si evento de teclado producio = true
	cState.eventKeyboard = false;

	SDL_Event event;

	while(SDL_PollEvent(&event) && id == 1){ // Only if its player 2 keyboard and mouse can be used.
		switch (event.type){
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_UP){
				cState.Key_Up = BTT_PRESSED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_DOWN){
				cState.Key_Down = BTT_PRESSED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_LEFT){
				cState.Key_Left = BTT_PRESSED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_RIGHT){
				cState.Key_Right = BTT_PRESSED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_RETURN){
				cState.Key_Enter = BTT_PRESSED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_a){
				cState.Key_A = BTT_PRESSED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_d){
				cState.Key_D = BTT_PRESSED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_SPACE){
				cState.Key_Space = BTT_PRESSED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_w){
				cState.Key_W = BTT_PRESSED;
				cState.eventKeyboard = true;
			}

		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_UP){
				cState.Key_Up = BTT_RELEASED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_DOWN){
				cState.Key_Down = BTT_RELEASED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_LEFT){
				cState.Key_Left = BTT_RELEASED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_RIGHT){
				cState.Key_Right = BTT_RELEASED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_RETURN){
				cState.Key_Enter = BTT_RELEASED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_a){
				cState.Key_A = BTT_RELEASED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_d){
				cState.Key_D = BTT_RELEASED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_SPACE){
				cState.Key_Space = BTT_RELEASED;
				cState.eventKeyboard = true;
			}
			if (event.key.keysym.sym == SDLK_w){
				cState.Key_W = BTT_RELEASED;
				cState.eventKeyboard = true;
			}

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT){
				cState.Mouse_X = event.button.x;
				cState.Mouse_Y = event.button.y;
			}
		}
	}

	//Axis
	cState.Axis_LeftX = SDL_GameControllerGetAxis(_playerController[id], SDL_CONTROLLER_AXIS_LEFTX) / PARSE_VALUE;
	if (cState.Axis_LeftX > -DEAD_ZONE && cState.Axis_LeftX < DEAD_ZONE)
		cState.Axis_LeftX = 0.0f;
	cState.Axis_LeftY = -SDL_GameControllerGetAxis(_playerController[id], SDL_CONTROLLER_AXIS_LEFTY) / PARSE_VALUE;
	if (cState.Axis_LeftY > -DEAD_ZONE && cState.Axis_LeftY < DEAD_ZONE)
		cState.Axis_LeftY = 0.0f;
	cState.Axis_RightX = SDL_GameControllerGetAxis(_playerController[id], SDL_CONTROLLER_AXIS_RIGHTX) / PARSE_VALUE;
	if (cState.Axis_RightX > -DEAD_ZONE && cState.Axis_RightX < DEAD_ZONE)
		cState.Axis_RightX = 0.0f;
	cState.Axis_RightY = -SDL_GameControllerGetAxis(_playerController[id], SDL_CONTROLLER_AXIS_RIGHTY) / PARSE_VALUE;
	if (cState.Axis_RightY > -DEAD_ZONE && cState.Axis_RightY < DEAD_ZONE)
		cState.Axis_RightY = 0.0f;


	//Triggers
	cState.Trigger_Left = SDL_GameControllerGetAxis(_playerController[id], SDL_CONTROLLER_AXIS_TRIGGERLEFT) / PARSE_VALUE;
	if (cState.Trigger_Left < DEAD_ZONE)
		cState.Trigger_Left = 0.0f;
	cState.Trigger_Right = SDL_GameControllerGetAxis(_playerController[id], SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / PARSE_VALUE;
	if (cState.Trigger_Right < DEAD_ZONE)
		cState.Trigger_Right = 0.0f;

	
	//Main Buttons
	cState.Button_A = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_A);
	if (cState.Button_A == BTT_NONE && lastState[id].Button_A == BTT_PRESSED) cState.Button_A = BTT_RELEASED;
	cState.Button_B = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_B);
	if (cState.Button_B == BTT_NONE && lastState[id].Button_B == BTT_PRESSED) cState.Button_B = BTT_RELEASED;
	cState.Button_X = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_X);
	if (cState.Button_X == BTT_NONE && lastState[id].Button_X == BTT_PRESSED) cState.Button_X = BTT_RELEASED;
	cState.Button_Y = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_Y);
	if (cState.Button_Y == BTT_NONE && lastState[id].Button_Y == BTT_PRESSED) cState.Button_Y = BTT_RELEASED;
	cState.Left_Shoulder = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	if (cState.Left_Shoulder == BTT_NONE && lastState[id].Left_Shoulder == BTT_PRESSED) cState.Left_Shoulder = BTT_RELEASED;
	cState.Right_Shoulder = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
	if (cState.Right_Shoulder == BTT_NONE && lastState[id].Right_Shoulder == BTT_PRESSED) cState.Right_Shoulder = BTT_RELEASED;

	//Pads
	cState.DPad_Down = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	if (cState.DPad_Down == BTT_NONE && lastState[id].DPad_Down == BTT_PRESSED) cState.DPad_Down = BTT_RELEASED;
	cState.DPad_Up = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_DPAD_UP);
	if (cState.DPad_Up == BTT_NONE && lastState[id].DPad_Up == BTT_PRESSED) cState.DPad_Up = BTT_RELEASED;
	cState.DPad_Left = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	if (cState.DPad_Left == BTT_NONE && lastState[id].DPad_Left == BTT_PRESSED) cState.DPad_Left = BTT_RELEASED;
	cState.DPad_Right = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	if (cState.DPad_Right == BTT_NONE && lastState[id].DPad_Right == BTT_PRESSED) cState.DPad_Right = BTT_RELEASED;


	//Other Buttons
	cState.Button_Back = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_BACK);
	if (cState.Button_Back == BTT_NONE && lastState[id].Button_Back == BTT_PRESSED) cState.Button_Back = BTT_RELEASED;
	cState.Button_Start = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_START);
	if (cState.Button_Start == BTT_NONE && lastState[id].Button_Start == BTT_PRESSED) cState.Button_Start = BTT_RELEASED;
	cState.Left_Stick = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_LEFTSTICK);
	if (cState.Left_Stick == BTT_NONE && lastState[id].Left_Stick == BTT_PRESSED) cState.Left_Stick = BTT_RELEASED;
	cState.Right_Stick = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_RIGHTSTICK);
	if (cState.Right_Stick == BTT_NONE && lastState[id].Right_Stick == BTT_PRESSED) cState.Right_Stick = BTT_RELEASED;

	lastState[id] = cState;
}