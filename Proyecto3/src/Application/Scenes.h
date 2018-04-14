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
	virtual ~gameScene();


	//Entity Management
	void addEntity(Entity *);
	void deleteEntity(std::string id);

	//Comunication methods
	void getMessage(Message *);
	virtual void dispatch();
	virtual void processScnMsgs() = 0;

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
	std::list<Message *> _sceneMessages;


};

/*----------------------------- BASIC SCENE -----------------------------*/

//Basic class to debug and test the ogre implementation
//and the behaviour of the components
class basicScene: public gameScene
{
public:
	basicScene(std::string id, Game * game);
	virtual ~basicScene();


	virtual bool run();
	virtual void dispatch();
	void processScnMsgs();
	inline Ogre::SceneManager* getScnMngr(){ return scnMgr; }

private:
	Ogre::SceneManager * scnMgr;
	Ogre::Viewport * vp;
	Ogre::Camera * cam;
	Ogre::Light * light;
	
	
};

/*----------------------------- GAMEPLAY SCENE -----------------------------*/

/*
This scene subdivides in 3 diferente states that manage the proccess to battle:
	1. The "LoadOut" menu, where the players choose the God and the deck of carts (active and passive) to play.
	2. The "Battle" state, where the actions happens.
	3. The "End" menu that pops when one of the playes wins.

The scene itself is a state machine. 
To manage all this data, the scene stores information of the game (like number of players, state of battle, etc).
 
					EL NOMBRE DE LOS ESTADOS NO ES DEFINITIVO
*/

//Definition of the 3 possible states.
typedef enum GameplayState{
	GS_SETUP, GS_BATTLE, GS_END, GS_STOPPED
};
//Struct and global variables that stores the battle information
const int MAX_ROUNDS = 3;
const float TIME_LIMIT = 0;

struct BattleState{
	bool battleStarted = false; //Bool battle started
	bool battleEnded = false;
	float timeElapsed = 0;		//Time elapsed sice the start of the battle
	int roundsCompleted = 0;	//Rounds completed
	float timeCountStart = 0;	//Time when battle started
};


class GamePlayScene : public gameScene
{
public:
	GamePlayScene(std::string id, Game * game, int nPlayers);
	virtual ~GamePlayScene();

	virtual bool run();
	virtual void dispatch();
	virtual void processScnMsgs();

private:
	//Methods that manages the state of the scene
	void loadOut();
	void battle();
	void end();
	void controllerDisconected(int id);

	Ogre::SceneManager * scnMgr;
	Ogre::Viewport * vp;
	Ogre::Camera * cam;
	Ogre::Light * light;

	GameplayState _currState;	//The current state of the scene
	BattleState _bS;		//The state of the battle
	int _nPlayers;				//Number of players
	std::vector<Entity*> _players = std::vector<Entity*>(4);	//Array of pointer to the players Entities
	std::vector<bool> _pReady = std::vector<bool>(4, false);			//Array that show if players are ready to play
	bool _paused;

};

#endif