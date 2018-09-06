#ifndef PLAYERCOMPONENTS_H
#define PLAYERCOMPONENTS_H

#include "Components.h"
#include <Box2D.h>

//class Entity;
//class Message;

typedef enum FilterMask;



//--------- RIGID BODY COMPONENT ---------
class CRigidBody : public GameComponent
{
public:

	CRigidBody(Entity * father, b2World * world, Ogre::Vector3 posInPixels, float heightInPixels, float weightInPixels, float angle, RigidBodyType rbType, ShapeType shType, FilterMask myCategory, int controllerId);
	virtual ~CRigidBody();


	virtual void tick(float delta);
	virtual void getMessage(Message * m);


private:

	float _rbWeight;
	float _rbHeight;
	b2World* _myWorld;
	b2Vec2 _pos;
	b2Vec2 _vel;
	b2Body* _body;
	b2BodyDef _bodyDef;
	b2FixtureDef _fixtureDef;
	b2Fixture* _fixture;

	MUpdateTransform * _updateTransform;

};
/*
//--------- COLLISION HANDLER COMPONENT ---------
//Interface for collision handler component
class CollisionHandlerComponent : public gameComponent
{
public:
CollisionHandlerComponent();
~CollisionHandlerComponent();

virtual void onCollisionEnter();
virtual void onCollisionEnd();

private:

};


*/
//--------- PLAYER C.H. COMPONENT ---------
class CPlayerCollisionHandler : public GameComponent
{
public:
	CPlayerCollisionHandler(Entity* father);
	virtual ~CPlayerCollisionHandler();

	void tick(float delta);
	void getMessage(Message * m);
	uint16_t getMyMask(){ return _myMask; }

private:

	//void handleBulletHit();

	uint16_t _myMask;



};

/*-----------------------------	PLAYER CONTROLLER COMPONENT	--------------------*/
//This component take the input events to send new messages to control the player (attack, move, etc.)
class CPlayerController : public GameComponent
{
public:
	CPlayerController(Entity * father, int playerId);
	~CPlayerController();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

	inline const int getId(){ return _id; };
private:
	int _id;
	const float TRIGGER_DEADZONE = 100.0f;
	const float AXIS_DEADZONE = 100.0f;
};


/*-----------------------------	LIFE COMPONENT	--------------------*/
//Provides life to an entity
class CLife : public GameComponent
{
public:
	CLife(Entity * father, float iniLife);
	~CLife();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

	//Returns the current life of the entity
	inline int getCurrentLife(){ return _currentLife; };

private:
	float _maxLife;			//Max number for the HPs
	float _currentLife;		//The current life of the entity
};


/*-----------------------------	PLAYER MOVE COMPONENT	--------------------*/
//Set the move velocity for the entity
class CPlayerMove : public GameComponent
{
public:
	CPlayerMove(Entity * father, float vel);
	~CPlayerMove();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

	//Returns the current life of the entity
	inline float getVelocity(){ return _moveVel; };
	//reset var of component
	inline void resetVel(){ _moveVel = _auxVelReset; };

private:
	float _maxSpeed;			//Max velocity
	float _moveVel;				//The  current movemente speed the entity has
	float _auxVelReset;
};

/*-----------------------------	PLAYER JUMP COMPONENT	--------------------*/
//Provides an entity the capacity to jump
class CPlayerJump : public GameComponent
{
public:
	CPlayerJump(Entity * father, float dist);
	~CPlayerJump();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

	//Returns the current jump distance of the entity
	inline float getForce(){ return _jumpForce; };

	inline void resetForceJump(){ _jumpForce = _auxJumpReset; };

private:
	float _maxForce;			//Max number of jump units
	float _jumpForce;			//The current jump units of the entity

	float _timeCounter;
	float _lastTimeJump;
	float _jumpRate;

	float _auxJumpReset;

	int _nJumps;				//Remaining jumps
	int _maxJumps;				//Max jumps

};

/*-----------------------------	PLAYER BASIC ATTACK COMPONENT	--------------------*/
//Provides an entity the capacity shot bullet as basic attack. 

const float SPAWN_PARSE = 1.0f / 328.0f;

class CPlayerBasicAttack : public GameComponent
{
public:
	CPlayerBasicAttack(Entity * father, float fireRate, E_BULLET bT, Ogre::Vector3 entPos, float damage);
	virtual ~CPlayerBasicAttack();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

	//Returns the current fire rate of the entity
	inline float getFireRate(){ return _fireRate; };

	inline void resetDamage(){ _damage = _auxDamageReset; };
	inline void resetFireRate(){ _fireRate = _auxFireRateReset; };


private:
	float _maxFireRate;			//Max fire rate
	float _fireRate;			//The current fire rate 
	float _auxFireRateReset;
	float _lastTimeShot;		//The last time in SDL time when player spawn a bullet (Fire rate control)
	float _timeCounter;			//Variable for the fire rate control.
	E_BULLET _bulletType;		//The type of bullet who will be instantiated
	float _radius;
	Ogre::Vector3 _ogrepos;
	float _damage;
	float _auxDamageReset;

	void calculateSpawnPoint(float vX, float vY, float &angle, Ogre::Vector3 &iniPos);
};


/*-----------------------------	BULLET COMPONENT	--------------------*/
//Provides an entity the behavior of a bullet
class CBullet : public GameComponent
{
public:
	CBullet(Entity * father, E_BULLET bT, float damage, float vel);
	~CBullet();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

	inline void resetVelocity(){ _velocity = _auxVelocityReset; };

private:
	E_BULLET _bulletType;		//The type of bullet who will be instantiated
	float _damage;				//The damage done by one bullet 
	float _velocity;			//The movement velocity of the bullet
	//rango de explosión
	bool _toDelete;
	float _auxVelocityReset;

	irrklang::ISound* s;

};

#endif