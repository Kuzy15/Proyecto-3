#include "Game.h"
#include <OgreWindowEventUtilities.h>
#include <OgreViewport.h>
#include <OgreRenderWindow.h>
#include <iostream>
#include <exception>
#include <OgreTextureManager.h>


//Later removable
#include <OgreCamera.h>
#include <OgreNode.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>


#pragma region Constructor and destructor
 Game::Game(){

	 
	 b2Vec2 gravity(1.0f, 1.0f);
	 b2World World(gravity);
	 b2BodyDef b;
	 
	 initOgre();
}
 Game::~Game(){


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



	//------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------
	/*
	THIS WILL BE CHANGED.
	JUST A BASIC CONSTRUCT OF A VIEWPORT
	
	*/
	Ogre::SceneManager * scnMgr = root->createSceneManager(Ogre::ST_GENERIC);
	Ogre::Camera * cam;
	Ogre::Viewport * vp;



	//Self-explanatory methods
	cam = scnMgr->createCamera("MainCam");
	cam->setPosition(0, 0, 80);
	cam->lookAt(0, 0, -300);
	cam->setNearClipDistance(5);


	//------------------------------------------------------------------------------------------------------
	//ViewPort Addition
	vp = pWindow->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(150, 150, 150));

	cam->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));



	//------------------------------------------------------------------------------------------------------
	//Scene SetUp
	try {

		Ogre::Entity * robot = scnMgr->createEntity("ogrehead.mesh");
		Ogre::SceneNode * robotNode = scnMgr->getRootSceneNode()->createChildSceneNode();
		robotNode->attachObject(robot);
	}
	catch (Ogre::FileNotFoundException e) {
		std::string a = e.getFullDescription();
		std::cout << a;
	}
	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	Ogre::Light * light;
	light = scnMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);



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
	
	render();

}
void Game::render() {

	while (true)
	{
		Ogre::WindowEventUtilities::messagePump();
		if (pWindow->isClosed())return;
		if (!root->renderOneFrame())return;
	}

}

#pragma endregion