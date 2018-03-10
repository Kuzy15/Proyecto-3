#include "Game.h"
#include <OgreWindowEventUtilities.h>
#include <OgreViewport.h>
#include <OgreRenderWindow.h>
#include <exception>
#include <OgreTextureManager.h>
#include "Scenes.h"


//Debug 
#ifdef _DEBUG
#include <iostream>
#endif



//Later removable
#include <OgreCamera.h>
#include <OgreNode.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>


#pragma region Constructor and destructor
 Game::Game(){

	 //Init Box2D physics environment
	 world = new b2World(GRAVITY);
	 timer = new b2Timer();
	 lastupdate = timer->GetMilliseconds();

	 totalTime = 0;
	 
	 initOgre();


	 actScene = new basicScene("testScene", this);
}
 Game::~Game(){

	 //Remove the game from the window listeners
	 Ogre::WindowEventUtilities::removeWindowEventListener(pWindow, this);
	 
	 //Delete the physics world
	 delete world;
	 world = nullptr;
 }
#pragma endregion

#pragma region Game Getters and setters
 Ogre::RenderWindow * Game::getRenderWindow(){
	 if (pWindow != NULL)return pWindow;
	 //else throw exception so we know no window is there
 }
 Ogre::Root * Game::getRoot(){
	 return root;
 }
#pragma endregion

#pragma region Ogre Game functions
bool Game::initOgre(){

	//Firstly we define wether we are using a resources and plugins file for debug
	// or for release. Then we create the root.
#ifdef _DEBUG
	resCfgLoc = "OgreD/resources_d.cfg";
	plugCfgLoc = "OgreD/plugins_d.cfg";
#else
	resCfgLoc = "Ogre/resources.cfg";
	plugCfgLoc = "Ogre/plugins.cfg";
#endif
	try{
		root = new Ogre::Root(plugCfgLoc);
	}
	catch (std::exception e){
		std::cout << e.what();
		std::cout << "aa";
		return false;
	}
	//------------------------------------------------------------------------------------------------------
	//Setting UP Resources 

	//Parsing the config file into the system.
	cf.load(resCfgLoc);


	//name: Path to resources in disk,
	//loctype: defines what kind of location the element is (e.g. Filesystem, zip..)
	Ogre::String name, locType;

	//We now iterate through rach section in the resources.cfg.
	//Sections are signaled as [NAME]
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
	while (secIt.hasMoreElements())
	{
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;

		//Now we are iterating INSIDE the section [secIt]
		for (it = settings->begin(); it != settings->end(); ++it)
		{
			locType = it->first;
			name = it->second;

			//We now know the type of the element and its path.
			//We add it as a location to the Resource Group Manager
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
			std::cout << "inserting:\n";
			std::cout << name << locType << std::endl;
		}
	}
	//If there is no previous Ogre.cfg, this displays the config dialog
	if (!(root->restoreConfig() || root->showConfigDialog()))
		return false;

	//------------------------------------------------------------------------------------------------------
	//Render Window Creation
	pWindow = root->initialise(true, "OGRE3D Game");



	//------------------------------------------------------------------------------------------------------
	//Resources Init

	//We are only going to use 5 mimpams at a time. Mipmaps are efficent ways to save a texture.
	//Taking only 1/3 more of space, we can have several sizes of the texture to choose from.
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	//Now we init every resource previously added
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();



	//We register game as a listener of the window events, to know if it's been closed
	Ogre::WindowEventUtilities::addWindowEventListener(pWindow, this);


 }
#pragma endregion


#pragma region Game Update Methods
/*Main loop of the game.
First we read the input.
Second we call the current scene logics.
Third we physically simulate the scene
Fourth we render (or not) the scene
*/
void Game::loop() {
	while (!pWindow->isClosed()){
		/*
			delta = timer->GetMilliseconds();

			cont += delta / 1000;
			lastupdate = timer->GetMilliseconds();

			handleInput();
			while (cont > 0.016){
			world->Step(FPS_CAP, 10, 2);
			nFrames++;

			//scenes.top()->update();
			cont -= 0.016;
			}

			//scenes.top()->update();

			*/
		actScene->run();
		render();
	}

}

//Método que renderiza
void Game::render() {

	
	
	Ogre::WindowEventUtilities::messagePump();
	if (pWindow->isClosed())return;
	if (!root->renderOneFrame())return;
	

}

//Read the input
void Game::handleInput(){

	



}

#pragma endregion