#include "Scenes.h"
#include "Game.h"
#include <Entity.h>
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


#pragma region gameScene 

gameScene::gameScene(std::string id, Game * game) :_id(id), pGame(game){

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
		ent->tick(delta);
	}
	return true;
} 
void gameScene::getMessage(Message * m){
	_messages.push(m);
}
void gameScene::addEntity(Entity * ent){
	_entities.push_back(ent);
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
	
	test1->addComponent(new meshRenderComponent("Ra.mesh", test1, scnMgr));
	
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
	dispatch();

	//Logic simulation done here
	return updateEnts(0.025);

}

void basicScene::dispatch(){
	/*We only process as many messages as we had at the start of the update.
	*Any messsage introduced as a result of reading a message is processed
	in the next frame*/
	int N = _messages.size();

	for (int i = 0; i < N; i++){
		Message * k = _messages.front();
		for (auto aux : _entities){
			aux->getMessage(k);
		}

		//After broadcasting the message, we pop and delete it
		_messages.pop();
		delete k;
	}
}

#pragma endregion