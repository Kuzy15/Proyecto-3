

#ifndef SCENES_H
#define SCENES_H
#include "Game.h"
class Entity;
class Message;

class gameScene
{
public:
	gameScene(std::string id);
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

private:
	std::string _id;


	//Ogre basic atributes
	Ogre::SceneManager *scnMgr;

	//Messaging attributes and methods
	std::list<Entity *> _entities;
	std::queue<Message *> _messages;




};



#endif