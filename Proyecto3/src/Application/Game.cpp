#include <algorithm>
#include <OgreWindowEventUtilities.h>
#include <OgreViewport.h>
#include <OgreRenderWindow.h>
#include <exception>
#include <OgreTextureManager.h>


#include "Scenes.h"
#include "InputManager.h"
#include "Game.h"
#include "CollisionManager.h"
#include "DebugNew.h"
#include "EF_Entities.h"

#include <OgreFontManager.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayManager.h>
#include <exception>
#include "EntityFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



//Debug
#ifdef _DEBUG
#include <iostream>
#endif

Game * Game::_instance = nullptr;
CollisionManager collisionManager;

using namespace irrklang;

#pragma region Constructor and destructor
Game::Game(){
	_instance = this;

	// start the sound engine with default parameters
	_soundEngine = createIrrKlangDevice();

	if (!_soundEngine){
#ifdef _DEBUG
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
#endif
		exit(1);
	}

	//Init Box2D physics environment
	world = new b2World(GRAVITY);
	//world->SetAllowSleeping(false);
	world->SetContactListener(&collisionManager);






	//Inicialization of SDL. Only starts JOYSTICK functionality.
	if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0){
#ifdef _DEBUG
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
#endif
		exit(1);
	}

	 currentTime = newTime = frameTime = accumulator = inputTime = 0;

	 initOgre();


	 _nextPlayers = std::vector<Player>();
	 _nextStage = ES_TEMPLE;

	 _exit = false;
	 _changeScene = false;


	 //actScene = new GamePlayScene("GamePlayScene", this, (*players), ES_ISLANDS);
	 GameScene* actScene = new initScene(resCfgLoc);//new MainMenuScene("MainMenuScene", this);
	 scenes.push(actScene);



}
 Game::~Game(){


	 while (!scenes.empty()){
		 GameScene* s = scenes.top();
		 scenes.pop();
		 delete s;

	 }
	 //Remove the game from the window listeners
	 //Ogre::WindowEventUtilities::removeWindowEventListener(pWindow, this);

	 //Delete the physics world
	 delete world;
	 world = nullptr;

	 InputManager::resetInstance();

	 EntityFactory::getInstance().resetInstance();
	 /*if (root != nullptr)
		delete root;*/

 }
#pragma endregion

#pragma region Game Getters and setters
 Ogre::RenderWindow * Game::getRenderWindow(){
	 if (pWindow != NULL)return pWindow;
	 //else throw exception so we know no window is there
 }
 Ogre::Root * Game::getRoot(){
	 return Ogre::Root::getSingletonPtr();
 }
 b2World* Game::getPhysicsWorld(){
	 return world;
 }
 Ogre::OverlaySystem * Game::getOverlaySystem() {
	 return pOverSyst;
 };

 void Game::popScene(){

	 if (!scenes.empty()){
		 GameScene* s = scenes.top();
		 scenes.pop();
		 delete s;
	 }
 }
 void Game::newScene(){

	 GameScene* s;
	 switch (_nextScene)
	 {
	 case GAMEPLAY:
		 s = new GamePlayScene("GameplayScene", this, _nextPlayers, _nextStage);
		 break;
	 case MAIN_MENU:
		 s = new MainMenuScene("MainMenuScene", this);
		 break;
	 case MULTIPLAYER:
		 s = new SelectGodScene("MultiplayerScene", this);
		 break;
	 default:
		 break;
	 }
	scenes.push(s);


 }

 void Game::changeScene(SCENES_ENUM newScene, std::vector<Player> players , E_STAGE s ){
	 _changeScene = true;
	 _nextScene = newScene;

	 _nextStage = s;
	 _nextPlayers = players;
 }
#pragma endregion


 Game * Game::getInstance() {
	 if (_instance == nullptr)_instance =  new Game();
	return _instance;
 }

 ISoundEngine* Game::getSoundEngine(){
	 return _soundEngine;
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
#ifdef _DEBUG
		std::cout << "-------------------------------------------- "<< std::endl;
		std::cout << e.what()<< std::endl;
#endif
		return false;
	}

	pOverSyst = OGRE_NEW Ogre::OverlaySystem();
	pOverMan = Ogre::OverlayManager::getSingletonPtr();


	if (!(root->restoreConfig() || root->showConfigDialog()))
		return false;

	//------------------------------------------------------------------------------------------------------
	//Render Window Creation
	pWindow = root->initialise(true, "OGRE3D Game");

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("../Media/Init", "FileSystem", "Essential");
	try {
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Essential");
	}
	catch (Ogre::Exception e)
	{
#ifdef _DEBUG

		std::cout << e.what() << std::endl;
#endif // _DEBUG

	}

 }
#pragma endregion


 void Game::exitGame(){
	 _exit = true;
 }

#pragma region Game Update Methods
/*Main loop of the game.
First we read the input.
Second we call the current scene logics.
Third we physically simulate the scene
Fourth we render (or not) the scene
*/
void Game::loop() {

	currentTime = (SDL_GetTicks() / 1000.0);
	int frames = 0;


	while (!pWindow->isClosed() && !_exit){

		if (_changeScene){
			popScene();
			newScene();
			_changeScene = false;
		}

		//Refresh loop parameters
		newTime = (SDL_GetTicks() / 1000.0);
		frameTime = newTime - currentTime;
		currentTime = newTime;
		accumulator += frameTime;

		//Loop for game logic and physic step (60 times per second)
		while (accumulator >= FPS_CAP){

			handleInput();
			world->Step(FPS_CAP, 10, 2);
			if (!scenes.empty())
				scenes.top()->run();
			accumulator -= FPS_CAP;
			frames++;
		}
		//std::cout << frames << std::endl;
#ifdef DEBUG
		world->DrawDebugData();
#endif // DEBUG

		render();

#ifdef DEBUG
		actScene->clearDebugDraw();
#endif // DEBUG

	}

}

//M�todo que renderiza
void Game::render() {

	Ogre::WindowEventUtilities::messagePump();
	if (pWindow->isClosed())
		return;
	if (!root->renderOneFrame())return;


}

//Read the input
void Game::handleInput(){ InputManager::getInstance().handleInput();}



#pragma endregion
