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
#include "Components.h"
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

	deleteAllEntities();

	EntityFactory::getInstance().resetInstance();
	scnMgr->clearScene();
	scnMgr->destroyAllManualObjects();

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

	/*for (auto e : _menuEntities){
		aux = e.second;
		if (aux != nullptr)
			delete aux;
	}*/

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
	_prepareLimitTime = 30000.0f; //30 seconds
	_prepareCounter = 0.0f;

	//Load the stage passed by parameter
	loadStage();



	//Store and configure the players structure
	_players = players;
	std::vector<Ogre::Vector3> playersPos(2);
	playersPos[0] = Ogre::Vector3(-30.0f, 0.0f, 0.0f);
	playersPos[1] = Ogre::Vector3(-20.0f, 0.0f, 0.0f);

	int i = 0;
	//for (Player p : _players){
	for (int i = 0;  i < _players.size(); i++){

		_players[i].entity = EntityFactory::getInstance().createGod(_players[i].god, this, playersPos[i], _players[i].controllerId);
		_players[i].abilities.push_back(CMP_HERA_RUNE);
		_players[i].abilities.push_back(CMP_PASSIVE_HADES);
		_players[i].abilities.push_back(CMP_KHEPRI_BEETLE);
		_players[i].currentActive = CMP_ACTIVE_DEFAULT;
		_players[i].currentPassive = CMP_PASSIVE_DEFAULT;
		_players[i].entity->setActive(true);
		addEntity(_players[i].entity);

	}

	//Not paused at start
	_paused = false;





	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

	// Create an overlay
	try {
		overlay = overlayManager.getByName("GUI");
		bgCards = overlayManager.getByName("Background");
	}
	catch (Ogre::Exception e) {
		cout << e.what() << std::endl;
		cout << std::endl;
	}
	// Show the overlay
	Ogre::FontManager::getSingleton().getByName("GUI/TimeText")->load();
	Ogre::FontManager::getSingleton().getByName("GUI/PlayerText")->load();


	Entity * k = new Entity("GUI", this);


	//k->addComponent(new CNormalButton(overlay, k, 0, Ogre::Vector2(-100, 150), Ogre::Vector2(0, 0), K, "Placeholder"));
	k->addComponent(new CGUITimer(k, overlay));
	k->addComponent(new CPlayerGUI(k, overlay, P1, players[0].god));
	k->addComponent(new CPlayerGUI(k, overlay, P2, players[1].god));
	addEntity(k);


	overlay->show();
	//bgCards->show();



	//Set the starter state to SETUP
	_currState = GS_BATTLE;
	_prepareCounter = SDL_GetTicks();

	loadAbilities();

	//getMessage(new MButtonAct(id, 0));
	//getMessage(new MButtonAct(id, 3));

}
GamePlayScene::~GamePlayScene(){
	_players.clear();
	_cardGUIEntities.clear();

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

	return aux;

}

void GamePlayScene::dispatch(){



	nMessages = _messages.size();
	std::list<Message *>::iterator it = _messages.begin();




	switch (_currState){
	case GS_SETUP:
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
				for (std::list<Entity*>::iterator itEnt = _cardGUIEntities.begin(); itEnt != _cardGUIEntities.end(); ++itEnt) {
					if ((*it)->getEmmiter() != (*itEnt)->getID())
						(*itEnt)->getMessage(*it);
				}
			}
		}
		break;
	case GS_BATTLE:
		GameScene::dispatch();
		break;
	case GS_END:

		break;
	default:
		break;
	}




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
		getMessage(new MUpdateSceneTimer(_id,_battleState.timeElapsed));


		//If time is greater than limit, stop battle
		if (_battleState.timeElapsed > TIME_LIMIT){
			_battleState.battleEnded = true;
			//_currState = GS_END;
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
	std::vector<Entity*>* stageEntities = EntityFactory::getInstance().createStage(_stage, this);

	//Then push them to the main array of entities
	for (auto e : (*stageEntities)){
		addEntity(e);
	}
	stageEntities->clear();
	delete stageEntities;
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
	else{
		playerWonId = 0;
	}
	//Increase runds completed
	_players[playerWonId].roundsWon++;
	getMessage(new MRoundFinished(_id,_players[playerWonId].entity->getID()));
	_battleState.roundsCompleted++;

	//If the rounds elapsed are 3, we finish the game. Else, continue.
	if (_battleState.roundsCompleted == MAX_ROUNDS){
		changePhase(GS_END);
	}
	else
		changePhase(GS_SETUP);


}

