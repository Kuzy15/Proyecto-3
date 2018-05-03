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

	
	void* bodyUserDataA = c->GetFixtureA()->GetBody()->GetUserData();
	void* bodyUserDataB = c->GetFixtureB()->GetBody()->GetUserData();
	//Get the pointer to the first entity in the contact
	if (bodyUserDataA){
		MCollisionBegin* m = new MCollisionBegin(c->GetFixtureA()->GetFilterData().categoryBits, c->GetFixtureB()->GetFilterData().categoryBits, static_cast<Entity*>(bodyUserDataB), static_cast<Entity*>(bodyUserDataA)->getID());
		static_cast<Entity*>(bodyUserDataA)->getMessage(m);
	}

	//check if fixture B was a ball
	
	if (bodyUserDataB){
		MCollisionBegin* m = new MCollisionBegin(c->GetFixtureB()->GetFilterData().categoryBits, c->GetFixtureA()->GetFilterData().categoryBits, static_cast<Entity*>(bodyUserDataA), static_cast<Entity*>(bodyUserDataB)->getID());
		static_cast<Entity*>(bodyUserDataB)->getMessage(m);
	}

}


void CollisionManager::EndContact(b2Contact* c){



}