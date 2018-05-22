#ifndef SCENES_H
#define SCENES_H
#include <OgreTextAreaOverlayElement.h>

class Entity;
class Message;
class Game;
class b2Body;
enum E_GOD;
enum E_STAGE;
enum ComponentType;
struct ControllerInputState;



#pragma region Game Scene

/*----------------------------- GAME SCENE -----------------------------*/
//Father class to every scene in the game.
//Implements the basic attributes shared by every scene.
class GameScene
{
public:
	GameScene(std::string id, Game * pGame);
	virtual ~GameScene();


	//Entity Management
	void addEntity(Entity *);
	void deleteEntity(std::string id);
	void deleteAllEntities();

	//Comunication methods
	void getMessage(Message *);
	Ogre::SceneManager * getSceneManager();
	Game* getGame();
	virtual void dispatch();
	virtual void processScnMsgs() = 0;

	//Main loop of the scene.
	//Each scene implements it differently
	virtual bool run() = 0;
	bool updateEnts(float delta);

	//Debug
	void clearDebugDraw();

	//Box2d Bodies delete
	void addBodyToDelete(b2Body* b);
	void addEntityToDelete(Entity* e);

protected:
	std::string _id;
	Game * pGame;
	int nMessages;

	void clearMessageQueue();
	void deleteAllMessages();
	//Messaging attributes and methods
	std::map<std::string,Entity *> _entities;
	std::map<int,Entity*> _menuEntities;
	std::vector<Entity *> _entitiesToDelete;
	std::list<Message *> _messages;
	std::list<Message *> _sceneMessages;
	std::vector<b2Body*> _physicBodies;

	//Basic Ogre atributes common to every scene
	Ogre::SceneManager * scnMgr;
	Ogre::Viewport * vp;
	Ogre::Overlay* overlay;


	//List of bodies to destruct at the end of the frame.
	void destroyBodies();
	void destroyEntities();

};

#pragma endregion

#pragma region Basic Scene
/*----------------------------- BASIC SCENE -----------------------------*/

//Basic class to debug and test the ogre implementation
//and the behaviour of the components
class BasicScene: public GameScene
{
public:
	BasicScene(std::string id, Game * game);
	virtual ~BasicScene();


	virtual bool run();
	virtual void dispatch();
	void processScnMsgs();
	

private:
	Ogre::Light * light;
	int i;
	Ogre::TextAreaOverlayElement* textArea;
	Ogre::OverlayContainer* panel;
	
};

#pragma endregion

#pragma region GameplayScene

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

struct Player{
	
	Entity* entity = nullptr;
	int controllerId = -1;
	E_GOD god;
	int roundsWon = 0;
	std::vector<ComponentType> abilities;
	ComponentType currentActive; // = default
	ComponentType currentPassive;
};


class GamePlayScene : public GameScene
{
public:
	GamePlayScene(std::string id, Game * game, std::vector<Player> players, E_STAGE stage);
	virtual ~GamePlayScene();

	virtual bool run();
	virtual void dispatch();
	virtual void processScnMsgs();

private:
	//Methods that manages the state of the scene
	void preparePhase();
	void battlePhase();
	void endPhase();
	void changePhase(GameplayState);
	//Load the stage
	void loadStage();
	//Load the ability cards entities for each player
	void loadAbilities();
	void addAbilityComponent(int playerId, ComponentType id, int type); //Type values (0 = Active), (1 = Passive)
	//Controller manage methods
	void controllerDisconected(int id);
	void controllerConnected(int id);

	//ProcessMessages
	void processMsgSetup(Message* m);
	void processMsgBattle(Message* m);
	void processMsgEnd(Message* m);
	void playerDied(std::string playerDead);

	Ogre::Light * light;

	GameplayState _currState;	//The current state of the scene
	BattleState _battleState;		//The state of the battle
	E_STAGE _stage;			//Stage type
	const int TOTAL_ROUNDS = 3;
	int _nPlayers;				//Number of players
	std::vector<Player> _players;	//Array of pointer to the players Entities
	std::vector<bool> _pReady = std::vector<bool>(4, false);			//Array that show if players are ready to play
	bool _paused;
	std::list<Entity*> _cardGUIEntities;		//Buttons for card select entities

	Ogre::Overlay* bgCards;

	float _prepareCounter;
	float _prepareLimitTime;


};


#pragma endregion


#pragma region Main Menu Scene
/*----------------------------- Main Menu SCENE -----------------------------*/

//Basic class to debug and test the ogre implementation
//and the behaviour of the components
class MainMenuScene : public GameScene
{
public:
	MainMenuScene(std::string id, Game * game);
	virtual ~MainMenuScene();


	virtual bool run();
	virtual void dispatch();
	void processScnMsgs();


private:

	void processInput(ControllerInputState c);

	int selectedButton;
	

};

#pragma endregion

#pragma region Fight Menu Scene
/*----------------------------- FIGHT MENU SCENE -----------------------------*/

//Basic class to debug and test the ogre implementation
//and the behaviour of the components
class FightMenuScene : public GameScene
{
public:
	FightMenuScene(std::string id, Game * game);
	virtual ~FightMenuScene();


	virtual bool run();
	virtual void dispatch();
	void processScnMsgs();


private:

	void processInput(ControllerInputState c);

	int selectedButton;


};

#pragma endregion

#pragma region MultiplayerScene

/*----------------------------- Multiplayer SCENE -----------------------------*/

/*
This scene subdivides in 2 diferente states:
1. The "Champ Select", where the players choose the God and the deck.
2. The "Map Select" state.
3. The "Loading" .
*/

//Definition of the 3 possible states.
typedef enum MultiplayerState{
	MS_CHAMP_SELECT, MS_MAP_SELECT, MS_LOADING
};


class MultiplayerScene : public GameScene
{
public:
	MultiplayerScene(std::string id, Game * game);
	virtual ~MultiplayerScene();

	virtual bool run();
	virtual void dispatch();
	virtual void processScnMsgs();

private:

	void showChampGui();
	void showMapGui();
	void godSelect(Message* m);
	void mapSelect(Message* m);

	MultiplayerState state;
	std::vector<Player> players;
	E_STAGE stage;

};


#pragma endregion

#pragma region Loading Scene

/*----------------------------- Loading SCENE -----------------------------*/
//Scenes that show an loading screen between main scenes.

class LoadingScene : public GameScene
{
public:
	LoadingScene(std::string id, Game * game, GameScene* nextScene);
	virtual ~LoadingScene();

	virtual bool run();
	virtual void dispatch();
	virtual void processScnMsgs();

private:
	GameScene* _nextScene;
	float _timeLimit;
	float _counter;

};


#pragma endregion
#endif