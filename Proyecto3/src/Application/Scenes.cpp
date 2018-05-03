#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreRenderWindow.h>
#include "DebugNew.h"
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

	addEntity(EntityFactory::getInstance().createGod(EG_RA,this,Ogre::Vector3(-20,20,0)));
	addEntity(EntityFactory::getInstance().createGod(EG_AHPUCH, this, Ogre::Vector3(20, 20, 0)));
	
	/*Entity * Suelo2 = new Entity("2", this);
	Suelo2->addComponent(new CRigidBody(Suelo2, game->getPhysicsWorld(), Ogre::Vector3(-51, -15, 0), 10, 1, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));

	addEntity(Suelo2);

	Entity * Suelo3 = new Entity("3", this);
	Suelo2->addComponent(new CRigidBody(Suelo3, game->getPhysicsWorld(), Ogre::Vector3(33, 5, 0), 1, 8, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	addEntity(Suelo3);

	Entity * Suelo4 = new Entity("4", this);
	Suelo2->addComponent(new CRigidBody(Suelo4, game->getPhysicsWorld(), Ogre::Vector3(33, -26, 0), 1, 8, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	addEntity(Suelo4);*/

	Entity * e1 = new Entity("Suelo_01", this);
	//e1->addComponent(new CRigidBody(e1, game->getPhysicsWorld(), Ogre::Vector3(0, -20, 0), 3, 20, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	CMeshRender* cM = new CMeshRender({ 45, -20, -30 }, "suelo.mesh", e1, scnMgr, { 100.0f, 100.0f, 100.0f }, { 0, -90.0f, 0 });
	e1->addComponent(cM);
	addEntity(e1);
	
	Entity * Suelo = new Entity("ColliderSuelo_01", this);
	Suelo->addComponent(new CRigidBody(Suelo, game->getPhysicsWorld(), Ogre::Vector3(-100, -15, 0), 3,cM->getSize().x, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	//Suelo->addComponent(new CMeshRender(Ogre::Vector3(0, 0, 0), "Barrel.mesh", Suelo, scnMgr));

	addEntity(Suelo);
	
	
	
	Entity * porton = new Entity("Porton_01", this);
	//porton->addComponent(new CRigidBody(porton, game->getPhysicsWorld(), Ogre::Vector3(0, -10, 0), 3, 8, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	porton->addComponent(new CMeshRender({ 0, -10, -10 }, "porton.mesh", porton, scnMgr, { 1.0f, 1.0f, 1.0f }, { 0, 50, 180 }));
	addEntity(porton);

	Entity * rightEdge = new Entity("ColliderLimite_Dcho", this);
	rightEdge->addComponent(new CRigidBody(rightEdge, game->getPhysicsWorld(), Ogre::Vector3(55, -15, 0), 50, 2, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));	
	addEntity(rightEdge);

	Entity * temple = new Entity("Templo_01", this);
	//temple->addComponent(new CRigidBody(temple, game->getPhysicsWorld(), Ogre::Vector3(55, -15, 0), 50, 2, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	temple->addComponent(new CMeshRender({ -30, -2, -20 }, "templo.mesh", temple, scnMgr, { 10.0f, 10.0f, 10.0f }, { 0, 300, 0 }));
	addEntity(temple);

	Entity * portonCollider = new Entity("ColliderPorton_01", this);
	portonCollider->addComponent(new CRigidBody(portonCollider, game->getPhysicsWorld(), Ogre::Vector3(-2, 2, 0), 2, 15, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	//temple->addComponent(new CMeshRender({ -30, 0, -20 }, "templo.mesh", temple, scnMgr, { 10.0f, 10.0f, 10.0f }, { 0, 300, 0 }));
	addEntity(portonCollider);




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
GamePlayScene::GamePlayScene(std::string id, Game * game, int nP) : GameScene(id, game) {
	scnMgr = pGame->getRoot()->createSceneManager(Ogre::ST_GENERIC);

	//Debug draw
#ifdef _DEBUG
	dInstance.setSceneManager(scnMgr);
	pGame->getPhysicsWorld()->SetDebugDraw(&dInstance);
	dInstance.SetFlags(b2Draw::e_shapeBit /*| b2Draw::e_aabbBit*/);
#endif

	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	light = scnMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);


	//SCENE DEBUG

	Entity * cam = new Entity("Camera1", this);
	cam->addComponent(
		new CActionCamera(cam, scnMgr, vp)
		);
	addEntity(cam);

	Ogre::Vector3 v(1.0f, 1.0f, 1.0f);

	addEntity(EntityFactory::getInstance().createGod(EG_RA, this, Ogre::Vector3(-20, 20, 0)));
	addEntity(EntityFactory::getInstance().createGod(EG_AHPUCH, this, Ogre::Vector3(20, 20, 0)));

	Entity * e1 = new Entity("Suelo_01", this);
	//e1->addComponent(new CRigidBody(e1, game->getPhysicsWorld(), Ogre::Vector3(0, -20, 0), 3, 20, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	CMeshRender* cM = new CMeshRender({ 45, -20, -30 }, "suelo.mesh", e1, scnMgr, { 100.0f, 100.0f, 100.0f }, { 0, -90.0f, 0 });
	e1->addComponent(cM);
	addEntity(e1);

	Entity * Suelo = new Entity("ColliderSuelo_01", this);
	Suelo->addComponent(new CRigidBody(Suelo, game->getPhysicsWorld(), Ogre::Vector3(-100, -15, 0), 3, cM->getSize().x, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	//Suelo->addComponent(new CMeshRender(Ogre::Vector3(0, 0, 0), "Barrel.mesh", Suelo, scnMgr));

	addEntity(Suelo);



	Entity * porton = new Entity("Porton_01", this);
	//porton->addComponent(new CRigidBody(porton, game->getPhysicsWorld(), Ogre::Vector3(0, -10, 0), 3, 8, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	porton->addComponent(new CMeshRender({ 0, -10, -10 }, "porton.mesh", porton, scnMgr, { 1.0f, 1.0f, 1.0f }, { 0, 50, 180 }));
	addEntity(porton);

	Entity * rightEdge = new Entity("ColliderLimite_Dcho", this);
	rightEdge->addComponent(new CRigidBody(rightEdge, game->getPhysicsWorld(), Ogre::Vector3(55, -15, 0), 50, 2, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	addEntity(rightEdge);

	Entity * temple = new Entity("Templo_01", this);
	//temple->addComponent(new CRigidBody(temple, game->getPhysicsWorld(), Ogre::Vector3(55, -15, 0), 50, 2, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	temple->addComponent(new CMeshRender({ -30, -2, -20 }, "templo.mesh", temple, scnMgr, { 10.0f, 10.0f, 10.0f }, { 0, 300, 0 }));
	addEntity(temple);

	Entity * portonCollider = new Entity("ColliderPorton_01", this);
	portonCollider->addComponent(new CRigidBody(portonCollider, game->getPhysicsWorld(), Ogre::Vector3(-2, 2, 0), 2, 15, RB_STATIC, SH_POLYGON, MASK_STATIC_TERRAIN));
	//temple->addComponent(new CMeshRender({ -30, 0, -20 }, "templo.mesh", temple, scnMgr, { 10.0f, 10.0f, 10.0f }, { 0, 300, 0 }));
	addEntity(portonCollider);


	//GAMEPLAY SCENE SET UP
	/*Create the players Entities(depends on the parameter passed by argument. This value
	//must correspond with the controllers connected).
	_nPlayers = nP;
	for (int i = 0; i < _nPlayers; i++){
		_players[i] = new Entity(std::to_string(i), this);
		addEntity(_players[i]);
	}
	*/

	//Not paused at start
	_paused = false;

	//Set the starter state to LOADOUT
	_currState = GS_SETUP;
}
GamePlayScene::~GamePlayScene(){
	delete light;

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

	//Delete entities removed from the scene at the last frame
	destroyEntities();

	//Delete box2d bodies of the removed entities
	destroyBodies();

	
	return aux;
}

void GamePlayScene::dispatch(){
	GameScene::dispatch();
}

void GamePlayScene::processScnMsgs(){

	int nSceneMessages = _sceneMessages.size();
	for (std::list<Message *>::iterator it = _sceneMessages.begin(); it != _sceneMessages.end();){
		Message* m = (*it);
		if (m->getType() == MSG_ADD_ENTITY)
		{
			addEntity(static_cast<MAddEntity*>(m)->getEntity());
		}
		else if (m->getType() == MSG_CONTROLLER_STATE)
		{

		}

		it++;
		_sceneMessages.pop_front();
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