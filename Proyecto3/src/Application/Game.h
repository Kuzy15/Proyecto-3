#ifndef GAME_H
#define GAME_H

#include <OgreRoot.h>
#include <Box2D.h>
#include <OgreConfigFile.h>
#include <OgreWindowEventUtilities.h>



class gameScene;

//Game is a window event listener,
//Therefore we can react to the window events
class Game :
	public Ogre::WindowEventListener
{

public: 
	Game();
	~Game();
	void loop();
	Ogre::RenderWindow * getRenderWindow();
	Ogre::Root * getRoot();
	b2World* getPhysicsWorld();


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


	//Box2D parameters
	b2World* world;		//Pointer to the world
	const b2Vec2 GRAVITY = b2Vec2(-0.0,-1.0f);
	const float FPS_CAP = 1.0f / 60.0f;		//World step time simulation (60 frames per second).

	//Input manager parameter
	//InputManager iM;

	//Game loop variables
	float currentTime;
	float newTime;
	float frameTime;
	float accumulator;
	


	//This will be removed. Just to test
	gameScene * actScene;



	//internal private methods
	bool initOgre();
	void render();
	void handleInput();

};


#endif