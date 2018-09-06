

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Ogre.h>
//#include <OgreOverlay.h>
#include <string>
#include <list>
#include <Box2D.h>
#include <OgreTextAreaOverlayElement.h>
#include <irrKlang.h>
#include "Messages.h"

class Entity;
class Message;
typedef enum E_BULLET;
typedef void ButtonCallback();
typedef enum E_GOD;




/*---------------------------- CONSTANTS DEFINITION ----------------------*/
//Limits for the components variables 
const float MAX_SPEED = 13.0f;
const int MAX_LIFE = 150;
const float MAX_JUMP_DISTANCE = 90.0f;
const float MAX_FIRE_RATE = 200.0f;

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
	CMP_GUI_TIMER,
	CMP_HERA_RUNE,
	CMP_HERIS_MARK,
	CMP_PARTICLE_RENDER,
	CMP_ANIMATION,
	CMP_CAMERA_FOLLOW,
	CMP_NORMAL_BUTTON,
	CMP_ACTIVE_DEFAULT,
	CMP_PASSIVE_DEFAULT

};

std::string compToString(ComponentType t, int &type);


typedef void ButtonAbilityCallback(int playerId, ComponentType c, int type);

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

#endif