#include <stdio.h>
#include <OgreRoot.h>

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

	Ogre::Root *root;
#ifdef _DEBUG

	root = new Ogre::Root("OgreD/plugins_d.cfg");
#else
	root = new Ogre::Root("Ogre/plugins.cfg");
#endif

	//printf("Hola mundo\n");

	
	return 0;
}
