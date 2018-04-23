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

	SDL_Event event;
	/* Other initializtion code goes here */

	//Create main Input message who contains the events and variables to store the values
	for (int i = 0; i < _currentNPlayers; i++){

		_inputMsg[i] = new MInputState(i,BROADCAST, _emitter);

	}

	/* Start main game loop here */
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			break;
		case SDL_QUIT:
			break;			
//Events for Controller recognize: connect and disconnect
		case SDL_CONTROLLERDEVICEADDED:
			addJoystick();
			_myQueue.push_back(new MControllerState(BROADCAST, _emitter, event.cdevice.which, 1));
			break;
		case SDL_CONTROLLERDEVICEREMOVED:
			deleteJoystick(event.cdevice.which);
			_myQueue.push_back(new MControllerState(BROADCAST, _emitter, event.cdevice.which, 0));
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
		if (_inputMsg[i] != nullptr) {
			updateControllersState(_inputMsg[i]->getCInputState(),i);
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


void InputManager::updateControllersState(ControllerInputState &cState, int id){

	//Axis
	cState.Axis_LeftX = SDL_GameControllerGetAxis(_playerController[id], SDL_CONTROLLER_AXIS_LEFTX) / PARSE_VALUE;
	if (cState.Axis_LeftX > -DEAD_ZONE && cState.Axis_LeftX < DEAD_ZONE)
		cState.Axis_LeftX = 0;
	cState.Axis_LeftY = SDL_GameControllerGetAxis(_playerController[id], SDL_CONTROLLER_AXIS_LEFTY) / PARSE_VALUE;
	if (cState.Axis_LeftY > -DEAD_ZONE && cState.Axis_LeftY < DEAD_ZONE)
		cState.Axis_LeftY = 0;
	cState.Axis_RightX = SDL_GameControllerGetAxis(_playerController[id], SDL_CONTROLLER_AXIS_RIGHTX) / PARSE_VALUE;
	if (cState.Axis_RightX > -DEAD_ZONE && cState.Axis_RightX < DEAD_ZONE)
		cState.Axis_RightX = 0;
	cState.Axis_RightY = SDL_GameControllerGetAxis(_playerController[id], SDL_CONTROLLER_AXIS_RIGHTY) / PARSE_VALUE;
	if (cState.Axis_RightY > -DEAD_ZONE && cState.Axis_RightY < DEAD_ZONE)
		cState.Axis_RightY = 0;


	//Triggers
	cState.Trigger_Left = SDL_GameControllerGetAxis(_playerController[id], SDL_CONTROLLER_AXIS_TRIGGERLEFT) / PARSE_VALUE;
	if (cState.Trigger_Left < DEAD_ZONE)
		cState.Trigger_Left = 0;
	cState.Trigger_Right = SDL_GameControllerGetAxis(_playerController[id], SDL_CONTROLLER_AXIS_TRIGGERRIGHT) / PARSE_VALUE;
	if (cState.Trigger_Right < DEAD_ZONE)
		cState.Trigger_Right = 0;

	
	//Main Buttons
	cState.Button_A = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_A);
	cState.Button_B = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_B);
	cState.Button_X = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_X);
	cState.Button_Y = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_Y);
	cState.Left_Shoulder = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
	cState.Right_Shoulder = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);

	//Pads
	cState.DPad_Down = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	cState.DPad_Up = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_DPAD_UP);
	cState.DPad_Left = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	cState.DPad_Right = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

	//Other Buttons
	cState.Button_Back = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_BACK);
	cState.Button_Start = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_START);
	cState.Left_Stick = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_LEFTSTICK);
	cState.Right_Stick = (ButtonState)SDL_GameControllerGetButton(_playerController[id], SDL_CONTROLLER_BUTTON_RIGHTSTICK);
}