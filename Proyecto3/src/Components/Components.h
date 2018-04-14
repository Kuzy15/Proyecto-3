

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
typedef enum rigidBodyType {
	
	DYNAMIC,
	STATIC,
	KINEMATIC
};

typedef enum shapeType {

	CIRCLE,
	POLYGON
};

typedef enum componentType {
	//to be deleted
	STRING_COMPONENT,
	MESSAGESEND_COMPONENT,
	MESH_RENDER_COMPONENT,
	PHYSICS_COMPONENT,
	PLAYER_CONTROLLER_COMPONENT,
	LIFE_COMPONENT,
	PLAYER_MOVEMENT_COMPONENT,
	PLAYER_JUMP_COMPONENT,
	PLAYER_BASIC_ATTACK_COMPONENT


};

//Basic gameComponent class from which every other component will inherit.
class gameComponent
{
public:
	
	virtual ~gameComponent();
	//Update method
	virtual void tick(float delta) = 0;
	//Gets a message from the entity and acts according to it
	virtual void getMessage(Message *) = 0;

	//Getters and setters
	bool getActive();
	void setActive(bool nw);
	componentType getID();


  


protected:
	//Used to send a message from the component
	void sendMessage(Message *);

	//Constructor protected so that only
	//other game components inheriting from this class can use it
	gameComponent(componentType id, Entity * ent);

	//pointer to the components' entity
	Entity * pEnt;
	//basic atributes of the component
	componentType _id;
	bool _active;

};
/*-------------------------DEBUG COMPONENTS------------------------------------*/
class stringComponent : public gameComponent
{
public:
	stringComponent(Entity *);
	~stringComponent();
	virtual void getMessage(Message * m);
	virtual void tick(float delta);

private:
	std::string whatSay;

};
class messageSendComponent:public gameComponent
{
public:
	messageSendComponent(Entity * ent);
	~messageSendComponent();
	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:
	int i;

};



/*-------------------------RENDER COMPONENTS------------------------------------*/
//--------- RENDER COMPONENT ---------
class renderComponent : public gameComponent
{
public:
	~renderComponent();
	Ogre::SceneNode * getSceneNode();
	virtual void getMessage(Message *m);

protected:
	renderComponent(componentType t, Entity * father, Ogre::SceneManager * scnM);
	// Inside the Ogre Node we can find al the render values needed by ogre
	Ogre::SceneNode * pOgreSceneNode;
	// Pointer to the Ogre Scene Manager that created the component
	Ogre::SceneManager * pSceneMgr;
	// Cache value for the rendering positions. They will be updated when needed in the tick function
	Ogre::Vector3 _ogrepos;
	Ogre::Quaternion _ogrequat;
};

//--------- MESH RENDER COMPONENT ---------
class meshRenderComponent: public renderComponent
{
public:
	meshRenderComponent(Ogre::Vector3, std::string meshName, Entity * father, Ogre::SceneManager * scnM);
	~meshRenderComponent();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:
	Ogre::Entity * pOgreEnt;
	
};
//--------- PLANE RENDER COMPONENT ---------
class planeRenderComponent: public renderComponent
{
public:
	planeRenderComponent();
	~planeRenderComponent();

private:

};



/*-------------------------PHYSICS COMPONENTS------------------------------------*/
//--------- RIGID BODY COMPONENT ---------
class RigidBodyComponent : public gameComponent
{
public:
	RigidBodyComponent(Entity * father, b2World * world, Ogre::Vector3 posInPixels, float heightInPixels, float weightInPixels, rigidBodyType rbType, shapeType shType);
	~RigidBodyComponent();

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

/*-----------------------------	PLAYER CONTROLLER COMPONENT	--------------------*/
//This component take the input events to send new messages to control the player (attack, move, etc.)
class PlayerControllerComponent : public gameComponent
{
public:
	PlayerControllerComponent(Entity * father, int playerId);
	~PlayerControllerComponent();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

	inline const int getId(){ return _id; };
private:
	int _id;
};


/*-----------------------------	LIFE COMPONENT	--------------------*/
//Provides life to an entity
class LifeComponent : public gameComponent
{
public:
	LifeComponent(Entity * father);
	~LifeComponent();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

	//Returns the current life of the entity
	inline int getCurrentLife(){ return _currentLife; };

private:
	int maxLife;			//Max number for the HPs
	int _currentLife;		//The current life of the entity
};


/*-----------------------------	PLAYER MOVE COMPONENT	--------------------*/
//Set the move velocity for the entity
class PlayerMoveComponent : public gameComponent
{
public:
	PlayerMoveComponent(Entity * father, float vel);
	~PlayerMoveComponent();

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
class PlayerJumpComponent : public gameComponent
{
public:
	PlayerJumpComponent(Entity * father, float dist);
	~PlayerJumpComponent();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

	//Returns the current jump distance of the entity
	inline float getVelocity(){ return _jumpDist; };

private:
	float _maxDistance;			//Max number of jump units
	float _jumpDist;			//The current jump units of the entity
};

/*-----------------------------	PLAYER BASIC ATTACK COMPONENT	--------------------*/
//Provides an entity the capacity shot bullet as basic attack. 
class PlayerBasicAttackComponent : public gameComponent
{
public:
	PlayerBasicAttackComponent(Entity * father, float fireRate, E_BULLET bT);
	~PlayerBasicAttackComponent();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

	//Returns the current fire rate of the entity
	inline float getFireRate(){ return _fireRate; };

private:
	float _maxFireRate;			//Max fire rate
	float _fireRate;			//The current fire rate 
	E_BULLET _bulletType;		//The type of bullet who will be instantiated
};


#endif