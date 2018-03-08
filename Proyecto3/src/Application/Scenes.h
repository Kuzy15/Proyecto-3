

#ifndef SCENES_H
#define SCENES_H
#include "Game.h"
class Entity;
class Message;

//Father class to every scene in the game.
//Implements the basic attributes shared by every scene.
class gameScene
{
public:
	gameScene(std::string id, Game * pGame);
	~gameScene();


	//Entity Management
	void addEntity(Entity *);
	void deleteEntity(std::string id);

	//Comunication methods
	void getMessage(Message *);
	virtual void dispatch() = 0;

	//Main loop of the scene.
	//Each scene implements it differently
	virtual bool run() = 0;

protected:
	std::string _id;
	Game * pGame;


	//Messaging attributes and methods
	std::list<Entity *> _entities;
	std::queue<Message *> _messages;


};
//Basic class to debug and test the ogre implementation
//and the behaviour of the components
class basicScene: public gameScene
{
public:
	basicScene(std::string id, Game * game);
	~basicScene();


	virtual bool run();
	virtual void dispatch();

private:
	Ogre::SceneManager * scnMgr;
	Ogre::Viewport * vp;
	Ogre::Camera * cam;
	Ogre::Light * light;
	
	bool updateEnts(float delta);
};


#endif