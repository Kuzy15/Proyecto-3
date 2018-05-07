

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
const float MAX_SPEED = 13.0f;
const int MAX_LIFE = 150;
const float MAX_JUMP_DISTANCE = 150.0f;
const float MAX_FIRE_RATE = 0.0f;

//Pixels per meter
const float PPM = 3.0f;




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
	//CMP_ARMOR,
	CMP_MOVEMENT_SPEED,
	CMP_JUMP,
	CMP_BASIC_ATTACK,
	CMP_CAMERA,
	CMP_BULLET,
	CMP_PASSIVE_SKILL,
	CMP_SHU_HEADDRESS,
	CMP_JONSU_MOON,
	CMP_KHEPRI_BEETLE



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
	ComponentType getComponentType();


  


protected:
	//Used to send a message from the component
	void sendMessage(Message *);

	//Constructor protected so that only
	//other game components inheriting from this class can use it
	GameComponent(ComponentType componentType, Entity * ent);

	//pointer to the components' entity
	Entity * pEnt;
	//basic atributes of the component
	ComponentType _componentType;
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
	virtual ~CRender();
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

	CMeshRender(Ogre::Vector3 pos, std::string meshName, Entity * father, Ogre::SceneManager * scnM, Ogre::Vector3 scale, Ogre::Vector3 rotation);
	~CMeshRender();


	virtual void tick(float delta);
	virtual void getMessage(Message * m);
	Ogre::Vector3 getSize();

private:
	Ogre::Entity * pOgreEnt;
	bool invisActive;           //is invis´ component added

	
	
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
	virtual ~CCamera();
	virtual void tick(float delta);
	virtual void getMessage(Message * m);

protected:
	std::string _camName;
	Ogre::SceneManager * _scnMgr;
	Ogre::Viewport * _vp;
	Ogre::Vector3 _pos, _lastPos, _newPos;
	Ogre::Vector3 _lookAt, _lastLookAt;
	


	//Parameters for the camera movement
	Ogre::Camera * pCam;
};

class CActionCamera: public CCamera
{
public:
	CActionCamera(Entity * father, Ogre::SceneManager * scnMgr, Ogre::Viewport * vp);
	~CActionCamera();

	virtual void getMessage(Message * m);
	virtual void tick(float delta);

	//Cache positions of the players
	Ogre::Vector3 _pj1, _pj2;
	//Vector that we will use to add to our position
	Ogre::Vector3 _toMove;
	//Smooth factor, so the camera does not move sharply
	const float smooth;
	//Max and min Z coordinates for the camera.
	const float MAXZ, MINZ;

	//Z ratio, to calculate the Z after initialization
	float zRatio;

private:

};



/*-------------------------PHYSICS COMPONENTS------------------------------------*/

typedef enum FilterMask {
	MASK_PLAYER = 0x0001,
	MASK_STATIC_TERRAIN = 0x0002,
	MASK_DINAMIC_TERRAIN = 0x0004,
	MASK_BULLET = 0x0008,
	MASK_HEAD = 0x0010,
	MASK_CHEST = 0x0020,
	MASK_LEGS = 0x0040,
	MASK_FOOT_SENSOR = 0x0080 ////0000 0000 1000 0000
	

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

	inline void resetDamage(){ _auxDamageReset = _damage; };
	inline void resetFireRate(){ _auxFireRateReset = _fireRate; };


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

	inline void resetVelocity(){ _auxVelocityReset = _velocity; };

private:
	E_BULLET _bulletType;		//The type of bullet who will be instantiated
	float _damage;				//The damage done by one bullet 
	float _velocity;			//The movement velocity of the bullet
	//rango de explosión
	bool _toDelete;
	float _auxVelocityReset;

};


