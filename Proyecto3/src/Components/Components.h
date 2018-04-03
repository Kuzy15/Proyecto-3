

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Ogre.h>
#include <string>
#include <list>

class Entity;
class Message;


typedef enum componentType {
	//to be deleted
	STRING_COMPONENT,
	MESSAGESEND_COMPONENT,
	MESH_RENDER_COMPONENT

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

#endif