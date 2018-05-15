#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL.h>
#include <vector>
#include <list>
#include "Messages.h"

//Const values for the deadzones of the controller's joystick
const float PARSE_VALUE = 100.0f;
const float DEAD_ZONE = 50.0f;			//Then, the values are in the range [5,327] or [-5,-327]			
//Max player (no debe estar aquí)
const int MAX_PLAYERS = 2;									

/*	This class defines an interface to comunicate with SDL Input (event) system.	*/

class InputManager
{
private:
	InputManager();
	void putMessage(MInputState* newMessage);						//Push a new message to the local queue
	void addJoystick(int w);												//Add a new Controller
	void deleteJoystick(int w);										//Delete a Controller
	void updateControllersState(ControllerInputState &cState, int id);
	
	static InputManager* _instance;								//pointer to the unique instance of 
	
	std::vector<SDL_Joystick*> _playersJoystick = std::vector<SDL_Joystick*>(MAX_PLAYERS);				//Player 1 and Player 2 Joysticks structs 
	std::vector<SDL_GameController*> _playerController = std::vector<SDL_GameController*>(MAX_PLAYERS);;
	std::vector<Message*> _myQueue;																		//Internal list for messages.
	std::vector<MInputState*> _inputMsg = std::vector<MInputState*>(MAX_PLAYERS);							//Array for controllers input
	int _currentNPlayers;
	
	const std::string _emitter = "InputManager";

public:
	~InputManager();

	//Methods
	static InputManager& getInstance();							//Return the unique instance
	void static resetInstance();
	void handleInput();											//Main functiion. Read the input from SDL
	void getMessages(std::list<Message*> &sceneQueue);			//Send input messages to another queue
	int numMessages();											//Return the number of currently messages in the queue

};
#endif

