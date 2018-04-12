#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H
#include <Box2D.h>

class CollisionManager :
	public b2ContactListener
{
public:
	CollisionManager();
	virtual ~CollisionManager();

	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);
	
	//void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	
	//void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
	
};
#endif
