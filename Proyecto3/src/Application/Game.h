#ifndef GAME_H
#define GAME_H


#include <OgreRoot.h>
#include <Box2D.h>
#include <OgreConfigFile.h>
#include <OgreWindowEventUtilities.h>
#include "irrKlang.h"
#include <OgreOverlaySystem.h>
#include <stack>
#include "EF_Entities.h"

using namespace irrklang;


class GameScene;
struct Player;


enum SCENES_ENUM { MAIN_MENU = 0, MULTIPLAYER = 1, GAMEPLAY = 2  };


//Game is a window event listener,
//Therefore we can react to the window events
class Game
{

public:
	Game();
	~Game();
	void loop();
	static Game * getInstance();

	Ogre::RenderWindow * getRenderWindow();
	Ogre::Root * getRoot();
	ISoundEngine* getSoundEngine();
	b2World* getPhysicsWorld();

	Ogre::OverlaySystem * getOverlaySystem();
	void setOverlaySystem(Ogre::OverlaySystem *);

	void newScene();
	void changeScene(SCENES_ENUM newScene, std::vector<Player> players = std::vector<Player>(), E_STAGE s = ES_TEMPLE );
	void popScene();


	void exitGame();


private:

	//Ogre parameters
	Ogre::Root * root;
	//Location and type of the resources and plugins files
	Ogre::String resCfgLoc;
	Ogre::String plugCfgLoc;
	//configFile we are going to use to parse our configuration to the game
	Ogre::ConfigFile cf;
	//window
	Ogre::RenderWindow * pWindow;

	Ogre::OverlaySystem * pOverSyst;
	Ogre::OverlayManager * pOverMan;

	//Sound engine pointer
	ISoundEngine* _soundEngine;


	
	//Stack of Scenes
	std::stack<GameScene*> scenes;
	SCENES_ENUM _nextScene;
	//Box2D parameters
	b2World* world;		//Pointer to the world
	const b2Vec2 GRAVITY = b2Vec2(0.0,-40.0f);
	const float FPS_CAP = 1.0f / 60.0f;		//World step time simulation (60 frames per second).

	//Game loop variables
	float currentTime;
	float newTime;
	float frameTime;
	float accumulator;
	float inputTime;


	bool _exit;
	bool _changeScene;



	//Singleton
	static Game * _instance;
	std::vector<Player> _nextPlayers;
	E_STAGE _nextStage;
	//internal private methods
	bool initOgre();
	bool initSound();
	bool initSDL();
	void render();
	void handleInput();

};


#endif
