#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreRenderWindow.h>
#include "DebugNew.h"

//Debug 
#ifdef _DEBUG
#include <iostream>
#define new DEBUG_NEW
#endif


#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>
#include <OgreOverlaySystem.h>


//Later removable
#include <OgreCamera.h>
#include <OgreNode.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <Components.h>
#include <Box2D.h>

#include <Entity.h>
#include "InputManager.h"
#include "EntityFactory.h"
#include "Game.h"
#include "Messages.h"
#include "Scenes.h"
#include "DebugDraw.h"
//Debug 
#ifdef _DEBUG
#include <iostream>
#define new DEBUG_NEW
#endif

DebugDraw dInstance;

#pragma region GameScene 

GameScene::GameScene(std::string id, Game * game) :_id(id), pGame(game), vp(0),scnMgr(0){
	scnMgr = pGame->getRoot()->createSceneManager(Ogre::ST_GENERIC);
	scnMgr->addRenderQueueListener(Game::getInstance()->getOverlaySystem());
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
	
	destroyBodies();

	scnMgr->removeRenderQueueListener(Game::getInstance()->getOverlaySystem());


	/*if (scnMgr != nullptr)
		pGame->getRoot()->destroySceneManager(scnMgr);*/
	
}

void GameScene::clearDebugDraw(){ dInstance.clear(); }
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

	size_t nMessages = _messages.size();
	for (size_t i = 0; i <nMessages; i++) {
		Message * aux = _messages.front();
		delete aux;
		_messages.pop_front();
	}

	size_t nSceneMessages = _sceneMessages.size();
	for (int j = 0; j < nSceneMessages; j++) {
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

void GameScene::addBodyToDelete(b2Body* b){
	_physicBodies.emplace_back(b);
}

void GameScene::destroyBodies(){

	for (size_t i = 0; i < _physicBodies.size(); i++){
		pGame->getPhysicsWorld()->DestroyBody(_physicBodies[i]);
	}

	_physicBodies.clear();

}

void GameScene::addEntityToDelete(Entity * e){
	_entitiesToDelete.emplace_back(e);
}

void GameScene::destroyEntities(){

	for (size_t i = 0; i < _entitiesToDelete.size(); i++){
		deleteEntity(_entitiesToDelete[i]->getID());
	}
	_entitiesToDelete.clear();
}
#pragma endregion

#pragma region BasicScene
/*BASIC SCENE TO TEST SCENE IMPLEMENTATION.
//BUILDS A OGREHEAD BUT DOES NOT INCLUDE IT IN THE 
//SCENE ENTITY LIST. IT IS ONLY IN OGRE ENTITIES LIST.
//ALSO CREATS A BASIC ENTITY WITH A stringComponent attached*/
BasicScene::BasicScene(std::string id, Game * game): GameScene(id, game) {
	i = 0;
	
	//Debug draw
	dInstance.setSceneManager(scnMgr);
	pGame->getPhysicsWorld()->SetDebugDraw(&dInstance);
	dInstance.SetFlags(b2Draw::e_shapeBit /*| b2Draw::e_aabbBit*/);

	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	light = scnMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);
	

	//SCENE DEBUG
	
	Entity * cam = new Entity ("Camera1", this);
	cam->addComponent(
		new CActionCamera(cam, scnMgr, vp)
	);
	addEntity(cam);

	Ogre::Vector3 v(1.0f, 1.0f, 1.0f);

}


