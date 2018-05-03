#include "CollisionManager.h"
#include <Entity.h>
#include "Messages.h"

#include "DebugNew.h"



#ifdef _DEBUG
#include <iostream>
#define new DEBUG_NEW
#endif

CollisionManager::CollisionManager()
{
}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::BeginContact(b2Contact* c){

	

	//Get the pointer to the first entity in the contact
	void* bodyUserData = c->GetFixtureA()->GetBody()->GetUserData();
	if (bodyUserData){
		MCollisionBegin* m = new MCollisionBegin(c->GetFixtureA()->GetFilterData().categoryBits, c->GetFixtureB()->GetFilterData().categoryBits, static_cast<Entity*>(bodyUserData)->getID());
		static_cast<Entity*>(bodyUserData)->getMessage(m);
	}

	//check if fixture B was a ball
	bodyUserData = c->GetFixtureB()->GetBody()->GetUserData();
	if (bodyUserData){
		MCollisionBegin* m = new MCollisionBegin(c->GetFixtureB()->GetFilterData().categoryBits, c->GetFixtureA()->GetFilterData().categoryBits, static_cast<Entity*>(bodyUserData)->getID());
		static_cast<Entity*>(bodyUserData)->getMessage(m);
	}

}


void CollisionManager::EndContact(b2Contact* c){



}