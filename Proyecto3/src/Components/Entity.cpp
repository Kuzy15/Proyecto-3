#include "Entity.h"
#include "Components.h"
#include <exception>

#pragma region Constructors and Destructors
Entity::Entity(std::string id, Scene * sc) :_id(id), scene(sc)
{

}
Entity::~Entity()
{
	dispatch();
	msgs.empty();
	
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