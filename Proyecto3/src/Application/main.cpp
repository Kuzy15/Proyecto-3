#include <stdio.h>
#include "Game.h"
#include "DebugNew.h"

#ifdef _DEBUG || !_WIN32
#define new DEBUG_NEW


int main(int argc, char *argv[]){
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Check Memory Leaks
#else 
#include <Windows.h>

int CALLBACK WinMain(
	//Copiarlo de internet
	_In_ HINSTANCE hInstance,
	_In_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow	
	){
#endif 

	Game * game;
	game = new Game();
	game->loop();
	delete game;

	
	
	return 0;
}
