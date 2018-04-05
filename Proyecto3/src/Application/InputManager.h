#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL.h>
#include <vector>
#include <list>
#include "Messages.h"

//Const values for the deadzones of the controller's joystick
const float DEAD_ZONE = 50.0f;			//Then, the values are in the range [50,327] or [-50,-327]			
const float PARSE_VALUE = 100.0f;
//Max player
const int MAX_PLAYERS = 2;									

/*	This class defines an interface to comunicate with SDL Input (event) system.	*/

class InputManager
{
private:
	InputManager();
	void putMessage(InputStateMessage* newMessage);						//Push a new message to the local queue
	void addJoystick();											//Add a new Controller
	void deleteJoystick(int w);										//Delete a Controller
	
	static InputManager* _instance;								//pointer to the unique instance of InputManager
	
	std::vector<SDL_Joystick*> _playersJoystick = std::vector<SDL_Joystick*>(MAX_PLAYERS);				//Player 1 and Player 2 Joysticks structs 
	std::vector<SDL_GameController*> _playerController = std::vector<SDL_GameController*>(MAX_PLAYERS);;
	std::vector<Message*> _myQueue;																		//Internal list for messages.
	std::vector<InputStateMessage*> _inputMsg = std::vector<InputStateMessage*>(MAX_PLAYERS);							//Array for controllers input
	int _currentNPlayers;
	
	const std::string _emitter = "InputManager";

public:
	~InputManager();

	//Methods
	static InputManager& getInstance();							//Return the unique instance
	void handleInput();											//Main functiion. Read the input from SDL
	void getMessages(std::list<Message*> &sceneQueue);			//Send input messages to another queue
	int numMessages();											//Return the number of currently messages in the queue

};
#endif

