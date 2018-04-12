#include "CollisionManager.h"
#include <Entity.h>

CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::BeginContact(b2Contact* c){

	

	//Get the pointer to the first entity in the contact
	void* bodyUserData = c->GetFixtureA()->GetBody()->GetUserData();
	if (bodyUserData)
		static_cast<Entity*>(bodyUserData)->getMessage();

	//check if fixture B was a ball
	bodyUserData = c->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserData)
		static_cast<Entity*>(bodyUserData)->getMessage();

}


void CollisionManager::EndContact(b2Contact* c){



}