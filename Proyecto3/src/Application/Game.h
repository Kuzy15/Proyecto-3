#ifndef GAME_H
#define GAME_H

#include <OgreRoot.h>
#include <Box2D.h>
#include <OgreConfigFile.h>

class Game
{
public:
	Game();
	~Game();
	void loop();



private:

	//Ogre parameters
	Ogre::Root * root;
	//Location and type of the resources and plugins files
	Ogre::String resCfgLoc;
	Ogre::String plugCfgLoc;
	//configFile we are going to use to parse our configuration to the game
	Ogre::ConfigFile cf;

	//Box2D parameters
	b2World* world;		//Pointer to the world
	const b2Vec2 GRAVITY = b2Vec2(0,9.8f);
	const float FPS_CAP = 1.0f / 60.0f;		//World step time simulation (60 frames per second).

	//Probando mierdas
	b2Timer* timer;
	float lastupdate;
	float delta;
	float cont;
	float totalTime;
	int nFrames;

	//window
	Ogre::RenderWindow * pWindow;

	
	bool initOgre();
	void render();
	void handleInput();

};


#endif