#include "Game.h"

#ifndef SCENES_H
#define SCENES_H
class Entity;
class Message;

class gameScene
{
public:
	gameScene();
	~gameScene();

	//Getters and setters
	const Ogre::SceneManager * getSceneManager();


	//Entity Management
	void addEntity(Entity *);
	void deleteEntity(std::string id);

	//Comunication methods
	void getMessage(Message *);
	void dispatch();

private:	
	Ogre::SceneManager *scnMgr;
	std::list<Entity *> entities;
	std::queue<Message *> messages;


};



#endif