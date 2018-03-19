#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL.h>
#include <vector>
#include <queue>

class Message;

/*	This class defines an interface to comunicate with SDL Input (event) system.	*/

class InputManager
{
private:
	InputManager();
	void pushMessage(Message* newMessage);						//Push a new message to the local queue
	
	static InputManager* _instance;								//pointer to the unique instance of InputManager
	
	std::vector<SDL_Joystick*> _playersJoystick;				//Player 1 and Player 2 Joysticks structs 
	std::vector<SDL_GameController*> _playerController;			
	const int MAX_PLAYERS = 2;									//Max player input
	std::queue<Message*> _inputMsgQ;							//Local message queue

public:
	~InputManager();

	//Methods
	static InputManager& getInstance();							//Return the unique instance
	void handleInput();											//Main functiion. Read the input from SDL
	void getMessages(std::queue<Message*> &sceneQueue);			//Send input messages to another queue
	int numMessages();											//Return the number of currently messages in the queue

};
#endif

