#include "Scenes.h"
#include "Game.h"
#include <Entity.h>

#pragma region gameScene 

gameScene::gameScene(std::string id) :_id(id){

}
gameScene::~gameScene(){

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
			it = _entities.erase(it);
			found = false;
		}
		else it++;
	}
}


#pragma endregion