BasicScene::~BasicScene(){
	
	

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

	//Delete entities removed from the scene at the last frame
	destroyEntities();

	//Delete box2d bodies of the removed entities
	destroyBodies();
	

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
GamePlayScene::GamePlayScene(std::string id, Game * game, std::vector<Player> players, E_STAGE stage) : GameScene(id, game), _stage(stage) {

	//Debug draw
#ifdef _DEBUG
	dInstance.setSceneManager(scnMgr);
	pGame->getPhysicsWorld()->SetDebugDraw(&dInstance);
	dInstance.SetFlags(b2Draw::e_shapeBit /*| b2Draw::e_aabbBit*/);
#endif

	//The limit time to choose cards
	_prepareLimitTime = 30000.0f; //30 seconds
	_prepareCounter = 0.0f;

	//Load the stage passed by parameter
	loadStage();

	//Store and configure the players structure
	_players = players;
	std::vector<Ogre::Vector3> playersPos(2);
	playersPos[0] = Ogre::Vector3(20.0f, 0.0f, 0.0f);
	playersPos[1] = Ogre::Vector3(-20.0f, 0.0f, 0.0f);
	
	int i = 0;
	for (Player p : _players){
		p.entity = EntityFactory::getInstance().createGod(p.god, this, playersPos[i],p.controllerId);
		addEntity(p.entity);
			i++;
	}

	//Not paused at start
	_paused = false;

	//Set the starter state to SETUP
	_currState = GS_SETUP;
	_prepareCounter = SDL_GetTicks();



	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	// Create an overlay
	overlay = overlayManager.getByName("KEK");
	// Show the overlay
	Ogre::FontManager::getSingleton().getByName("Caption")->load();

	// Create a panel

	overlay->show();

}
GamePlayScene::~GamePlayScene(){
	

}
bool GamePlayScene::run(){
	

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
		//loadOut();
		break;
		//This state should control the gameplay state (Time, rounds, the end, etc)
	case GS_BATTLE:
		battlePhase();
		break;
		//Last state before leave the scene
	case GS_END:
		endPhase();
	case GS_STOPPED:
		
		break;
	default:
		break;
	}

	//Logic simulation done here
	bool aux = updateEnts(0.025);

	//Clear dispatched messages
	clearMessageQueue();

	//Delete entities removed from the scene at the last frame
	destroyEntities();

	//Delete box2d bodies of the removed entities
	destroyBodies();

	overlay->show();
	return aux;
}

void GamePlayScene::dispatch(){
	GameScene::dispatch();
}

void GamePlayScene::processScnMsgs(){

	int nSceneMessages = _sceneMessages.size();
	for (std::list<Message *>::iterator it = _sceneMessages.begin(); it != _sceneMessages.end();){
		Message* m = (*it);
		switch (m->getType())
		{
		case MSG_ADD_ENTITY:
			addEntity(static_cast<MAddEntity*>(m)->getEntity());
			break;
		case MSG_CONTROLLER_STATE:
			break;
		case MSG_DIE:
			if (m->getEmmiter().compare(0, 6, std::string("Player")) == 0){
				playerDied(m->getEmmiter());
			}
			break;
		default:
			break;
		}

		it++;
		_sceneMessages.pop_front();
	}

}

void GamePlayScene::preparePhase(){
	

	float currentTime = SDL_GetTicks();
	if (currentTime - _prepareCounter > _prepareLimitTime)
		_currState = GS_BATTLE;

		bool playersAreReady = true;
		for(int i = 0; i < _nPlayers; i++){
			playersAreReady = playersAreReady & _pReady[i];
		}
		
		if (playersAreReady){
			_currState = GS_BATTLE;
		}
}

void GamePlayScene::battlePhase(){
	/*
		Pick data from the battle State (_battleState), and control the end of it
	*/

	//Pre-battle: little wait time to let players getting ready
	if (!_battleState.battleStarted){



	//Contador de 5 seg (por ejemplo) y empieza el combate
	_battleState.battleStarted = true;
	_battleState.timeCountStart = SDL_GetTicks();
	}

	//Battle
	else{

		//Update time elapsed
		_battleState.timeElapsed = SDL_GetTicks() - _battleState.timeCountStart;

		//If time is greater than limit, stop battle
		if (_battleState.timeElapsed > TIME_LIMIT){
			_battleState.battleEnded = true;
			_currState = GS_END;
		}




	}



}

void GamePlayScene::endPhase(){

}

void GamePlayScene::controllerDisconected(int id){

	_paused = true;


}

void GamePlayScene::controllerConnected(int id){
	_paused = false;
}

void GamePlayScene::loadStage(){

	//Store the entities in an aux array
	std::vector<Entity*> stageEntities = EntityFactory::getInstance().createStage(_stage, this);

	//Then push them to the main array of entities
	for (Entity* e : stageEntities){
		addEntity(e);
	}
	
}

void GamePlayScene::changePhase(GameplayState newState){

	switch (newState){
	case GS_SETUP:
			break;
	case GS_BATTLE:
		break;
	case GS_END:
		break;
	}

}

void GamePlayScene::playerDied(std::string e){

	//Find which player was killed and wich won
	int playerDeadId = std::stoi( e.substr(7, 8));
	int playerWonId;
	if (playerDeadId == 0){
		playerWonId = 1;
	}
	else
		playerWonId = 0;

	//Increase runds completed
	_players[playerWonId].roundsWon++;
	_battleState.roundsCompleted++;

	//If the rounds elapsed are 3, we finish the game. Else, continue.
	if (_battleState.roundsCompleted == MAX_ROUNDS){
		changePhase(GS_END);
	}
	else
		changePhase(GS_SETUP);
	

}
#pragma endregion