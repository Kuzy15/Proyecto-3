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


#include <stdlib.h>
#include <time.h>
#include <random>

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
#include "Components.h"
#include "RenderComponents.h"
#include "Camera.h"

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
	deleteAllEntities();

	Ogre::OverlayManager::getSingleton().destroy(overlay->getName());



	deleteAllMessages();

	scnMgr->clearScene();
	scnMgr->destroyAllManualObjects();

	Game::getInstance()->getRenderWindow()->removeAllViewports();

	destroyBodies();

	scnMgr->removeRenderQueueListener(Game::getInstance()->getOverlaySystem());


	/*if (scnMgr != nullptr)
		Game::getInstance()->getRoot()->destroySceneManager(scnMgr);*/

}

void GameScene::clearDebugDraw(){dInstance.clear(); }
bool GameScene::updateEnts(float delta){
	for (auto ent : _entities){
		if(ent.second != NULL)ent.second->tick(delta);
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

	_entities.emplace(std::pair<std::string,Entity*>(ent->getID(),ent));
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
			for (std::map<std::string, Entity*>::iterator itEnt = _entities.begin(); itEnt != _entities.end(); ++itEnt) {
				if ((*it)->getEmmiter() != (*itEnt).second->getID())
					(*itEnt).second->getMessage(*it);
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

void GameScene::deleteAllEntities(){
	Entity* aux;
	for(std::pair<std::string, Entity*> e : _entities){
		aux = e.second;
		delete aux;
	}
	_entities.clear();


}

void GameScene::deleteEntity(std::string id){


	std::map<std::string, Entity*>::iterator it;
	Entity* aux;
	it = _entities.find(id);
	if (it != _entities.end()){
		aux = (*it).second;
		size_t error = _entities.erase(id);
		if (error != 1){
			//error
		}
		delete aux;

	}
}

void GameScene::addBodyToDelete(b2Body* b){
	_physicBodies.emplace_back(b);
}

void GameScene::destroyBodies(){

	for (size_t i = 0; i < _physicBodies.size(); i++){
		Game::getInstance()->getPhysicsWorld()->DestroyBody(_physicBodies[i]);
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

std::string GameScene::godToString(E_GOD g){
	switch (g){
	case EG_RA:
		return "RA";
		break;
	case EG_AHPUCH:
		return "AH_PUCH";
		break;
	case EG_ZEUS:
		return "ZEUS";
		break;
	case EG_HACHIMAN:
		return "HACHIMAN";
		break;
	default:
		break;
	}
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
	Game::getInstance()->getPhysicsWorld()->SetDebugDraw(&dInstance);
	dInstance.SetFlags(b2Draw::e_shapeBit /*| b2Draw::e_aabbBit*/);

	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	light = scnMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);


	//SCENE DEBUG

	Entity * cam = new Entity ("Camera1", this);
	cam->addComponent(
		new CActionCamera(cam, scnMgr, vp,100,100, 40,100)
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

void exitCallBack() {
	Game::getInstance()->exitGame();
}
void battleCallBack() {

}
void selectGodCallBack(){

	Game::getInstance()->changeScene(MULTIPLAYER);

}
void returnMainMenu(){

	Game::getInstance()->changeScene(MAIN_MENU);

}

void returnMulitplayerMenu(){

	Game::getInstance()->changeScene(MULTIPLAYER);
}


#pragma region GamePlayScene

/*------------ Callback functions for card select buttons --------------*/



//Scene that runs and manage the battle phase of the game.
GamePlayScene::GamePlayScene(std::string id, Game * game, std::vector<Player> players, E_STAGE stage) : GameScene(id, game), _stage(stage) {

	//Debug draw
#ifdef _DEBUG
	dInstance.setSceneManager(scnMgr);
	Game::getInstance()->getPhysicsWorld()->SetDebugDraw(&dInstance);
	dInstance.SetFlags(b2Draw::e_shapeBit /*| b2Draw::e_aabbBit*/);
#endif

	//The limit time to choose cards
	_prepareLimitTime = 50000.0f; //15 seconds
	_prepareCounter = 0.0f;

	_postGameLimitTime = 5000.0f;
	_postGameCounter = 0.0f;

	_preGameLimitTime = 5000.0f;
	_preGameCounter = 0.0f;

	//Load the stage passed by parameter
	loadStage();



	//Store and configure the players structure
	_players = players;

	std::vector<Ogre::Vector3> playerPos;
	playerPos.push_back(_posP1);
	playerPos.push_back(_posP2);

	int i = 0;
#pragma region Abilities
	_totalPassives.push_back(CMP_PASSIVE_VIDAR);
	_totalPassives.push_back(CMP_PASSIVE_HADES);
	_totalPassives.push_back(CMP_PASSIVE_ULL);
	_totalPassives.push_back(CMP_PASSIVE_VALI);
	_totalPassives.push_back(CMP_PASSIVE_HERMES);
	_totalPassives.push_back(CMP_PASSIVE_SYN);

	_totalActives.push_back(CMP_SHU_HEADDRESS);
	_totalActives.push_back(CMP_JONSU_MOON);
	_totalActives.push_back(CMP_KHEPRI_BEETLE);
	_totalActives.push_back(CMP_HERA_RUNE);
	_totalActives.push_back(CMP_HERIS_MARK);
#pragma endregion


	std::srand(time(NULL));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>dis(0, 5);
	std::uniform_int_distribution<>dis2(0, 4);

	int rng = 0;
	int rng2 = 0;
	for (int i = 0;  i < _players.size(); i++){

		_players[i].entity = EntityFactory::getInstance().createGod(_players[i].god, this, playerPos[i], _players[i].controllerId);

		rng = dis(gen);
		_players[i].abilities.push_back(_totalPassives[rng]);

		rng2 = dis(gen);
		if (rng2 == rng)
			rng2 = (rng2 + 1) % 6;
		_players[i].abilities.push_back(_totalPassives[rng2]);

		rng = dis2(gen);
		_players[i].abilities.push_back(_totalActives[rng]);

		_players[i].currentActive = CMP_ACTIVE_DEFAULT;
		_players[i].currentPassive = CMP_PASSIVE_DEFAULT;
		addEntity(_players[i].entity);
		_players[i].entity->setActive(false);
		


	}
	dis.reset();
	dis2.reset();




	std::srand(time(NULL));
	
	//Not paused at start
	_paused = false;


	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();


	// Create an overlay
	try {
		overlay = overlayManager.getByName("GUI");
		bgCards = overlayManager.getByName("bgCards");
		bgEnd = overlayManager.getByName("bgEnd");
	}
	catch (Ogre::Exception e) {
		#ifdef _DEBUG
		cout << e.what() << std::endl;
		cout << std::endl;
		#endif
	}
	// Show the overlay
	Ogre::FontManager::getSingleton().getByName("GUI/TimeText")->load();
	Ogre::FontManager::getSingleton().getByName("GUI/PlayerText")->load();

	overlay->hide();
	bgEnd->hide();

	Entity * k = new Entity("GUI", this);


	//k->addComponent(new CNormalButton(overlay, k, 0, Ogre::Vector2(-100, 150), Ogre::Vector2(0, 0), K, "Placeholder"));
	k->addComponent(new CGUITimer(k, overlay));
	k->addComponent(new CPlayerGUI(k, overlay, P1, players[0].god));
	k->addComponent(new CPlayerGUI(k, overlay, P2, players[1].god));
	addEntity(k);


	//Add end GUI entities
	Entity* endButton0 = new Entity("EndButton_0", this);
	endButton0->addComponent(new CNormalButton(bgEnd, endButton0, 0, Ogre::Vector2(-100, 50.0f), Ogre::Vector2(), exitCallBack, "Exit"));
	endButton0->setActive(false);
	addEntity(endButton0);
	_endGUIEntities.push_back(endButton0);


	changePhase(GS_SETUP);

	irrklang::ISound* bso = game->getSoundEngine()->play2D("../Media/sounds/BattleTheme.wav",true,false,true);
	if (bso){
		bso->setVolume(0.1f);
	}

}
GamePlayScene::~GamePlayScene(){
	_players.clear();
	_cardGUIEntities.clear();

	Entity* aux;

	for (Entity* e : _stageEntities){
		aux = e;
		if (aux != nullptr)
			delete aux;
	}

	_stageEntities.clear();

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
		preparePhase();
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

	return aux;

}

void GamePlayScene::dispatch(){

	GameScene::dispatch();

}

void GamePlayScene::processScnMsgs(){


	int nSceneMessages = _sceneMessages.size();
	for (std::list<Message *>::iterator it = _sceneMessages.begin(); it != _sceneMessages.end();){
		Message* m = (*it);
		switch (_currState){
		case GS_SETUP:
			processMsgSetup(m);
			break;
		case GS_BATTLE:
			processMsgBattle(m);
			break;
		case GS_END:
			processMsgEnd(m);
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
		changePhase(GS_BATTLE);

	bool playersAreReady = true;
	for(int i = 0; i < _nPlayers; i++){
		playersAreReady = playersAreReady & _pReady[i];
	}

	if (playersAreReady){
		changePhase(GS_BATTLE);
	}
}

void GamePlayScene::battlePhase(){
	/*
		Pick data from the battle State (_battleState), and control the end of it
	*/

	//Pre-battle: little wait time to let players getting ready
	if (!_battleState.battleStarted){
	//Contador de 5 seg (por ejemplo) y empieza el combate
		if ((SDL_GetTicks() - _preGameCounter) > _preGameLimitTime){
			_battleState.battleStarted = true;
			_battleState.timeCountStart = SDL_GetTicks();
			InputManager::getInstance().stopReceiving(true);

		}
	}

	//Battle
	else{
		if (!_battleState.battleEnded){
			//Update time elapsed
			_battleState.timeElapsed = SDL_GetTicks() - _battleState.timeCountStart;
			getMessage(new MUpdateSceneTimer(_id, (TIME_LIMIT - _battleState.timeElapsed) / 1000.0f));


			//If time is greater than limit, stop battle
			if (_battleState.timeElapsed > TIME_LIMIT){

				_battleState.battleEnded = true;
				InputManager::getInstance().stopReceiving(false);
				_postGameCounter = SDL_GetTicks();



			}
		}
		else{

			//If the rounds elapsed are 3, we finish the game. Else, continue.
			if ((SDL_GetTicks() - _postGameCounter)  > _postGameLimitTime){
				if (_battleState.roundsCompleted == MAX_ROUNDS || _players[0].roundsWon >= 2 || _players[1].roundsWon >= 2){
					changePhase(GS_END);
					InputManager::getInstance().stopReceiving(true);
				}
				else{
					resetPlayers();
					InputManager::getInstance().stopReceiving(true);
					changePhase(GS_SETUP);
				}

				_battleState.battleStarted = false;
				_battleState.battleEnded = false;
				for (Player p : _players){
					p.entity->setActive(false);
				}

			}



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
	std::vector<Entity*>* stageEntities = EntityFactory::getInstance().createStage(_stage, this, _stageEntities);

	//Then push the cam to the main arrya entities. The rest goes into _stageEntities array.
	for (auto e : (*stageEntities)){
		addEntity(e);
	}
	stageEntities->clear();
	delete stageEntities;
}

void GamePlayScene::changePhase(GameplayState newState){

	switch (newState){
	case GS_SETUP:
		overlay->hide();
		//Set the starter state to SETUP
		_prepareCounter = SDL_GetTicks();
		player1Index = 0;
		player2Index = 3;
		_nPlayers = MAX_PLAYERS;
		
		loadAbilities();
		bgCards->show();
		getMessage(new MButtonAct(_id, player1Index));
		_currState = GS_SETUP;
			break;
	case GS_BATTLE:
		for (Entity* e : _cardGUIEntities){
			addEntityToDelete(e);
		}
		_cardGUIEntities.clear();
		bgCards->hide();
		overlay->show();
		_preGameCounter = SDL_GetTicks();
		getMessage(new MUpdateSceneTimer(_id, (TIME_LIMIT) / 1000.0f));

		_currState = GS_BATTLE;
		InputManager::getInstance().stopReceiving(false);
		for (Player p : _players){
			p.entity->setActive(true);
		}


		break;
	case GS_END:
		overlay->hide();
		bgCards->hide();
		bgEnd->show();
		for (Entity* e : _endGUIEntities){
			e->setActive(true);
		}
		player1Index = 0;
		getMessage(new MButtonAct(_id, player1Index));
		_currState = GS_END;
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
	else{
		playerWonId = 0;
	}
	//Increase runds completed
	_players[playerWonId].roundsWon++;
	getMessage(new MRoundFinished(_id,_players[playerWonId].entity->getID()));
	_battleState.roundsCompleted++;

	_battleState.battleEnded = true;
	InputManager::getInstance().stopReceiving(false);
	_postGameCounter = SDL_GetTicks();

	for (int i = 0; i < _cardGUIEntities.size(); i++){
		deleteEntity(_cardGUIEntities[i]->getID());	
	}
	_cardGUIEntities.clear();




}

void GamePlayScene::resetPlayers(){


	std::vector<Ogre::Vector3> playerPos;
	playerPos.push_back(_posP1);
	playerPos.push_back(_posP2);

	int i = 0;

	for (int i = 0; i < _players.size(); i++){
		deleteEntity(_players[i].entity->getID());
		_players[i].entity = nullptr;
		_players[i].activeSelected = false;
		_players[i].passiveSelected = false;
		_players[i].currentActive = CMP_ACTIVE_DEFAULT;
		_players[i].currentPassive = CMP_PASSIVE_DEFAULT;
		_players[i].entity = EntityFactory::getInstance().createGod(_players[i].god,this,playerPos[i],_players[i].controllerId);
		_players[i].entity->setActive(false);
		addEntity(_players[i].entity);
		_pReady[i] = false;

	}


	getMessage(new MLifeState(_players[0].entity->getID(), 100));
	getMessage(new MLifeState(_players[1].entity->getID(), 100));
	//getMessage(new MResetGui(_id));
	reloadAbilities();
}



void start_c(){



}
void GamePlayScene::reloadAbilities(){

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>dis(0, 5);
	std::uniform_int_distribution<>dis2(0, 4);

	_players[0].abilities.clear();
	_players[1].abilities.clear();

	int rng = 0;
	int rng2 = 0;
	for (int i = 0; i < _players.size(); i++){

		rng = dis(gen);
		_players[i].abilities.push_back(_totalPassives[rng]);

		rng2 = dis(gen);
		if (rng2 == rng)
			rng2 = (rng2 + 1) % 6;
		_players[i].abilities.push_back(_totalPassives[rng2]);

		rng = dis2(gen);
		_players[i].abilities.push_back(_totalActives[rng]);

		_players[i].currentActive = CMP_ACTIVE_DEFAULT;
		_players[i].currentPassive = CMP_PASSIVE_DEFAULT;


	}
	std::srand(rng);
	dis.reset();
	dis2.reset();


}

void GamePlayScene::loadAbilities(){

	Entity* aux;
	Ogre::Vector2 auxPos(0.05f,0.06f);
	int idCounter = 0;
	for (Player p : _players){
		//There, we should choose 3 random abilities to show
		for (ComponentType c : p.abilities){
			aux = new Entity((to_string(p.controllerId) + to_string(c) + to_string(_battleState.roundsCompleted)), this);
			aux->addComponent(new CAbilityButton(bgCards, aux, idCounter, auxPos, Ogre::Vector2(0, 0), p.controllerId, c));
			auxPos.x += 0.18f;
			_cardGUIEntities.emplace_back(aux);
			addEntity(aux);
			idCounter++;
		}
		auxPos.y += 0.43f;
		auxPos.x -= 0.18f;
	}


	for (int i = 3; i < 6; i++){
		_cardGUIEntities.at(i)->setActive(false);
	}


	auxPos = Ogre::Vector2(0.0f, 700.0f);

	/*aux = new Entity("Start_Button", this);
	aux->addComponent(new CNormalButton(bgCards, aux, idCounter, auxPos, Ogre::Vector2(0, 0),start_c,"Start"));*/

}

void GamePlayScene::addAbilityComponent(int playerId, ComponentType compId){

	GameComponent* c = EntityFactory::getInstance().createAbility(compId, _players[playerId].entity, playerId);
	#ifdef _DEBUG
		if (c == nullptr)
			std::cout << "No existe esa habilidad" << std::endl;
	#endif
	int type = static_cast<CAbility*>(c)->getType();

	//if is active or passive
	if (type == 0){
		if (_players[playerId].currentActive != compId){
			_players[playerId].entity->deleteComponent(_players[playerId].currentActive);
			_players[playerId].entity->addComponent(c);
		}
		else { delete c; }
		_players[playerId].activeSelected = true;
	}
	else{
		if (_players[playerId].currentPassive != compId /*cambiar por default*/){
			_players[playerId].entity->deleteComponent(_players[playerId].currentPassive);
			_players[playerId].entity->addComponent(c);
		}
		else { delete c; }
		_players[playerId].passiveSelected = true;
	}

	if (_players[playerId].activeSelected && _players[playerId].passiveSelected){
		_pReady[playerId] = true;
		if (_pReady[0] && !_pReady[1]){
			for (int i = 3; i < 6; i++){
				_cardGUIEntities.at(i)->setActive(true);
			}
			for (int i = 0; i < 3; i++){
				_cardGUIEntities.at(i)->setActive(false);
			}
		}
	}
}

void GamePlayScene::processMsgSetup(Message* m){

	MAbilitySet* mAbility;
	MInputState* mInput;

	switch (m->getType()){
	case MSG_INPUT_STATE:
		mInput = static_cast<MInputState*>(m);
		if (mInput->getId() == 0 && !_pReady[0]){
			if (mInput->getCInputState().Button_A == BTT_RELEASED){
				getMessage(new MButtonClick(_id, player1Index));

			}
			else if (mInput->getCInputState().DPad_Right == BTT_RELEASED){
				player1Index++;
				if (player1Index > 2) player1Index = 0;
				getMessage(new MButtonAct(_id, player1Index));
			}
			else if (mInput->getCInputState().DPad_Left == BTT_RELEASED){
				player1Index--;
				if (player1Index < 0) player1Index = 2;
				getMessage(new MButtonAct(_id, player1Index));
			}
		}
		else if (mInput->getId() == 1 && _pReady[0]){
			if (mInput->getCInputState().Button_A == BTT_RELEASED)
				getMessage(new MButtonClick(_id, player2Index));
			else if (mInput->getCInputState().DPad_Right == BTT_RELEASED){
				player2Index++;
				if (player2Index > 5) player2Index = 3;
				getMessage(new MButtonAct(_id, player2Index));
			}
			else if (mInput->getCInputState().DPad_Left == BTT_RELEASED){
				player2Index--;
				if (player2Index < 3) player2Index = 5;
				getMessage(new MButtonAct(_id, player2Index));
			}
		}
		break;
	case MSG_ABILITY_SETTER:
		mAbility = static_cast<MAbilitySet*>(m);
		addAbilityComponent(mAbility->getId(), mAbility->getComponentType());
		break;
	default:
		break;
	}


}

void GamePlayScene::processMsgBattle(Message* m){



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

}

void GamePlayScene::processMsgEnd(Message* m){



	MAbilitySet* mAbility;
	MInputState* mInput;

	switch (m->getType()){
	case MSG_INPUT_STATE:
		mInput = static_cast<MInputState*>(m);
		if (mInput->getId() == 0){
			if (mInput->getCInputState().Button_A == BTT_RELEASED){
				getMessage(new MButtonClick(_id, player1Index));
			}			
		}
		break;
	default:
		break;
	}



}
#pragma endregion

#pragma region Main Menu Scene
//The main menu class that contains the buttons to access to the diferents menus(Fight, Options, etc)
MainMenuScene::MainMenuScene(std::string id, Game * game) : GameScene(id, game) {

	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	selectedButton = 0;

	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
	irrklang::ISound* bso = game->getSoundEngine()->play2D("../Media/sounds/MenuTheme.wav", true);
	if (bso){
		bso->setVolume(0.2f);
	}

	// Create an overlay
	try {
		
		overlay = overlayManager.getByName("MENU");

	}
	catch (Ogre::Exception e) {
		cout << e.what() << std::endl;
		cout << std::endl;
	}
	// Show the overlay
	//Ogre::FontManager::getSingleton().getByName("TimeText")->load();

	Ogre::Viewport* vp = nullptr;
	Entity *cam = new Entity("camMenuIni",this);
	CCamera* cCam = new CCamera(cam, scnMgr, vp, cam->getID(), Ogre::Vector3(-50, 0, 0), Ogre::Vector3(1, 0, 0), 10);
	cam->addComponent(cCam);
	addEntity(cam);
	vp = cCam->getVP();

	Entity * background = new Entity("Bg", this);
	background->addComponent(new CSkyPlaneRender(background, scnMgr, 1, "MainMenu2", {70,0,0}, vp));
	//_menuEntities.emplace(std::pair<int, Entity*>(2, background));
	addEntity(background);


	Entity * fightButton = new Entity("fightButton", this);
	fightButton->addComponent(new CNormalButton(overlay, fightButton, 0, Ogre::Vector2(-450, 50), Ogre::Vector2(0, 0), selectGodCallBack, "Battle"));

	_menuEntities.emplace(std::pair<int, Entity*>(0, fightButton));
	addEntity(fightButton);

	Entity * exitButton = new Entity("exitButton", this);
	exitButton->addComponent(new CNormalButton(overlay, exitButton, 1, Ogre::Vector2(-450, 200), Ogre::Vector2(0, 0), exitCallBack, "Exit"));

	_menuEntities.emplace(std::pair<int, Entity*>(1, exitButton));
	addEntity(exitButton);

	overlay->show();

	//Set the first button selected
	getMessage(new MButtonAct(_id, selectedButton));

}


MainMenuScene::~MainMenuScene(){


}




bool MainMenuScene::run(){
	//Here we would get the time between frames

	//Take messages from input
	InputManager::getInstance().getMessages(_messages);
	//Then we deliver the messages
	dispatch();

	processScnMsgs();

	//Logic simulation done here
	bool aux = updateEnts(0.025);

	//Clear dispatched messages
	clearMessageQueue();


	return aux;

}

void MainMenuScene::dispatch(){
	GameScene::dispatch();


}

void MainMenuScene::processScnMsgs()
{
	MInputState* input;
	ControllerInputState c;

	int totalButtons = _menuEntities.size();

	int nSceneMessages = _sceneMessages.size();
	for (std::list<Message *>::iterator it = _sceneMessages.begin(); it != _sceneMessages.end();){
		Message* m = (*it);
		switch (m->getType())
		{
		case MSG_INPUT_STATE:
			input = static_cast<MInputState*>(*it);
			c = input->getCInputState();
			if (input->getId() == 0){
				if (c.DPad_Down == BTT_RELEASED){
					selectedButton++;
					if (selectedButton >= totalButtons)
						selectedButton = 0;
					getMessage(new MButtonAct(_id, selectedButton));
				}
				else if (c.DPad_Up == BTT_RELEASED){
					selectedButton--;
					if (selectedButton < 0)
						selectedButton = (totalButtons - 1);
					getMessage(new MButtonAct(_id, selectedButton));
				}

				else if (c.Button_A == BTT_RELEASED){
					getMessage(new MButtonClick(_id, selectedButton));
				}
			}
			break;
		default:
			break;
		}

		it++;
		_sceneMessages.pop_front();
	}



};




#pragma endregion


#pragma region Select God Scene
//The main menu class that contains the buttons to access to the diferents menus(Fight, Options, etc)
SelectGodScene::SelectGodScene(std::string id, Game * game) : GameScene(id, game) {

	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	light = getSceneManager()->createLight("SelectGodLight");
	light->setPosition(0, 30, 50);
	light->setCastShadows(true);

	
	std::vector<Player>* players = new std::vector<Player>(2);
	players->at(0).controllerId = 0;
	players->at(1).controllerId = 1;
	_players = (*players);
	delete players;

	selectedButton = 0;
	player1Index = player2Index = 0;


	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
	// Create an overlay
	try {
		overlay = overlayManager.getByName("SELECT");

	}
	catch (Ogre::Exception e) {
		cout << e.what() << std::endl;
		cout << std::endl;
	}
	// Show the overlay
	//Ogre::FontManager::getSingleton().getByName("TimeText")->load();

	Ogre::Viewport* vp = nullptr;
	cam = new Entity("camSelectGod", this);
	CCamera* cCam = new CCamera(cam, scnMgr, vp, cam->getID(), Ogre::Vector3(0, 0, 60), Ogre::Vector3(0, 0, 0), 10);
	vp = cCam->getVP();
	cam->addComponent(cCam);

	/*Entity * background = new Entity("backgroundMainMenu", this);
	background->addComponent(new CSkyPlaneRender(background, scnMgr, 1, 0, "MainMenu", {0,0,0}));
	_menuEntities.emplace(std::pair<int, Entity*>(0, background));*/


	Entity *sky = new Entity("sky", this);
	sky->addComponent(new CSkyPlaneRender(sky, sky->getScene()->getSceneManager(), 1, "SelectGodMenu", Ogre::Vector3{ 0, 0, -5 }, vp));
	addEntity(sky);

	Entity* renderAhPuch = new Entity("0", this);
	CMeshRender * AhPuchMeshRender = new CMeshRender({ 0, -5, 40 }, "AhPuch.mesh", renderAhPuch, renderAhPuch->getScene()->getSceneManager(), { 1.6f, 1.6f, 1.6f }, { 0, 0, 0 });
	AhPuchMeshRender->setVisible(true);
	renderAhPuch->addComponent(AhPuchMeshRender);
	addEntity(renderAhPuch);

	Entity* renderAux2 = new Entity("1", this);
	CMeshRender* RaMeshRender = new CMeshRender({ 0, -5, 40 }, "Ra.mesh", renderAux2, renderAux2->getScene()->getSceneManager(), { 1.6f, 1.6f, 1.6f }, { 0, 0, 0 });
	RaMeshRender->setVisible(false);
	renderAux2->addComponent(RaMeshRender);
	addEntity(renderAux2);

	Entity* renderAux3 = new Entity("2", this);
	CMeshRender* ZeusMeshRender = new CMeshRender({ 0, -5, 40 }, "Zeus.mesh", renderAux3, renderAux3->getScene()->getSceneManager(), { 1.6f, 1.6f, 1.6f }, { 0, 0, 0 });
	ZeusMeshRender->setVisible(false);
	renderAux3->addComponent(ZeusMeshRender);
	addEntity(renderAux3);

	Entity* renderAux4 = new Entity("3", this);
	CMeshRender* HachimanMeshRender = new CMeshRender({ 0, -5, 40 }, "Hachiman.mesh", renderAux4, renderAux4->getScene()->getSceneManager(), { 1.6f, 1.6f, 1.6f }, { 0, 0, 0 });
	HachimanMeshRender->setVisible(false);
	renderAux4->addComponent(HachimanMeshRender);
	addEntity(renderAux4);


#pragma region Player1 Gods


	Entity * aux = new Entity("AhPuchP0", this);
	aux->addComponent(new CGodButton(overlay, aux, 0, Ogre::Vector2(0.5f - (0.1041f * 2.0f), 0.75f), Ogre::Vector2(0, 0), _players[0].controllerId, EG_AHPUCH));
	_godsButtons.push_back(aux);



	Entity * aux2 = new Entity("RaP0", this);
	aux->addComponent(new CGodButton(overlay, aux2, 1, Ogre::Vector2(0.5f - 0.1041f, 0.75f), Ogre::Vector2(0, 0), _players[0].controllerId, EG_RA));
	_godsButtons.push_back(aux2);


	Entity * aux3 = new Entity("ZeusP0", this);
	aux->addComponent(new CGodButton(overlay, aux3, 2, Ogre::Vector2(0.5f, 0.75f), Ogre::Vector2(0, 0), _players[0].controllerId, EG_ZEUS));
	_godsButtons.push_back(aux3);


	Entity * aux4 = new Entity("HachimanP0", this);
	aux->addComponent(new CGodButton(overlay, aux4, 3, Ogre::Vector2(0.5f  + 0.1041f, 0.75f), Ogre::Vector2(0, 0), _players[0].controllerId, EG_HACHIMAN));
	_godsButtons.push_back(aux4);


#pragma endregion




	for (int i = 0; i < 4; i++){
		addEntity(_godsButtons[i]);
	}

	overlay->show();

	//Set the first button selected
	getMessage(new MButtonAct(_id, selectedButton));

}


SelectGodScene::~SelectGodScene(){

	delete light;
	light = nullptr;

	delete cam;
	cam = nullptr;

	//delete cCam;
	//cCam = nullptr;

	//delete sky;
	//sky = nullptr;
}



bool SelectGodScene::run(){
	//Here we would get the time between frames

	//Take messages from input
	InputManager::getInstance().getMessages(_messages);
	//Then we deliver the messages
	dispatch();

	processScnMsgs();

	//Logic simulation done here
	bool aux = updateEnts(0.025);

	//Clear dispatched messages
	clearMessageQueue();


	return aux;

}

void SelectGodScene::dispatch(){
	GameScene::dispatch();


}

void SelectGodScene::processScnMsgs()
{
	MGodSet* mGodSet;

	int nSceneMessages = _sceneMessages.size();
	for (std::list<Message *>::iterator it = _sceneMessages.begin(); it != _sceneMessages.end();){
		Message* m = (*it);
		MInputState* mInput;

		switch (m->getType()){
		case MSG_INPUT_STATE:
			mInput = static_cast<MInputState*>(m);
			if (mInput->getId() == 0 && !_pReady[0]){
				if (mInput->getCInputState().Button_A == BTT_RELEASED){
					getMessage(new MButtonClick(_id, player1Index));

				}
				else if (mInput->getCInputState().DPad_Right == BTT_RELEASED){
					_entities.at(std::to_string(player1Index))->getMessage(new MDesSeleGodRender(_entities.at(std::to_string(player1Index))->getID()));
					player1Index++;
					if (player1Index > 3) player1Index = 0;
					getMessage(new MButtonAct(_id, player1Index));
					_entities.at(std::to_string(player1Index))->getMessage(new MActSeleGodRender(_entities.at(std::to_string(player1Index))->getID()));

				}
				else if (mInput->getCInputState().DPad_Left == BTT_RELEASED){
					_entities.at(std::to_string(player1Index))->getMessage(new MDesSeleGodRender(_entities.at(std::to_string(player1Index))->getID()));
					player1Index--;
					if (player1Index < 0) player1Index = 3;
					getMessage(new MButtonAct(_id, player1Index));
					_entities.at(std::to_string(player1Index))->getMessage(new MActSeleGodRender(_entities.at(std::to_string(player1Index))->getID()));
				}
			}
			else if (mInput->getId() == 1 && _pReady[0]){

				if (mInput->getCInputState().Button_A == BTT_RELEASED){
					getMessage(new MButtonClick(_id, player2Index));

				}
				else if (mInput->getCInputState().DPad_Right == BTT_RELEASED){
					_entities.at(std::to_string(player2Index))->getMessage(new MDesSeleGodRender(_entities.at(std::to_string(player2Index))->getID()));
					player2Index++;
					if (player2Index > 3) player2Index = 0;
					getMessage(new MButtonAct(_id, player2Index));
					_entities.at(std::to_string(player2Index))->getMessage(new MActSeleGodRender(_entities.at(std::to_string(player2Index))->getID()));

				}
				else if (mInput->getCInputState().DPad_Left == BTT_RELEASED){
					_entities.at(std::to_string(player2Index))->getMessage(new MDesSeleGodRender(_entities.at(std::to_string(player2Index))->getID()));
					player2Index--;
					if (player2Index < 0) player2Index = 3;
					getMessage(new MButtonAct(_id, player2Index));
					_entities.at(std::to_string(player2Index))->getMessage(new MActSeleGodRender(_entities.at(std::to_string(player2Index))->getID()));
				}
			}
			break;
		case MSG_GOD_SETTER:
			mGodSet = static_cast<MGodSet*>(m);
			selectGod(mGodSet->getGod(), mGodSet->getId());
			_entities.at(std::to_string(player1Index))->getMessage(new MDesSeleGodRender(_entities.at(std::to_string(player1Index))->getID()));
			_entities.at(std::to_string(player2Index))->getMessage(new MActSeleGodRender(_entities.at(std::to_string(player2Index))->getID()));
			break;
		default:
			break;

		}
		it++;
		_sceneMessages.pop_front();
	}

};

void SelectGodScene::processInput(ControllerInputState c){

	int totalButtons = _menuEntities.size();

	if (c.DPad_Down == BTT_PRESSED){
		selectedButton++;
		if (selectedButton >= totalButtons)
			selectedButton = 0;
		_menuEntities.at(selectedButton)->getMessage(new MButtonAct(_id, selectedButton));
	}
	else if (c.DPad_Up == BTT_PRESSED){
		selectedButton--;
		if (selectedButton < 0)
			selectedButton = (totalButtons - 1);
		_menuEntities.at(selectedButton)->getMessage(new MButtonAct(_id, selectedButton));
	}

	if (c.Button_A == BTT_PRESSED){
		_menuEntities.at(selectedButton)->getMessage(new MButtonClick(_id, selectedButton));
	}
}

void SelectGodScene::selectGod(E_GOD g, int playerId){

	_players[playerId].god = g;
	_pReady[playerId] = true;

	if (playerId == 0){
		for (int i = 0; i < 4; i++){
			deleteEntity(_godsButtons[i]->getID());
		}
		createButtons();

		for (int i = 4; i < 8; i++){
			addEntity(_godsButtons[i]);
		}

		getMessage(new MButtonAct(_id, 0));
	}

	else{

		E_STAGE stage;
		int rng = std::rand() % 2;

		(rng == 0) ? stage = ES_TEMPLE : stage = ES_ISLANDS;
		Game::getInstance()->changeScene(GAMEPLAY, _players, stage);



	}

}

void SelectGodScene::createButtons(){

#pragma region Player2 Gods

	Entity * aux = new Entity("AhPuchP1", this);
	aux->addComponent(new CGodButton(overlay, aux, 0, Ogre::Vector2(0.5f - (0.1041f * 2.0f), 0.75f), Ogre::Vector2(0, 0), _players[1].controllerId, EG_AHPUCH));
	_godsButtons.push_back(aux);



	Entity * aux2 = new Entity("RaP1", this);
	aux->addComponent(new CGodButton(overlay, aux2, 1, Ogre::Vector2(0.5f - 0.1041f, 0.75f), Ogre::Vector2(0, 0), _players[1].controllerId, EG_RA));
	_godsButtons.push_back(aux2);


	Entity * aux3 = new Entity("ZeusP1", this);
	aux->addComponent(new CGodButton(overlay, aux3, 2, Ogre::Vector2(0.5f, 0.75f), Ogre::Vector2(0, 0), _players[1].controllerId, EG_ZEUS));
	_godsButtons.push_back(aux3);


	Entity * aux4 = new Entity("HachimanP1", this);
	aux->addComponent(new CGodButton(overlay, aux4, 3, Ogre::Vector2(0.5f + 0.1041f, 0.75f), Ogre::Vector2(0, 0), _players[1].controllerId, EG_HACHIMAN));
	_godsButtons.push_back(aux4);



#pragma endregion

}
#pragma endregion


#pragma region Fight Menu Scene
//The main menu class that contains the buttons to access to the diferents menus(Fight, Options, etc)
FightMenuScene::FightMenuScene(std::string id, Game * game) : GameScene(id, game) {

	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	selectedButton = 0;

	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	// Create an overlay
	try {
		overlay = overlayManager.getByName("FIGHT");
	}
	catch (Ogre::Exception e) {
		cout << e.what() << std::endl;
		cout << std::endl;
	}
	// Show the overlay
	//Ogre::FontManager::getSingleton().getByName("TimeText")->load();

	Ogre::Viewport* vp = nullptr;
	Entity *cam = new Entity("camMenuCombate", this);
	cam->addComponent(new CCamera(cam, scnMgr, vp, cam->getID(), Ogre::Vector3(-50, 0, 0), Ogre::Vector3(1, 0, 0), 10));

	/*Entity * background = new Entity("backgroundMainMenu", this);
	background->addComponent(new CSkyPlaneRender(background, scnMgr, 1, 0, "MainMenu", {0,0,0}));
	_menuEntities.emplace(std::pair<int, Entity*>(0, background));*/

	Entity * multiplayerButton = new Entity("multiplayerButton", this);
	multiplayerButton->addComponent(new CNormalButton(overlay, multiplayerButton, 0, Ogre::Vector2(-750, 50), Ogre::Vector2(0, 0), exitCallBack, "Multiplayer"));
	_menuEntities.emplace(std::pair<int, Entity*>(0, multiplayerButton));

	Entity * tutorialButton = new Entity("tutorialButton", this);
	tutorialButton->addComponent(new CNormalButton(overlay, tutorialButton, 0, Ogre::Vector2(-750, 225), Ogre::Vector2(0, 0), exitCallBack, "Tutorial"));
	_menuEntities.emplace(std::pair<int, Entity*>(1, tutorialButton));

	overlay->show();

	//Set the first button selected
	_menuEntities.at(selectedButton)->getMessage(new MButtonAct(_id, selectedButton));

}


FightMenuScene::~FightMenuScene(){
}




bool FightMenuScene::run(){
	//Here we would get the time between frames

	//Take messages from input
	InputManager::getInstance().getMessages(_messages);
	//Then we deliver the messages
	dispatch();

	processScnMsgs();

	//Logic simulation done here
	bool aux = updateEnts(0.025);

	//Clear dispatched messages
	clearMessageQueue();


	return aux;

}

void FightMenuScene::dispatch(){
	GameScene::dispatch();


}

void FightMenuScene::processScnMsgs()
{
	MInputState* input;

	int nSceneMessages = _sceneMessages.size();
	for (std::list<Message *>::iterator it = _sceneMessages.begin(); it != _sceneMessages.end();){
		Message* m = (*it);
		switch (m->getType())
		{
		case MSG_INPUT_STATE:
			input = static_cast<MInputState*>(*it);
			processInput(input->getCInputState());
			break;
		default:
			break;
		}

		it++;
		_sceneMessages.pop_front();
	}



};

void FightMenuScene::processInput(ControllerInputState c){

	int totalButtons = _menuEntities.size();

	if (c.DPad_Down == BTT_PRESSED){
		selectedButton++;
		if (selectedButton >= totalButtons)
			selectedButton = 0;
		_menuEntities.at(selectedButton)->getMessage(new MButtonAct(_id, selectedButton));
	}
	else if (c.DPad_Up == BTT_PRESSED){
		selectedButton--;
		if (selectedButton < 0)
			selectedButton = (totalButtons - 1);
		_menuEntities.at(selectedButton)->getMessage(new MButtonAct(_id, selectedButton));
	}

	if (c.Button_A == BTT_PRESSED){
		_menuEntities.at(selectedButton)->getMessage(new MButtonClick(_id, selectedButton));
	}
}



#pragma endregion

#pragma region Multiplayer Scene
//The main menu class that contains the buttons to access to the diferents menus(Fight, Options, etc)
MultiplayerScene::MultiplayerScene(std::string id, Game * game) : GameScene(id, game) {

	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	state = MS_CHAMP_SELECT;

	players = std::vector<Player>(2);
	players[0].controllerId = 0;
	players[1].controllerId = 1;


}


MultiplayerScene::~MultiplayerScene(){
}




bool MultiplayerScene::run(){
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
	//destroyEntities();

	return aux;

}

void MultiplayerScene::dispatch(){
	GameScene::dispatch();

}

void MultiplayerScene::processScnMsgs()
{

	int nSceneMessages = _sceneMessages.size();
	for (std::list<Message *>::iterator it = _sceneMessages.begin(); it != _sceneMessages.end();){
		Message* m = (*it);

		switch(state){
		case MS_CHAMP_SELECT:
			godSelect(m);
			break;
		case MS_MAP_SELECT:
			mapSelect(m);
			break;
		case MS_LOADING:
			break;
		default:
			break;
		}

	it++;
	_sceneMessages.pop_front();
	}

};

void MultiplayerScene::showChampGui(){};
void MultiplayerScene::showMapGui(){};

void MultiplayerScene::godSelect(Message* m){};
void MultiplayerScene::mapSelect(Message* m){};




#pragma endregion

#pragma region Loading Scene
//The main menu class that contains the buttons to access to the diferents menus(Fight, Options, etc)
LoadingScene::LoadingScene(std::string id, Game * game, GameScene* nextScene) : GameScene(id, game) {

	//Randomly set an background image
	_timeLimit = 6.0f; //6 seconds
	_counter = SDL_GetTicks();

}


LoadingScene::~LoadingScene(){
}




bool LoadingScene::run(){

	float currentTime = SDL_GetTicks();


	if (currentTime - _counter > _timeLimit){

	}

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
	//destroyEntities();

	return aux;

}

void LoadingScene::dispatch(){
	GameScene::dispatch();

}

void LoadingScene::processScnMsgs()
{


};

#pragma endregion

#pragma region InitScene
initScene::initScene() :GameScene("InitScene", Game::getInstance())
{
	
	tInicial = SDL_GetTicks();
	Ogre::Camera * cam = scnMgr->createCamera("InitCam");
	Game::getInstance()->getRenderWindow()->addViewport(cam);
	cam->getViewport()->setBackgroundColour(Ogre::ColourValue(1, 1, 1));
	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
	overlay = overlayManager.getByName("InitGUI");
	overlay->show();

	
}
initScene::~initScene() {
	//Ogre::OverlayManager::getSingleton().destroyAllOverlayElements(); 
}
void initScene::processScnMsgs()
{

}

bool initScene::run() {
	if (SDL_GetTicks() - tInicial >= 5000) {
		Game::getInstance()->changeScene(MAIN_MENU);
	}

	return 0;
}


#pragma endregion