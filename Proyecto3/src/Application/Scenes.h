#ifndef SCENES_H
#define SCENES_H
class Entity;
class Message;
class Game;

/*----------------------------- GAME SCENE -----------------------------*/
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
	virtual void dispatch();

	//Main loop of the scene.
	//Each scene implements it differently
	virtual bool run() = 0;
	bool updateEnts(float delta);


protected:
	std::string _id;
	Game * pGame;
	int nMessages;

	void clearMessageQueue();
	//Messaging attributes and methods
	std::list<Entity *> _entities;
	std::list<Message *> _messages;


};

/*----------------------------- BASIC SCENE -----------------------------*/

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
	
	
};


#endif