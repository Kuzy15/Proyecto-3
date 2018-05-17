

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Ogre.h>
#include <OgreOverlay.h>
#include <string>
#include <list>
#include <Box2D.h>


class Entity;
class Message;
typedef enum E_BULLET;
typedef void ButtonCallback();
typedef enum E_GOD;



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
	CMP_SKYPLANE_RENDER,
	CMP_PHYSICS,
	CMP_PLAYER_CH,
	CMP_PLAYER_CONTROLLER,
	CMP_LIFE,
	CMP_MOVEMENT_SPEED,
	CMP_JUMP,
	CMP_BASIC_ATTACK,
	CMP_CAMERA,
	CMP_BULLET,
	CMP_PASSIVE_VIDAR,
	CMP_PASSIVE_HADES,
	CMP_PASSIVE_ULL,
	CMP_PASSIVE_VALI,
	CMP_PASSIVE_HERMES,
	CMP_PASSIVE_SYN,
	CMP_SHU_HEADDRESS,
	CMP_JONSU_MOON,
	CMP_KHEPRI_BEETLE,
	CMP_GUI_BUTTON,
	CMP_GUI_PLAYERGUI,
	CMP_HERA_RUNE,
	CMP_HERIS_MARK,
	CMP_PARTICLE_RENDER,
	CMP_CAMERA_FOLLOW

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

//--------- PARTICLE RENDER COMPONENT ---------
class CParticleRender : public CRender
{
public:

	CParticleRender(Ogre::Vector3 pos, std::string id,std::string particleSystem, Entity * father, Ogre::SceneManager * scnM, Ogre::Vector3 scale, Ogre::Vector3 rotation);
	~CParticleRender();


	virtual void tick(float delta);
	virtual void getMessage(Message * m);
private:
	Ogre::ParticleSystem* _particleSystem;



};

//--------- RIBBON TRAIL RENDER COMPONENT ---------
class CRibbonTrailRender : public CRender
{
public:

	CRibbonTrailRender(Ogre::Vector3 pos, std::string id, std::string trail, Entity * father, Ogre::SceneManager * scnM, Ogre::Vector3 scale, Ogre::Vector3 rotation);
	~CRibbonTrailRender();


	virtual void tick(float delta);
	virtual void getMessage(Message * m);
private:
	Ogre::RibbonTrail * trail;

};


//--------- SKYPLANE RENDER COMPONENT ---------
class CSkyPlaneRender : public CRender
{
public:

	CSkyPlaneRender(Entity * father, Ogre::SceneManager * scnM, float scale, float bow, std::string materialName);
	~CSkyPlaneRender();


	virtual void tick(float delta);
	virtual void getMessage(Message * m);
	
private:
	
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
	CActionCamera(Entity * father, Ogre::SceneManager * scnMgr, Ogre::Viewport * vp, float xBoundary, float yBoundary, float minZ, float maxZ);
	~CActionCamera();

	virtual void getMessage(Message * m);
	virtual void tick(float delta);
	bool outOfBoundaries(const Ogre::Vector3 &pos);


	const float BOUNDARY_X;
	const float BOUNDARY_Y;

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

/*---------------------------------- CAMERA FOLLOW -----------------------------------*/
class CCameraFollow : public GameComponent
{
public:
	CCameraFollow(Entity * father);
	virtual ~CCameraFollow();

	virtual void getMessage(Message * m);
	virtual void tick(float delta);
private:
	Ogre::Vector3 _nPos;

};

/*-------------------------PHYSICS COMPONENTS------------------------------------*/

typedef enum FilterMask {
	MASK_PLAYER = 0x0001,
	MASK_STATIC_TERRAIN = 0x0002,
	MASK_DINAMIC_TERRAIN = 0x0004,
	MASK_BULLET_0 = 0x0008,
	MASK_HEAD_0 = 0x0010,
	MASK_CHEST_0 = 0x0020,
	MASK_LEGS_0 = 0x0040,
	MASK_BULLET_1 = 0x1000,
	MASK_HEAD_1 = 0x0800,
	MASK_CHEST_1 = 0x0400,
	MASK_LEGS_1 = 0x0200,
	MASK_FOOT_SENSOR = 0x0080, 
	MASK_DEATHZONE = 0x0100////mil es el ultimo usado, 0x1000
	

};



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

};



/*-----------------------------	PASSIVE SKILL COMPONENTS	--------------------*/
class CAbility : public GameComponent
{
public:
	CAbility(ComponentType c,Entity * father, float componentLife, float componentArmor, uint16 mask); 
	~CAbility();

	virtual void getMessage(Message* m);

protected:
	float _componentLife;
	float _componentArmor;
	float _coolDown;
	uint16 _myMask;
	float _limitLife;

	
};

GameComponent* createPassiveAbilityEmpty(Entity* father, int id);
class CPSkillEmpty : public CAbility
{
public:
	CPSkillEmpty(Entity * father);
	~CPSkillEmpty();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};

