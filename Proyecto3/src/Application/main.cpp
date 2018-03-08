#include <stdio.h>
#include "Game.h"

#ifdef _DEBUG || !_WIN32

int main(){
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

	Game * game ;
	game = new Game();
	

	delete game;
	
	return 0;
}
