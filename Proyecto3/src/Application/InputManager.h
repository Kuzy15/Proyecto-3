#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL.h>
#include <vector>
#include <list>
#include "Messages.h"



/*	This class defines an interface to comunicate with SDL Input (event) system.	*/

class InputManager
{
private:
	InputManager();
	void putMessage(InputMessage* newMessage);						//Push a new message to the local queue
	
	static InputManager* _instance;								//pointer to the unique instance of InputManager
	
	std::vector<SDL_Joystick*> _playersJoystick;				//Player 1 and Player 2 Joysticks structs 
	std::vector<SDL_GameController*> _playerController;			
	const int MAX_PLAYERS = 2;									//Max player input
	InputMessage* _inputMsg;							//Local message queue
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

