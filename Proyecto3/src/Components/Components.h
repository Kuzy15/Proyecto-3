

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Ogre.h>
#include <string>
#include <list>
#include <Box2D.h>


class Entity;
class Message;
typedef enum E_BULLET;

/*---------------------------- CONSTANTS DEFINITION ----------------------*/
//Limits for the components variables 
const float MAX_SPEED = 0.0f;
const int MAX_LIFE = 0;
const float MAX_JUMP_DISTANCE = 0.0f;
const float MAX_FIRE_RATE = 0.0f;

//Pixels per meter
const int PPM = 100;




/*--------------------------- ENUM AND STRUCT DEFINITIONS ----------------*/
typedef enum RigidBodyType {
	
	RB_DYNAMIC,
	RB_STATIC,
	RB_KINEMATIC
};

typedef enum ShapeType {

	SH_CIRCLE,
	SH_POLYGON,
	SH_PLAYER
};

typedef enum ComponentType {
	//to be deleted
	CMP_STRING,
	CMP_MESSAGE_SEND,
	CMP_MESH_RENDER,
	CMP_PHYSICS,
	CMP_PLAYER_CH,
	CMP_PLAYER_CONTROLLER,
	CMP_LIFE,
	CMP_MOVEMENT_SPEED,
	CMP_JUMP,
	CMP_BASIC_ATTACK,
	CMP_CAMERA,
	CMP_BULLET



};

//Basic gameComponent class from which every other component will inherit.
class GameComponent
{
public:
	
	virtual ~GameComponent();
	//Update method
	virtual void tick(float delta) = 0;
	//Gets a message from the entity and acts according to it
	virtual void getMessage(Message *) = 0;

	//Getters and setters
	bool getActive();
	void setActive(bool nw);
	ComponentType getID();


  


protected:
	//Used to send a message from the component
	void sendMessage(Message *);

	//Constructor protected so that only
	//other game components inheriting from this class can use it
	GameComponent(ComponentType id, Entity * ent);

	//pointer to the components' entity
	Entity * pEnt;
	//basic atributes of the component
	ComponentType _id;
	bool _active;

};
/*-------------------------DEBUG COMPONENTS------------------------------------*/
class CString : public GameComponent
{
public:
	CString(Entity *);
	~CString();
	virtual void getMessage(Message * m);
	virtual void tick(float delta);

private:
	std::string whatSay;

};
class CMessageSend:public GameComponent
{
public:
	CMessageSend(Entity * ent);
	~CMessageSend();
	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:
	int i;

};



/*-------------------------RENDER COMPONENTS------------------------------------*/
//--------- RENDER COMPONENT ---------
class CRender : public GameComponent
{
public:
	~CRender();
	//Ogre::SceneNode * getSceneNode();
	virtual void getMessage(Message *m);

protected:
	CRender(ComponentType t, Entity * father, Ogre::SceneManager * scnM);
	// Inside the Ogre Node we can find al the render values needed by ogre
	Ogre::SceneNode * pOgreSceneNode;
	// Pointer to the Ogre Scene Manager that created the component
	Ogre::SceneManager * pSceneMgr;
	// Cache value for the rendering positions. They will be updated when needed in the tick function
	Ogre::Vector3 _ogrepos;
	Ogre::Quaternion _ogrequat;
	Ogre::SceneNode* pChild;
	
};

//--------- MESH RENDER COMPONENT ---------
class CMeshRender: public CRender
{
public:
	CMeshRender(Ogre::Vector3 pos, std::string meshName, Entity * father, Ogre::SceneManager * scnM, Ogre::Vector3 scale);
	~CMeshRender();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:
	Ogre::Entity * pOgreEnt;
	
	
};
/*//--------- PLANE RENDER COMPONENT ---------
class CPlaneRender: public CRender
{
public:
	CPlaneRender();
	~CPlaneRender();

private:

};*/
//---------   CAMERA COMPONENT   ---------
class CCamera: public GameComponent
{
public:
	CCamera(Entity * father, Ogre::SceneManager * scnMgr, Ogre::Viewport * vp, std::string camName, Ogre::Vector3 pos, Ogre::Vector3 lookAt,  int clipDistance);
	~CCamera();
	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:
	std::string _camName;
	Ogre::SceneManager * _scnMgr;
	Ogre::Viewport * _vp;
	Ogre::Vector3 _pos, _lastPos;
	Ogre::Vector3 _lookAt, _lastLookAt;
	
	Ogre::Camera * pCam;
};


/*-------------------------PHYSICS COMPONENTS------------------------------------*/

typedef enum FilterMask {
	MASK_PLAYER = 0x0001,
	MASK_STATIC_TERRAIN = 0x0002,
	MASK_DINAMIC_TERRAIN = 0x0004,
	MASK_BULLET = 0x0008,
	MASK_HEAD = 0x0010,
	MASK_CHEST = 0x0020,
	MASK_LEGS = 0x0040

};



//--------- RIGID BODY COMPONENT ---------
class CRigidBody : public GameComponent
{
public:

	CRigidBody(Entity * father, b2World * world, Ogre::Vector3 posInPixels, float heightInPixels, float weightInPixels, RigidBodyType rbType, ShapeType shType, FilterMask myCategory);
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
	b2Shape* _shape;
	b2FixtureDef _fixtureDef;
	b2Fixture* _fixture;
	
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

<<<<<<< HEAD
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

private:
	float _maxSpeed;			//Max velocity
	float _moveVel;				//The  current movemente speed the entity has
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

private:
	float _maxForce;			//Max number of jump units
	float _jumpForce;			//The current jump units of the entity
	int _nJumps;				//Remaining jumps
	int _maxJumps;				//Max jumps
};

/*-----------------------------	PLAYER BASIC ATTACK COMPONENT	--------------------*/
//Provides an entity the capacity shot bullet as basic attack. 
class CPlayerBasicAttack : public GameComponent
{
public:
	CPlayerBasicAttack(Entity * father, float fireRate, E_BULLET bT);
	~CPlayerBasicAttack();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

	//Returns the current fire rate of the entity
	inline float getFireRate(){ return _fireRate; };

private:
	float _maxFireRate;			//Max fire rate
	float _fireRate;			//The current fire rate 
	float _lastTimeShot;		//The last time in SDL time when player spawn a bullet (Fire rate control)
	float _timeCounter;			//Variable for the fire rate control.
	E_BULLET _bulletType;		//The type of bullet who will be instantiated
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


private:
	E_BULLET _bulletType;		//The type of bullet who will be instantiated
	float _damage;				//The damage done by one bullet 
	float _velocity;			//The movement velocity of the bullet
	//rango de explosión

};

#endif