GameComponent* createActiveAbilityEmpty(Entity* father, int id);
class CASkillEmpty : public CAbility
{
public:
	CASkillEmpty(Entity * father);
	~CASkillEmpty();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};


//invisibility
GameComponent* createAbilityVidar(Entity* father, int id);
class CPSkillVidar : public CAbility
{
public:
	CPSkillVidar(Entity * father); 
	~CPSkillVidar();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};


//Increase damage of a god
GameComponent* createAbilityHades(Entity* father, int id);
class CPSkillHades : public CAbility
{
public:
	CPSkillHades(Entity * father); 
	~CPSkillHades();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};



//Modify velocity of a god
GameComponent* createAbilityUll(Entity* father, int id);
class CPSkillUll : public CAbility
{
public:
	CPSkillUll(Entity * father); 
	~CPSkillUll();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};

//modify vel of bullets
GameComponent* createAbilityVali(Entity* father, int id);
class CPSkillVali : public CAbility
{
public:
	CPSkillVali(Entity * father); 
	~CPSkillVali();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};


//Modify velocity and jump of a god
GameComponent* createAbilityHermes(Entity* father, int id);
class CPSkillHermes : public CAbility
{
public:
	CPSkillHermes(Entity * father); 
	~CPSkillHermes();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};


//Modify vel of fire rate
GameComponent* createAbilitySyn(Entity* father, int id);
class CPSkillSyn : public CAbility
{
public:
	CPSkillSyn(Entity * father); 
	~CPSkillSyn();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);


};


/*-----------------------------	ACTIVE ABILITIES COMPONENTS	--------------------*/
//Shu Headdress, dash
GameComponent* createAbilityShuHeaddress(Entity* father, int id);
class CShuHeaddress : public CAbility
{
public:
	CShuHeaddress(Entity * father, int id); 
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

//Jonsu Moon, give mov speed during 5s
GameComponent* createAbilityJonsuMoon(Entity* father, int id);
class CJonsuMoon : public CAbility
{
public:
	CJonsuMoon(Entity * father, int id); 
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

//Khepri Beetle, increase fireRate during 3s
GameComponent* createAbilityKhepriBeetle(Entity* father, int id);
class CKhepriBeetle : public CAbility
{
public:
	CKhepriBeetle(Entity * father, int id); 
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

//Hera´s Rune, Restore life of Cards
GameComponent* createAbilityHeraRune(Entity* father, int id);
class CHeraRune : public CAbility
{
public:
	CHeraRune(Entity * father, int id); 
	~CHeraRune();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:

	int _playerId;
	float _timeCounter;
	float _initTime;
	bool isAvailable;


};

// Heris' Mark, plus 20% damage on next 10 attacks
GameComponent* createAbilityHerisMark(Entity* father, int id);
class CHerisMark: public CAbility
{
public:
	CHerisMark(Entity * father, int id);
	~CHerisMark();

	virtual void tick(float delta);
	virtual void getMessage(Message * m);

private:

	int _playerId;
	float _timeCounter;
	float _initTime;
	float _timeActiveLimit;
	bool _isActive;
	bool isAvailable;
	int _availableShots;
	bool _maxShots;

};



/*-------------------------------------------------------GUI COMPONENTS---------------------------------------------------------------------------*/
class CButtonGUI : public GameComponent
{
public:
	CButtonGUI(Ogre::Overlay * overlay, Entity * father, ButtonCallback callback, std::string buttonTxt, size_t _id, Ogre::Vector2 screenpos, Ogre::Vector2 pixelSize);
	~CButtonGUI();
	virtual void tick(float delta);
	virtual void getMessage(Message * m);
	size_t getScnId();

private:
	void toggleClick(bool click);
	void toggleActive(bool active);
	bool canClick();
	std::string materials[3];
	Ogre::OverlayContainer * pContainer;
	Ogre::Overlay * pOver;

	bool _active;
	bool _clicked;

	//The id that the button will have in the scene
	size_t _sceneId;
	ButtonCallback * _callback;
	Ogre::String _txt;



	float _lastClick;
	const float _minClickTime = 500;
};

enum guiPlayer{
	P1 = 0, P2 = 1
};


class CPlayerGUI: public GameComponent
{
public:
	CPlayerGUI(Entity* father, Ogre::Overlay * ov, guiPlayer p, E_GOD character);
	~CPlayerGUI();
	void updateLifebar(size_t val);
	void updateActive();
	virtual void tick(float delta);
	virtual void getMessage(Message * m);
	void updatePassive();

private:
	Ogre::OverlayContainer * pHud;
	Ogre::OverlayContainer * pLowerHud;
	Ogre::OverlayContainer * plifeBar;
	Ogre::OverlayContainer * pActiveBar;
	Ogre::Overlay * pOverlay;
	size_t LIFE_MAX_WIDTH, LIFE_MIN_WIDTH = 5;
	size_t ACTIVE_MAX_WIDTH, ACTIVE_MIN_WIDTH = 5;
	guiPlayer p;
};

#endif