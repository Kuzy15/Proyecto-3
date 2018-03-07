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

	
	void initOgre();

};


#endif