void GamePlayScene::loadAbilities(){

	Entity* aux;
	Ogre::Vector2 auxPos(-100.0f,150.0f);
	int idCounter = 0;
	for (Player p : _players){
		//There, we should choose 3 random abilities to show
		for (ComponentType c : p.abilities){
			aux = new Entity((to_string(p.controllerId) + to_string(c)), this);
			aux->addComponent(new CAbilityButton(bgCards, aux, idCounter, auxPos, Ogre::Vector2(0, 0), p.controllerId, c));
			auxPos.x += 20.0f;
			//limpiarlas luego
			_cardGUIEntities.emplace_back(aux);
			idCounter++;

		}
		auxPos.y += 30.0f;
	}

}

void GamePlayScene::addAbilityComponent(int playerId, ComponentType compId, int type){

	GameComponent* c = EntityFactory::getInstance().createAbility(compId, _players[playerId].entity, playerId);
	Player p = _players[playerId];
#ifdef _DEBUG
	if (c == nullptr)
		std::cout << "No existe esa habilidad" << std::endl;
#endif
	//if is active or passive
	if (type == 0){
		if (p.currentActive != compId ){
			p.entity->deleteComponent(p.currentActive);
			p.entity->addComponent(c);
		}
		else { delete c; }
	}
	else{
		if (p.currentPassive != compId /*cambiar por default*/){
			p.entity->deleteComponent(p.currentPassive);
			p.entity->addComponent(c);
		}
		else { delete c; }
	}

}



void GamePlayScene::processMsgSetup(Message* m){

	MInputState* mInput;

	switch (m->getType()){
	case MSG_INPUT_STATE:
		mInput = static_cast<MInputState*>(m);
		if (mInput->getCInputState().Button_A == BTT_PRESSED)
			getMessage(new MButtonClick(_id));

		break;

	}


}

void GamePlayScene::processMsgBattle(Message* m){

	MAbilitySet* mAbility;

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
	case MSG_ABILITY_SETTER:
		mAbility = static_cast<MAbilitySet*>(m);
		addAbilityComponent(mAbility->getId(), mAbility->getComponentType(), mAbility->getType());
		break;
	default:
		break;
	}

}

void GamePlayScene::processMsgEnd(Message* m){





}
#pragma endregion

#pragma region Main Menu Scene
//The main menu class that contains the buttons to access to the diferents menus(Fight, Options, etc)
MainMenuScene::MainMenuScene(std::string id, Game * game) : GameScene(id, game) {

	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	selectedButton = 0;

	Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

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
	cam->addComponent(new CCamera(cam, scnMgr, vp, cam->getID(), Ogre::Vector3(-50, 0, 0), Ogre::Vector3(1, 0, 0), 10));

	/*Entity * background = new Entity("backgroundMainMenu", this);
	background->addComponent(new CSkyPlaneRender(background, scnMgr, 1, 0, "MainMenu", {0,0,0}));
		_menuEntities.emplace(std::pair<int, Entity*>(0, background));*/

	Entity * fightButton = new Entity("fightButton", this);
	fightButton->addComponent(new CNormalButton(overlay, fightButton, 0, Ogre::Vector2(-750, 50), Ogre::Vector2(0, 0), exitCallBack, "Combate"));
	_menuEntities.emplace(std::pair<int, Entity*>(0, fightButton));

	Entity * exitButton = new Entity("exitButton", this);
	exitButton->addComponent(new CNormalButton(overlay, exitButton, 0, Ogre::Vector2(-750, 225), Ogre::Vector2(0, 0), exitCallBack, "Salir"));
	_menuEntities.emplace(std::pair<int, Entity*>(1, exitButton));

	overlay->show();

	//Set the first button selected
	_menuEntities.at(selectedButton)->getMessage(new MButtonAct(_id, selectedButton));

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

void MainMenuScene::processInput(ControllerInputState c){

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
		_menuEntities.at(selectedButton)->getMessage(new MButtonClick(_id));
	}
}



#pragma endregion


