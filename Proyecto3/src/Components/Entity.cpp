#include "Entity.h"
#include "Components.h"
#include <exception>
#include "Messages.h"

//Debug 
#ifdef _DEBUG
#include <iostream>
#endif


#pragma region Constructors and Destructors
Entity::Entity(std::string id, gameScene * sc) :_id(id), scene(sc)
{

}
Entity::~Entity()
{
	dispatch();
	
	for (gameComponent * c : components){
		components.pop_front();
		delete c;
	}
	if (!components.empty())throw new std::exception("Components not deleted correctly");
	while (!msgs.empty()){
		Message * aux = msgs.front();
		msgs.pop();
		delete aux;
	}

}
#pragma endregion

#pragma region Component Interaction
void Entity::addComponent(gameComponent * gc){
	components.push_back(gc);
}
void Entity::deleteComponent(componentType id){
	gameComponent * aux;
	bool found = false;
	for (std::list<gameComponent *>::iterator it = components.begin(); it != components.end() && !found;){
		if ((*it)->getID() == id){
			aux = *it;
			it = components.erase(it);
			delete aux;
			found = false;
		}
		else it++;
	}
}

#pragma endregion

//Pretty self-explainatory
#pragma region Getters and setters
void Entity::setActive(bool nw)
{
	//TO DO: Maybe introduce a new message notifying that we are active/inactive?
	_active = nw;
}

bool Entity::getActive()
{
	return _active;
}

std::string Entity::getID(){
	return _id;
}
#pragma endregion

#pragma region Message methods

void Entity::getMessage(Message * m){
	//If the message is SOMETHING we push it in the queue
	if (m != NULL)msgs.push(m);
}
void Entity::sendMessage(Message * m) {
	//scene->getMessage(m);
}
void Entity::dispatch(){
	/*We only process as many messages as we had at the start of the update.
	 *Any messsage introduced as a result of reading a message is processed
	in the next frame*/
	int N = msgs.size();
		
		for (int i = 0; i < N; i++){
		Message * k = msgs.front();
		for (auto aux : components){
			aux->getMessage(k);
		}

		//After broadcasting the message, we pop and delete it
		msgs.pop();
		delete k;
	}
}

#pragma endregion

#pragma region Tick
void Entity::tick(float delta){
	//Deliver every message in queue
	dispatch();

	//Update every component in the entity
	for (auto comp : components)comp->tick(delta);

}

#pragma endregion