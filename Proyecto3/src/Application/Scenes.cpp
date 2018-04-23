#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreRenderWindow.h>
#include "DebugNew.h"
//Debug 
#ifdef _DEBUG
#include <iostream>
#define new DEBUG_NEW
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
#include "DebugDraw.h"

//DebugDraw dInstance;

#pragma region gameScene 

GameScene::GameScene(std::string id, Game * game) :_id(id), pGame(game), vp(0),scnMgr(0){
	scnMgr = pGame->getRoot()->createSceneManager(Ogre::ST_GENERIC);
	nMessages = -1;
}
GameScene::~GameScene()
{


	deleteAllMessages();

	

	list<Entity*>::iterator it = _entities.begin();
	while (!_entities.empty()){
		delete _entities.front();
		_entities.pop_front();
	}
	
	
	
	EntityFactory::getInstance().resetInstance();
	scnMgr->clearScene();
	scnMgr->destroyAllManualObjects();
	
	if (scnMgr != nullptr)
		pGame->getRoot()->destroySceneManager(scnMgr);
	
	
}

void GameScene::clearDebugDraw(){ /*dInstance.clear();*/ }
bool GameScene::updateEnts(float delta){
	for (auto ent : _entities){
		if(ent != NULL)ent->tick(delta);
	}
	return true;
} 

Ogre::SceneManager * GameScene::getSceneManager(){

	return scnMgr;
}

Game* GameScene::getGame(){
	return pGame;
}

void GameScene::getMessage(Message * m){
	_messages.push_back(m);
}
void GameScene::addEntity(Entity * ent){
	_entities.push_back(ent);
}
void GameScene::dispatch() {
	/*We only process as many messages as we had at the start of the update.
	*Any messsage introduced as a result of reading a message is processed
	in the next frame*/
	nMessages = _messages.size();
	std::list<Message *>::iterator it =_messages.begin();
	for (int i = 0; i < nMessages && it != _messages.end(); i++, it++) {
		//If the message destination is only the scene, we only push it to the local queue.
		if ((*it)->getDestination() == SCENE_ONLY)
		{
			_sceneMessages.push_back((*it));
		}
		//If the message have more receivers than the scene, push them to the entities.
		else
		{
			if ((*it)->getDestination() == SCENE)
				_sceneMessages.push_back((*it));
			for (Entity * aux : _entities) {
				if ((*it)->getEmmiter() != aux->getID())
					aux->getMessage(*it);
			}
		}
	}
}
//Message disposal to use just before finishing the current scene loop.
// It uses the variable nMessages, that is set in the dispatch function.
void GameScene::clearMessageQueue() {
	
	for (int i = 0; i < nMessages; i++) {
		Message * aux = _messages.front();
		delete aux;
		_messages.pop_front();
	}
}

void GameScene::deleteAllMessages(){

	for (int i = 0; i < _messages.size(); i++) {
		Message * aux = _messages.front();
		delete aux;
		_messages.pop_front();
	}

	for (int j = 0; j < _sceneMessages.size(); j++) {
		Message * aux = _sceneMessages.front();
		delete aux;
		_sceneMessages.pop_front();
	}
}
void GameScene::deleteEntity(std::string id){
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
BasicScene::BasicScene(std::string id, Game * game): GameScene(id, game) {
	
	

	//Debug draw
	/*dInstance.setSceneManager(scnMgr);
	pGame->getPhysicsWorld()->SetDebugDraw(&dInstance);
	dInstance.SetFlags(b2Draw::e_shapeBit /*| b2Draw::e_aabbBit);
	*/


	



	//------------------------------------------------------------------------------------------------------
	//ViewPort Addition
	
	
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
	
	Entity * cam = new Entity ("Camera1", this);
	cam->addComponent(
		new CCamera(cam, scnMgr, vp, "MainCamera", Ogre::Vector3(0, 0, 100), Ogre::Vector3(0,0,0), 5)
	);
	addEntity(cam);
	
	addEntity(EntityFactory::getInstance().createGod(EG_RA,this,Ogre::Vector3(0,10,0)));
	
	/*Entity * Suelo2 = new Entity("2", this);
	Suelo2->addComponent(new CRigidBody(Suelo2, game->getPhysicsWorld(), Ogre::Vector3(-3, 5, 0), 3, 4, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	addEntity(Suelo2);

	Entity * Suelo3 = new Entity("3", this);
	Suelo2->addComponent(new CRigidBody(Suelo3, game->getPhysicsWorld(), Ogre::Vector3(33, 5, 0), 3, 8, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	addEntity(Suelo3);

	Entity * Suelo4 = new Entity("4", this);
	Suelo2->addComponent(new CRigidBody(Suelo4, game->getPhysicsWorld(), Ogre::Vector3(33, -26, 0), 3, 8, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	addEntity(Suelo4);
	
	Entity * Suelo = new Entity("1", this);
	Suelo->addComponent(new CRigidBody(Suelo, game->getPhysicsWorld(), Ogre::Vector3(-100, -40, 0), 3,2000, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	//Suelo->addComponent(new CMeshRender(Ogre::Vector3(0, 0, 0), "Barrel.mesh", Suelo, scnMgr));
	addEntity(Suelo);*/

	

	//EntityFactory::getInstance().createEntity(,,)
	

	



	
}


BasicScene::~BasicScene(){
	//dInstance.~DebugDraw();
	//delete light;

}




bool BasicScene::run(){
	//Here we would get the time between frames

	//Take messages from input
	InputManager::getInstance().getMessages(_messages);
	//Then we deliver the messages
	GameScene::dispatch();

	processScnMsgs();

	//Logic simulation done here
	bool aux = updateEnts(0.025);

	//Clear dispatched messages
	clearMessageQueue();
	
	

	return aux;

}

void BasicScene::dispatch(){
	GameScene::dispatch();

}

void BasicScene::processScnMsgs()
{
	int nSceneMessages = _sceneMessages.size();
	for (std::list<Message *>::iterator it = _sceneMessages.begin(); it != _sceneMessages.end();){
		Message* m = (*it);
		if (m->getType() == MSG_ADD_ENTITY)
		{
			addEntity(static_cast<MAddEntity*>(m)->getEntity());
		}

		it++;
		_sceneMessages.pop_front();
	}
	
};

#pragma endregion

#pragma region GamePlayScene
//Scene that runs and manage the battle phase of the game.
GamePlayScene::GamePlayScene(std::string id, Game * game, int nP) : GameScene(id, game) {
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


	Ra->addComponent(new messageSendComponent(Ra));
	Ra->addComponent(new meshRenderComponent(Ogre::Vector3(0, 0, 100), "Ra.mesh", Ra, scnMgr));


	addEntity(Ra);
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
	GameScene::dispatch();
}

void GamePlayScene::processScnMsgs(){

	nMessages = _sceneMessages.size();
	std::list<Message *>::iterator it = _sceneMessages.begin();
	for (int i = 0; i < nMessages && it != _sceneMessages.end(); i++, it++) {
		
		switch ((*it)->getType())
		{
		case MSG_CONTROLLER_STATE:

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