/////////iria debajo del de vida por mantener un orden
/*-----------------------------	ARMOR COMPONENT	--------------------*/
//Provides life to an entity
/*class CArmor : public GameComponent
{
public:
	CArmor(Entity * father, float iniArmor);
	~CArmor();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

	inline void setNewArmor(float BA){ _maxArmor += BA; _currentArmor += BA; };

	//Returns the current armor of the entity
	inline int getCurrentArmor(){ return _currentArmor; };

private:
	float _maxArmor;			//Max number for the armor
	float _currentArmor;		//The current armor of the entity
};
*/

/*-----------------------------	PASSIVE SKILL COMPONENTS	--------------------*/
class CAbility : public GameComponent
{
public:
	CAbility(ComponentType c,Entity * father, float componentLife, float componentArmor); //pasarle vida y armadura por parametros por si se quiere testear, sino se ponen a cholon y ni tan maaaaaal
	~CAbility();

	

protected:
	float _componentLife;
	float _componentArmor;
	float _coolDown;
	
};

//invisibility
class CPSkillVidar : public CAbility
{
public:
	CPSkillVidar(Entity * father); //pasarle vida y armadura por parametros por si se quiere testear, sino se ponen a cholon y ni tan maaaaaal
	~CPSkillVidar();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};


//Increase damage of a god
class CPSkillHades : public CAbility
{
public:
	CPSkillHades(Entity * father); //pasarle vida y armadura por parametros por si se quiere testear, sino se ponen a cholon y ni tan maaaaaal
	~CPSkillHades();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};



//Modify velocity of a god
class CPSkillUll : public CAbility
{
public:
	CPSkillUll(Entity * father); //pasarle vida y armadura por parametros por si se quiere testear, sino se ponen a cholon y ni tan maaaaaal
	~CPSkillUll();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};

//modify vel of bullets
class CPSkillVali : public CAbility
{
public:
	CPSkillVali(Entity * father); //pasarle vida y armadura por parametros por si se quiere testear, sino se ponen a cholon y ni tan maaaaaal
	~CPSkillVali();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};


//Modify velocity and jump of a god
class CPSkillHermes : public CAbility
{
public:
	CPSkillHermes(Entity * father); //pasarle vida y armadura por parametros por si se quiere testear, sino se ponen a cholon y ni tan maaaaaal
	~CPSkillHermes();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};


//Modify vel of fire rate
class CPSkillSyn : public CAbility
{
public:
	CPSkillSyn(Entity * father); //pasarle vida y armadura por parametros por si se quiere testear, sino se ponen a cholon y ni tan maaaaaal
	~CPSkillSyn();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};


/*-----------------------------	ACTIVE ABILITIES COMPONENTS	--------------------*/
//Shu Headdress
class CShuHeaddress : public CAbility
{
public:
	CShuHeaddress(Entity * father, int id); //pasarle vida y armadura por parametros por si se quiere testear, sino se ponen a cholon y ni tan maaaaaal
	~CShuHeaddress();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:

	b2Vec2* calculateDash(float xValue, float yValue);

	int _playerId;
	float _timeCounter;
	float _lastTimeDash;
	float _dashImpulse;
	

};

//Jonsu Moon
class CJonsuMoon : public CAbility
{
public:
	CJonsuMoon(Entity * father, int id); //pasarle vida y armadura por parametros por si se quiere testear, sino se ponen a cholon y ni tan maaaaaal
	~CJonsuMoon();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:

	int _playerId;
	float _timeCounter;
	float _initTime;
	float _timeActiveLimit;
	float _velocityPercentage;
	bool _isActive;
	bool isAvailable;

};

//Khepri Beetle
class CKhepriBeetle : public CAbility
{
public:
	CKhepriBeetle(Entity * father, int id); //pasarle vida y armadura por parametros por si se quiere testear, sino se ponen a cholon y ni tan maaaaaal
	~CKhepriBeetle();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:

	int _playerId;
	float _timeCounter;
	float _initTime;
	float _timeActiveLimit;
	float _fireRatePercentage;
	bool _isActive;
	bool isAvailable;

};

#endif