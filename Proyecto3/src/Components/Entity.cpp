#include "Entity.h"
#include "Components.h"
#include <exception>
#include "Messages.h"
#include "Scenes.h"

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
			found = true;
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
	if (m != NULL )
		msgs.push(m);
}
//Method to send a message to the scene
void Entity::sendMessage(Message * m) {
	scene->getMessage(m);
}
void Entity::dispatch(){
	/*We only process as many messages as we had at the start of the update.
	 *Any messsage introduced as a result of reading a message is processed
	in the next frame*/
	int N = msgs.size();
		
	for (int i = 0; i < N; i++) {
		//First we distribute the message across all the components.
		for (auto a : components)
			//If the message is not of type scene (that means that it is either ENTITY or BROADCAST)
			//Or the message is of type SCENE but we did not send it. This means we received it from outside.
			if(msgs.front()->getDestination() != SCENE|| msgs.front()->getEmmiter() != getID())a->getMessage(msgs.front());

		//Then, if the message was sent only for the entity, we delete it 
		if (msgs.front()->getDestination() == ENTITY) {
			Message * aux = msgs.front();
			msgs.pop();
			delete aux;
		}
		//if the message was received/sent from outside (BROADCAST, SCENE)
		else {
			//If the message was sent by this entity, we forward it to the Scene
			if (msgs.front()->getEmmiter() == getID()) 
				sendMessage(msgs.front());
			//Then we remove it from the entity's queue
			msgs.pop();
		}
	}
}

#pragma endregion

#pragma region Tick
void Entity::tick(float delta){
	//Deliver every message in queue
	dispatch();

	//Update every component in the entity
	for (auto comp : components)
		comp->tick(delta);

}

#pragma endregion