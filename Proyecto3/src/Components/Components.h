

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Ogre.h>
#include <string>
#include <list>
#include <OgreNode.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>

class Entity;
class Message;


typedef enum componentType {
	//to be deleted
	Component_Basic,
	MESH_RENDER_COMPONENT

};


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


class renderComponent : public gameComponent
{
public:
	~renderComponent();
	Ogre::SceneNode * getSceneNode();

protected:
	renderComponent(componentType t, Entity * father, Ogre::SceneManager * scnM);

private:
	Ogre::SceneNode * pOgreSceneNode;
	Ogre::SceneManager * pSceneMgr;
};

class meshRenderComponent: public renderComponent
{
public:
	meshRenderComponent(std::string meshName, Entity * father, Ogre::SceneManager * scnM);
	~meshRenderComponent();

private:
	Ogre::Entity * pOgreEnt;

};

#endif