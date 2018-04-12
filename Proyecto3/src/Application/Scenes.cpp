#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreRenderWindow.h>

//Debug 
#ifdef _DEBUG
#include <iostream>
#endif



//Later removable
#include <OgreCamera.h>
#include <OgreNode.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <Components.h>

#include <Entity.h>
#include "InputManager.h"
#include "EntityFactory.h"
#include "Game.h"
#include "Messages.h"
#include "Scenes.h"


#pragma region gameScene 

gameScene::gameScene(std::string id, Game * game) :_id(id), pGame(game){
	nMessages = -1;
}
gameScene::~gameScene()
{
	Entity * del;
	for (Entity * ent : _entities){
		del = ent;
		_entities.pop_front();
		delete ent;
	}

}
bool gameScene::updateEnts(float delta){
	for (auto ent : _entities){
		if(ent != NULL)ent->tick(delta);
	}
	return true;
} 
void gameScene::getMessage(Message * m){
	_messages.push_back(m);
}
void gameScene::addEntity(Entity * ent){
	_entities.push_back(ent);
}
void gameScene::dispatch() {
	/*We only process as many messages as we had at the start of the update.
	*Any messsage introduced as a result of reading a message is processed
	in the next frame*/
	nMessages = _messages.size();
	std::list<Message *>::iterator it =_messages.begin();
	for (int i = 0; i < nMessages && it != _messages.end(); i++, it++) {
		if ((*it)->getDestination() == SCENE)
			_sceneMessages.push_back((*it));
		for (Entity * aux : _entities) {
			if((*it)->getEmmiter() != aux->getID())
				aux->getMessage(*it);
		}
	}
}
//Message disposal to use just before finishing the current scene loop.
// It uses the variable nMessages, that is set in the dispatch function.
void gameScene::clearMessageQueue() {
	
	for (int i = 0; i < nMessages; i++) {
		Message * aux = _messages.front();
		delete aux;
		_messages.pop_front();
	}
}
void gameScene::deleteEntity(std::string id){
	Entity * aux;
	bool found = false;
	for (std::list<Entity *>::iterator it = _entities.begin(); it != _entities.end() && !found;){
		if ((*it)->getID() == id){
			aux = *it;
			it = _entities.erase(it);
			delete aux;
			found = false;
		}
		else it++;
	}
}
#pragma endregion

//BASIC SCENE TO TEST SCENE IMPLEMENTATION.
//BUILDS A OGREHEAD BUT DOES NOT INCLUDE IT IN THE 
//SCENE ENTITY LIST. IT IS ONLY IN OGRE ENTITIES LIST.
//ALSO CREATS A BASIC ENTITY WITH A stringComponent attached
#pragma region basicScene
basicScene::basicScene(std::string id, Game * game): gameScene(id, game) {
	scnMgr = pGame->getRoot()->createSceneManager(Ogre::ST_GENERIC);


	//Self-explanatory methods
	cam = scnMgr->createCamera("MainCam");
	cam->setPosition(0, 0, 150);
	cam->lookAt(0, 0, -300);
	cam->setNearClipDistance(5);


	//------------------------------------------------------------------------------------------------------
	//ViewPort Addition
	vp = game->getRenderWindow()->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(150, 150, 150));

	cam->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));
	
	
	//------------------------------------------------------------------------------------------------------
	//Scene SetUp

/*	try {
		Ogre::Entity * robot = scnMgr->createEntity("ogrehead.mesh");
		Ogre::SceneNode * robotNode = scnMgr->getRootSceneNode()->createChildSceneNode();
		robotNode->attachObject(robot);
	}
	catch (Ogre::FileNotFoundException e) {
		std::string a = e.getFullDescription();
		std::cout << a;
	}
	*/
	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	light = scnMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);
	

	//SCENE DEBUG
	
	Entity * test1 = new Entity("test1", this);
	Entity * test2 = new Entity("test2", this);
	Entity * test3 = new Entity("test3", this);
	Entity * test4 = EntityFactory::getInstance().createEntity(ET_GOD, EG_RA, this);

	test3->addComponent(new RigidBodyComponent(test3, game->getPhysicsWorld(), Ogre::Vector3(0, 0, 100), 1.5, 20, STATIC, POLYGON));
	test3->addComponent(new meshRenderComponent(Ogre::Vector3(0, 0, 100), "Barrel.mesh", test3, scnMgr));


	test2->addComponent(new stringComponent(test2));
	
	


	test1->addComponent(new RigidBodyComponent(test1, game->getPhysicsWorld(), Ogre::Vector3(0, 40, 100), 5,20,DYNAMIC,POLYGON));
	test1->addComponent(new meshRenderComponent(Ogre::Vector3(0,40,100),"Ra.mesh", test1, scnMgr));

	
	addEntity(test1);
	addEntity(test2);
	addEntity(test3);
	addEntity(test4);
	
}