#pragma region Select God Scene
//The main menu class that contains the buttons to access to the diferents menus(Fight, Options, etc)
SelectGodScene::SelectGodScene(std::string id, Game * game, std::vector<Player> players) : GameScene(id, game) {

	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	_players = players;

	selectedButton = 0;

	player1Index = 0;
	player2Index = 4;

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
	Entity *cam = new Entity("camSelectGod", this);
	CCamera * cCam = new CCamera(cam, scnMgr, vp, cam->getID(), Ogre::Vector3(0, 0, 60), Ogre::Vector3(0, 0, 0), 10);
	vp = cCam->getVP();
	cam->addComponent(cCam);

	/*Entity * background = new Entity("backgroundMainMenu", this);
	background->addComponent(new CSkyPlaneRender(background, scnMgr, 1, 0, "MainMenu", {0,0,0}));
	_menuEntities.emplace(std::pair<int, Entity*>(0, background));*/

	Entity *sky = new Entity("sky", this);
	sky->addComponent(new CSkyPlaneRender(sky, sky->getScene()->getSceneManager(), 1, "MainMenu", Ogre::Vector3{ 0, 0, -5 }, vp));
	addEntity(sky);

	Entity * aux = new Entity("AhPuchP0", this);
	aux->addComponent(new CGodButton(overlay, aux, 0, Ogre::Vector2(-600, 200), Ogre::Vector2(0, 0), _players[0].controllerId, EG_AHPUCH));
	_menuEntities.emplace(std::pair<int, Entity*>(0, aux));
	addEntity(aux);
	Entity* renderAhPuch = new Entity("AhPuchRender", this);
	CMeshRender * meshRender = new CMeshRender({ -6, -5, 40 }, "AhPuch.mesh", renderAhPuch, renderAhPuch->getScene()->getSceneManager(), { 1.6f, 1.6f, 1.6f }, { 0, 0, 0 });
	renderAhPuch->addComponent(meshRender);
	addEntity(renderAhPuch);
	

	Entity * aux2 = new Entity("RaP0", this);
	aux->addComponent(new CGodButton(overlay, aux2, 1, Ogre::Vector2(-400, 200), Ogre::Vector2(0, 0), _players[0].controllerId, EG_RA));
	_menuEntities.emplace(std::pair<int, Entity*>(0, aux2));
	addEntity(aux2);
	Entity* renderAux2 = new Entity("RaRender", this);
	renderAux2->addComponent(new CMeshRender({ -6, -5, 40 }, "Ra.mesh", renderAux2, renderAux2->getScene()->getSceneManager(), { 1.6f, 1.6f, 1.6f }, { 0, 0, 0 }));
	addEntity(renderAux2);

	Entity * aux3 = new Entity("ZeusP0", this);
	aux->addComponent(new CGodButton(overlay, aux3, 2, Ogre::Vector2(-200, 200), Ogre::Vector2(0, 0), _players[0].controllerId, EG_ZEUS));
	_menuEntities.emplace(std::pair<int, Entity*>(0, aux3));
	addEntity(aux3);
	Entity* renderAux3 = new Entity("ZeusRender", this);
	renderAux3->addComponent(new CMeshRender({ -6, -5, 40 }, "Zeus.mesh", renderAux3, renderAux3->getScene()->getSceneManager(), { 1.6f, 1.6f, 1.6f }, { 0, 0, 0 }));
	addEntity(renderAux3);

	Entity * aux4 = new Entity("HachimanP0", this);
	aux->addComponent(new CGodButton(overlay, aux4, 3, Ogre::Vector2(0, 200), Ogre::Vector2(0, 0), _players[0].controllerId, EG_HACHIMAN));
	_menuEntities.emplace(std::pair<int, Entity*>(0, aux4));
	addEntity(aux4);
	Entity* renderAux4 = new Entity("HachimanRender", this);
	renderAux4->addComponent(new CMeshRender({ -6, -5, 40 }, "Hachiman.mesh", renderAux4, renderAux4->getScene()->getSceneManager(), { 1.6f, 1.6f, 1.6f }, { 0, 0, 0 }));
	addEntity(renderAux4);
	




	overlay->show();

	//Set the first button selected
	_menuEntities.at(selectedButton)->getMessage(new MButtonAct(_id, selectedButton));

}


SelectGodScene::~SelectGodScene(){

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
	/*MInputState* input;

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
	*/

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
				else if (mInput->getCInputState().DPad_Down == BTT_RELEASED){
					player1Index++;
					if (player1Index > 2) player1Index = 0;
					getMessage(new MButtonAct(_id, player1Index));
				}
				else if (mInput->getCInputState().DPad_Up == BTT_RELEASED){
					player1Index--;
					if (player1Index < 0) player1Index = 2;
					getMessage(new MButtonAct(_id, player1Index));
				}
			}
			else if (mInput->getId() == 1 && _pReady[0]){
				if (mInput->getCInputState().Button_A == BTT_RELEASED)
					getMessage(new MButtonClick(_id, player2Index));
				else if (mInput->getCInputState().DPad_Down == BTT_RELEASED){
					player2Index++;
					if (player2Index > 5) player2Index = 3;
					getMessage(new MButtonAct(_id, player2Index));
				}
				else if (mInput->getCInputState().DPad_Up == BTT_RELEASED){
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
		_menuEntities.at(selectedButton)->getMessage(new MButtonClick(_id));
	}
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
		_menuEntities.at(selectedButton)->getMessage(new MButtonClick(_id));
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
