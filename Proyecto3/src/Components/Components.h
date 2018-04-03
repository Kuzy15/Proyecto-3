

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Ogre.h>
#include <string>
#include <list>
#include <Box2D.h>

class Entity;
class Message;
//Pixels per meter
const int PPM = 100;

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
	INPUT_BASE_COMPONENT,
	PHYSICS_COMPONENT


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
class rigidBodyComponent : public gameComponent
{
public:
	rigidBodyComponent(Entity * father, b2World * world, Ogre::Vector2 posInPixels, float heightInPixels, float weightInPixels, rigidBodyType rbType, shapeType shType);
	~rigidBodyComponent();

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


#endif