basicScene::~basicScene(){
	delete light;
	delete cam;
	delete vp;
	delete scnMgr;

}
bool basicScene::run(){
	//Here we would get the time between frames

	//Take messages from input
	InputManager::getInstance().getMessages(_messages);
	//Then we deliver the messages
	gameScene::dispatch();

	//Logic simulation done here
	bool aux = updateEnts(0.025);

	//Clear dispatched messages
	clearMessageQueue();

	return aux;

}

void basicScene::dispatch(){


}

void basicScene::processScnMsgs(){};

#pragma endregion
#pragma region GamePlayScene
//Scene that runs and manage the battle phase of the game.
GamePlayScene::GamePlayScene(std::string id, Game * game, int nP) : gameScene(id, game) {
	scnMgr = pGame->getRoot()->createSceneManager(Ogre::ST_GENERIC);


	//Self-explanatory methods
	cam = scnMgr->createCamera("MainCam");
	cam->setPosition(0, 0, 150);
	cam->lookAt(0, 0, -300);
	cam->setNearClipDistance(5);


	//------------------------------------------------------------------------------------------------------
	//ViewPort Addition
	vp = game->getRenderWindow()->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(150, 150, 150));

	cam->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));


	//------------------------------------------------------------------------------------------------------
	//Scene SetUp

	/*	try {
	Ogre::Entity * robot = scnMgr->createEntity("ogrehead.mesh");
	Ogre::SceneNode * robotNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	robotNode->attachObject(robot);
	}
	catch (Ogre::FileNotFoundException e) {
	std::string a = e.getFullDescription();
	std::cout << a;
	}
	*/
	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	light = scnMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);


	//GAMEPLAY SCENE SET UP
	//Create the players Entities(depends on the parameter passed by argument. This value
	//must correspond with the controllers connected).
	_nPlayers = nP;
	for (int i = 0; i < _nPlayers; i++){
		_players[i] = new Entity(std::to_string(i), this);
		addEntity(_players[i]);
	}

	//Not paused at start
	_paused = false;
	//Set the starter state to LOADOUT
	_currState = GS_SETUP;



	/*SCENE DEBUG


	test2->addComponent(new stringComponent(test2));
	//test2->addComponent()


	test1->addComponent(new messageSendComponent(test1));
	test1->addComponent(new meshRenderComponent(Ogre::Vector3(0, 0, 100), "Ra.mesh", test1, scnMgr));


	addEntity(test1);
	addEntity(test2);*/

}
GamePlayScene::~GamePlayScene(){
	delete light;
	delete cam;
	delete vp;
	delete scnMgr;

}
bool GamePlayScene::run(){
	//Here we would get the time between frames

	//Take messages from input
	InputManager::getInstance().getMessages(_messages);
	//Then we deliver the messages
	dispatch();

	//Proccess Scene Messages
	processScnMsgs();

	switch (_currState)
	{
		//In this state, we should set up the players God (mesh renderer, habilities, etc) and playing cards
	case GS_SETUP:
		loadOut();
		break;
		//This state should control the gameplay state (Time, rounds, the end, etc)
	case GS_BATTLE:
		battle();
		break;
		//Last state before leave the scene
	case GS_END:
		end();
	case GS_STOPPED:
		
		break;
	default:
		break;
	}

	//Logic simulation done here
	bool aux = updateEnts(0.025);

	//Clear dispatched messages
	clearMessageQueue();

	return aux;

}

void GamePlayScene::dispatch(){
	gameScene::dispatch();
}

void GamePlayScene::processScnMsgs(){

	nMessages = _sceneMessages.size();
	std::list<Message *>::iterator it = _sceneMessages.begin();
	for (int i = 0; i < nMessages && it != _sceneMessages.end(); i++, it++) {
		
		switch ((*it)->getType())
		{
		case CONTROLLER_STATE_MSG:

		default:
			break;
		}
		
		}

}

void GamePlayScene::loadOut(){
	














		


		bool playersAreReady = true;
		for(int i = 0; i < _nPlayers; i++){
			playersAreReady = playersAreReady & _pReady[i];
		}
		
		if (playersAreReady){
			_currState = GS_BATTLE;
		}
}

void GamePlayScene::battle(){
	/*
		Pick data from the battle State (_bS), and control the end of it
	*/

	//Pre-battle: little wait time to let players getting ready
	if (!_bS.battleStarted){



	//Contador de 5 seg (por ejemplo) y empieza el combate
	_bS.battleStarted = true;
	_bS.timeCountStart = SDL_GetTicks();
	}

	//Battle
	else{

		//Update time elapsed
		_bS.timeElapsed = SDL_GetTicks() - _bS.timeCountStart;

		//If time is greater than limit, stop battle
		if (_bS.timeElapsed > TIME_LIMIT){
			_bS.battleEnded = true;
			_currState = GS_END;
		}




	}



}
void GamePlayScene::end(){

}

void GamePlayScene::controllerDisconected(int id){

	_paused = true;


}
#pragma endregion