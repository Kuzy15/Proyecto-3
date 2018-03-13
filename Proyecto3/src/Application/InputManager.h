#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
#include <SDL.h>
#include <vector>

/*	This class defines an interface to comunicate with SDL Input (event) system.	*/


class InputManager
{
private:
	std::vector<SDL_Joystick*> _playersJoystick;	//Player 1 and Player 2 Joysticks structs 
	
	const int MAX_PLAYERS = 2;
public:
	InputManager();
	~InputManager();

	void handleInput();
};
#endif

